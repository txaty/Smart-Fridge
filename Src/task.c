#include "task.h"
#include "gpio.h"
#include "lvgl.h"
#include "esp8266.h"
#include "tos_at.h"
#include "sal_module_wrapper.h"
#include "file_handling.h"
#include "sntp.h"
#include "tools.h"
#include "display_content.h"

// Mutex
k_mutex_t display_touch_locker;
k_mutex_t pb8_pb9_mutex;

// Completion
k_completion_t wifi_connect_success;

// LED debug
k_task_t k_led_switch_rgb;
uint8_t k_led_switch_rgb_stk[LED_TASK_STK_SIZE];

void led_switch_rgb(void *pdata)
{
  while (K_TRUE)
  {
    int interval_time = 500;
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    tos_sleep_ms(interval_time);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    tos_sleep_ms(interval_time);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
    tos_sleep_ms(interval_time);
    HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
  }
}

// LCD display and touch screen
k_task_t k_display_touch;
uint8_t k_display_touch_stk[DISPLAY_TOUCH_TASK_SIZE];

void task_display_touch(void *pdata)
{
  k_err_t err;

  while (tos_mutex_pend(&display_touch_locker) != K_ERR_NONE);
  show_init_image();
  tos_mutex_post(&display_touch_locker);


  while (K_TRUE)
  {
    err = tos_mutex_pend(&display_touch_locker);
    if (err == K_ERR_NONE)
    {
      lv_task_handler();
      tos_mutex_post(&display_touch_locker);
    }
    tos_sleep_ms(10);
  }
}

// Wifi initialization and connection to AP
k_task_t k_wifi_connect;
uint8_t k_wifi_connect_stk[WIFI_TEST_CONNECT_SIZE];
char *wifi_ssid = "ASD";
char *wifi_pwd = "qwertyuiop";

void task_wifi_connect(void *pdata)
{
  k_err_t err;
  err = tos_mutex_pend(&pb8_pb9_mutex);
  if (err == K_ERR_NONE)
  {
    switch_pin_for_wifi();
    if (esp8266_sal_init(HAL_UART_PORT_3) == 0)
    {
      if (esp8266_join_ap(wifi_ssid, wifi_pwd) != 0)
      {
        printf("AP joining failed\n");
      }
      else
      {
        printf("AP joning success\n");
        tos_completion_post(&wifi_connect_success);
      }
    }
  }
}

// TCP test
k_task_t k_tcp_test;
uint8_t k_tcp_test_stk[TCP_TEST_SIZE];
char *server_ip = "45.76.101.197";
char *server_port = "4000";
int socket_id_0 = -1;
void task_tcp_test(void *pdata)
{
  socket_id_0 = tos_sal_module_connect(server_ip, server_port, TOS_SAL_PROTO_TCP);
  if (socket_id_0 == -1)
  {
    printf("TCP0 connect failed\r\n");
  }
  else
  {
    printf("TCP0 connect success! fd: %d\n", socket_id_0);
    tos_sal_module_send(socket_id_0, (const void *)"This is TCP Test!\r\n", strlen("This is TCP Test!\r\n"));
  }
  tos_sleep_ms(1000);
  socket_id_0 = tos_sal_module_close(socket_id_0);
  if (socket_id_0 != -1)
  {
    printf("Close connection success!");
  }
}

// NTP time sync
k_task_t k_ntp_time_sync;
uint8_t k_ntp_time_sync_stk[NTP_TIME_SYNC_SIZE];

void task_ntp_time_sync(void *pdata)
{
  while (tos_completion_pend(&wifi_connect_success) != K_ERR_NONE);
  
  ntp_client();
}

// SDIO test
k_task_t k_task_sdio;
uint8_t k_sdio_stk[SDIO_TASK_SIZE];

void task_sdio(void *pdata)
{
  char buffer[200];
  int indx = 0;
  Mount_SD("/");
  Format_SD();
  Unmount_SD("/");
  for (int i = 0; i < 10; ++i)
  {
    Mount_SD("/");
    sprintf(buffer, "Hello ---> %d\n", indx);
    Update_File("FILE1.TXT", buffer);
    sprintf(buffer, "world ---> %d\n", indx);
    Update_File("FILE2.TXT", buffer);
    Unmount_SD("/");

    indx++;
    printf("Debug %d\n", indx);
    osDelay(2000);
  }
}

// Console printf debug
k_task_t k_console_printf_debug;
uint8_t k_console_printf_debug_stk[CONSOLE_PRINTF_DEBUG_SIZE];

void task_console_printf_debug(void *pdata)
{
  while (tos_completion_pend(&wifi_connect_success) != K_ERR_NONE)
  {
    printf("Pend completion failed\r\n");
  }

  while (K_TRUE)
  {
    // RTC_TimeTypeDef rtc_timer;
    // HAL_RTC_GetTime(&hrtc, &rtc_timer, RTC_FORMAT_BIN);
    // printf("%0.2d:%0.2d:%0.2d\r\n", rtc_timer.Hours, rtc_timer.Minutes, rtc_timer.Seconds);
    printf("dummy\r\n");
    tos_sleep_ms(1000);
  }
}

// Application entry
k_task_t k_application_entry;
uint8_t k_application_entry_stk[APPLICATION_ENTRY_SIZE];

void application_entry(void *arg)
{
  printf("Hello TOS!\r\n");
  tos_task_create(&k_display_touch, "display_touch", task_display_touch, NULL,
                  4, k_display_touch_stk, DISPLAY_TOUCH_TASK_SIZE, 0);
  tos_task_create(&k_led_switch_rgb, "led_switch_rgb", led_switch_rgb, NULL,
                  8, k_led_switch_rgb_stk, LED_TASK_STK_SIZE, 0);
  tos_task_create(&k_wifi_connect, "wifi_connect", task_wifi_connect, NULL,
                      4, k_wifi_connect_stk, WIFI_TEST_CONNECT_SIZE, 0);
  tos_task_create(&k_console_printf_debug, "console_printf_debug", task_console_printf_debug, NULL,
                      7, k_console_printf_debug_stk,CONSOLE_PRINTF_DEBUG_SIZE, 0);
  // tos_task_create(&k_ntp_time_sync, "ntp_time_sync", task_ntp_time_sync, NULL,
  //                     4, k_ntp_time_sync_stk, NTP_TIME_SYNC_SIZE, 0);
}
