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
#include "temp_sensor.h"
#include "pwm_control.h"

// Global variables
int rtc_hour = 0;
int rtc_minutes = 0;
int fridge_temp = 0;

// Mutex
k_mutex_t display_touch_locker;
k_mutex_t pb8_pb9_mutex;
k_mutex_t rtc_update_locker;
k_mutex_t temp_update_locker;

// Completion
k_completion_t wifi_connect_success;
k_completion_t sntp_success;

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

// Init image
k_task_t *k_init_image;

void task_init_image(void *pdata)
{
  lcd_pwm_set_value(0);
  show_init_image();
  tos_task_create(&k_display_touch, "display_touch", task_display_touch, NULL,
                  4, k_display_touch_stk, DISPLAY_TOUCH_TASK_SIZE, 0);
}

// LCD display and touch screen
k_task_t k_display_touch;
uint8_t k_display_touch_stk[DISPLAY_TOUCH_TASK_SIZE];

void task_display_touch(void *pdata)
{
  tos_task_destroy(k_init_image);

  k_err_t err;

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
k_task_t *k_wifi_connect;
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
        tos_task_create_dyn(&k_ntp_time_sync, "ntp_time_sync", task_ntp_time_sync, NULL,
                            4, NTP_TIME_SYNC_SIZE, 0);
      }
    }
  }
}

// TCP test
#if TCP_TEST_ENABLE

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

#endif

// TCP task
k_task_t k_tcp_task;
char *server_ip = "45.76.101.197";
char *server_port = "4000";
int tcp_socket_id = -1;

void task_tcp_task(void *pdata)
{
  tcp_socket_id = tos_sal_module_connect(server_ip, server_port, TOS_SAL_PROTO_TCP);
  if (tcp_socket_id == -1)
  {
    printf("TCP0 connect failed\r\n");
  }
  else
  {
    printf("TCP0 connect success! fd: %d\n", tcp_socket_id);
    tos_sal_module_send(tcp_socket_id, (const void *)"This is TCP Test!\r\n", strlen("This is TCP Test!\r\n"));
  }
  tos_sleep_ms(1000);
  tcp_socket_id = tos_sal_module_close(tcp_socket_id);
  if (tcp_socket_id != -1)
  {
    printf("Close connection success!");
  }
}

// NTP time sync
k_task_t *k_ntp_time_sync;

void task_ntp_time_sync(void *pdata)
{
  while (tos_completion_pend(&wifi_connect_success) != K_ERR_NONE)
    ;
  tos_task_destroy(k_wifi_connect);
  ntp_client();

  tos_completion_post(&sntp_success);
}

//RTC update
k_task_t k_rtc_update;
uint8_t k_rtc_update_stk[RTC_UPDATE_SIZE];

void task_rtc_update(void *pdata)
{
  while (tos_completion_pend(&sntp_success) != K_ERR_NONE)
    ;

  tos_task_destroy(k_ntp_time_sync);
  k_err_t err;

  while (K_TRUE)
  {
    err = tos_mutex_pend(&rtc_update_locker);
    if (err == K_ERR_NONE)
    {
      RTC_TimeTypeDef rtc_timer;
      HAL_RTC_GetTime(&hrtc, &rtc_timer, RTC_FORMAT_BIN);
      rtc_hour = rtc_timer.Hours;
      rtc_minutes = rtc_timer.Minutes;
      tos_mutex_post(&rtc_update_locker);
    }
    tos_sleep_ms(3000);
  }
}

// Temperature update
k_task_t k_temp_update;
uint8_t k_temp_update_stk[TEMP_UPDATE_SIZE];

void task_temp_update(void *pdata)
{
  k_err_t err;

  while (K_TRUE)
  {
    err = tos_mutex_pend(&temp_update_locker);
    if (err == K_ERR_NONE)
    {
      err = tos_knl_sched_lock();
      if (err == K_ERR_NONE)
      {
        fridge_temp = (int)DS18B20_GetCelsiusTemp();
        printf("%d\r\n", fridge_temp);
      }
      tos_knl_sched_unlock();
      tos_mutex_post(&temp_update_locker);
    }
    tos_sleep_ms(500);
  }
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
    ;

  while (K_TRUE)
  {
    // RTC_TimeTypeDef rtc_timer;
    // HAL_RTC_GetTime(&hrtc, &rtc_timer, RTC_FORMAT_BIN);
    // printf("%0.2d:%0.2d:%0.2d\r\n", rtc_timer.Hours, rtc_timer.Minutes, rtc_timer.Seconds);
    // printf("dummy\r\n");
    k_err_t err;
    err = tos_mutex_pend(&rtc_update_locker);
    if (err == K_ERR_NONE)
    {
      printf("%d, %d\r\n", rtc_hour, rtc_minutes);
      tos_mutex_post(&rtc_update_locker);
    }
    tos_sleep_ms(1000);
  }
}
