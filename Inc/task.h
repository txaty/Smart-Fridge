#ifndef _TASK_H_
#define _TASK_H_

#include "cmsis_os.h"
#include "tos_k.h"

#define TCP_TEST_ENABLE 0

// Global variables
extern int rtc_hour;
extern int rtc_minutes;
extern int fridge_temp;

// Mutex
extern k_mutex_t display_touch_locker;
extern k_mutex_t pb8_pb9_mutex;
extern k_mutex_t rtc_update_locker;
extern k_mutex_t temp_update_locker;

// Completion
extern k_completion_t wifi_connect_success;
extern k_completion_t sntp_success;

// LED debug
#define LED_TASK_STK_SIZE 256
extern k_task_t k_led_switch_rgb;
extern uint8_t k_led_switch_rgb_stk[LED_TASK_STK_SIZE];

void led_switch_rgb(void *pdata);

// LCD display and touch screen
#define DISPLAY_TOUCH_TASK_SIZE 4096
extern k_task_t k_display_touch;
extern uint8_t k_display_touch_stk[DISPLAY_TOUCH_TASK_SIZE];

void task_display_touch(void *pdata);

// Wifi connection test
#define WIFI_TEST_CONNECT_SIZE 2048
extern k_task_t *k_wifi_connect;
extern char *wifi_ssid;
extern char *wifi_pwd;

void task_wifi_connect(void *pdata);

// TCP test
#if TCP_TEST_ENABLE

#define TCP_TEST_SIZE 4096
extern k_task_t k_tcp_test;
extern char *server_ip;
extern char *server_port;
extern int socket_id_0;

void task_tcp_test(void *pdata);

#endif



//NTP time sync
#define NTP_TIME_SYNC_SIZE 4096
extern k_task_t *k_ntp_time_sync;

void task_ntp_time_sync(void *pdata);

//RTC update
#define RTC_UPDATE_SIZE 512
extern k_task_t k_rtc_update;
extern uint8_t k_rtc_update_stk[RTC_UPDATE_SIZE];

void task_rtc_update(void *pdata);

// Temperature update
#define TEMP_UPDATE_SIZE 512
extern k_task_t k_temp_update;
extern uint8_t k_temp_update_stk[TEMP_UPDATE_SIZE];

void task_temp_update(void *pdata);

// SDIO test
#define SDIO_TASK_SIZE 512
extern k_task_t k_task_sdio;
extern uint8_t k_sdio_stk[SDIO_TASK_SIZE];

void task_sdio(void *pdata);

// Console printf debug
#define CONSOLE_PRINTF_DEBUG_SIZE 512
extern k_task_t k_console_printf_debug;
extern uint8_t k_console_printf_debug_stk[CONSOLE_PRINTF_DEBUG_SIZE];

void task_console_printf_debug(void *pdata);

#endif