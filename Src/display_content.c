#include "display_content.h"
#include "stm32f1xx.h"
#include "stdio.h"
#include "images.h"
#include "tos_k.h"
#include "task.h"
#include "rtc.h"
#include "pwm_control.h"

LV_IMAGE_DECLARE(team_logo);
LV_IMAGE_DECLARE(name);
lv_obj_t *team_logo_img = NULL;
lv_obj_t *name_img = NULL;

int flag_lvgl_enable = 1;

lv_obj_t *time_label;
lv_obj_t *temp_label;
lv_obj_t *temp_increment_button;
lv_obj_t *temp_decrement_button;

uint8_t temp_label_state = 1;

void show_init_image()
{
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_RED_GPIO_PIN);
    team_logo_img = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(team_logo_img, &team_logo);
    lv_obj_set_pos(team_logo_img, 24, 20);
    tos_knl_sched_lock();
    lv_task_handler();
    tos_knl_sched_unlock();
    tos_sleep_ms(500);
    lv_obj_del(team_logo_img);
    lv_obj_clean(lv_scr_act());
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_RED_GPIO_PIN);

    name_img = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(name_img, &name);
    lv_obj_set_pos(name_img, 40, 70);
    tos_knl_sched_lock();
    lv_task_handler();
    tos_knl_sched_unlock();
    tos_sleep_ms(1000);
    while (tos_completion_pend(&sntp_success) != K_ERR_NONE)
    {
        tos_sleep_ms(100);
    }
    lv_obj_del(name_img);
    lv_obj_clean(lv_scr_act());
    lv_task_handler();

    update_main_page();
    HAL_GPIO_TogglePin(LED_GPIO_PORT, LED_BLUE_GPIO_PIN);
}

static void clock_event_handler(lv_obj_t *obj, lv_event_t event)
{
    RTC_TimeTypeDef rtc_timer;
    char time_label_string[20];

    switch (event)
    {
    case LV_EVENT_REFRESH:
        HAL_RTC_GetTime(&hrtc, &rtc_timer, RTC_FORMAT_BIN);
        rtc_hour = rtc_timer.Hours;
        rtc_minutes = rtc_timer.Minutes;

        if (rtc_hour < 10 && rtc_minutes < 10)
        {
            sprintf(time_label_string, "0%d : 0%d", rtc_hour, rtc_minutes);
        }
        else if (rtc_hour < 10)
        {
            sprintf(time_label_string, "0%d : %d", rtc_hour, rtc_minutes);
        }
        else if (rtc_minutes < 10)
        {
            sprintf(time_label_string, "%d : 0%d", rtc_hour, rtc_minutes);
        }
        else
        {
            sprintf(time_label_string, "%d : %d", rtc_hour, rtc_minutes);
        }
        lv_label_set_text(obj, time_label_string);
        lv_obj_align(time_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 30);
        break;
    }
}

static void create_clock()
{
    static lv_style_t clock_digit_style;
    lv_style_init(&clock_digit_style);

    // lv_style_set_radius(&clock_digit_style, LV_STATE_DEFAULT, 5);
    // lv_style_set_bg_color(&clock_digit_style, LV_STATE_DEFAULT, LV_COLOR_SILVER);

    lv_style_set_text_color(&clock_digit_style, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_style_set_text_letter_space(&clock_digit_style, LV_STATE_DEFAULT, 5);
    lv_style_set_text_line_space(&clock_digit_style, LV_STATE_DEFAULT, 20);
    lv_style_set_text_font(&clock_digit_style, LV_STATE_DEFAULT, &lv_font_montserrat_48);

    time_label = lv_label_create(lv_scr_act(), NULL);
    lv_obj_add_style(time_label, LV_LABEL_PART_MAIN, &clock_digit_style);
    char time_label_string[20];
    if (rtc_hour < 10 && rtc_minutes < 10)
    {
        sprintf(time_label_string, "0%d : 0%d", rtc_hour, rtc_minutes);
    }
    else if (rtc_hour < 10)
    {
        sprintf(time_label_string, "0%d : %d", rtc_hour, rtc_minutes);
    }
    else if (rtc_minutes < 10)
    {
        sprintf(time_label_string, "%d : 0%d", rtc_hour, rtc_minutes);
    }
    else
    {
        sprintf(time_label_string, "%d : %d", rtc_hour, rtc_minutes);
    }
    lv_label_set_text(time_label, time_label_string);
    lv_obj_align(time_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 30);
    lv_obj_set_event_cb(time_label, clock_event_handler);
}

static void temp_label_event_handler(lv_obj_t *obj, lv_event_t event)
{
    char temp_label_string[30];

    switch (event)
    {
    case LV_EVENT_REFRESH:
        if (temp_label_state)
        {
            sprintf(temp_label_string, "Temperature: %d", fridge_temp);
        }
        else
        {
            sprintf(temp_label_string, "Target: %d", target_temp);
        }
        lv_label_set_text(obj, temp_label_string);
        break;
    }
}

static void temp_increment_button_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (target_temp < TARGET_TEMP_MAX)
        {
            target_temp++;
        }
    }
}

static void temp_decrement_button_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_SHORT_CLICKED || event == LV_EVENT_LONG_PRESSED_REPEAT)
    {
        if (target_temp > TARGET_TEMP_MIN)
        {
            target_temp--;
        }
    }
}

static void temp_background_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_CLICKED)
    {
        temp_label_state = !temp_label_state;
        char temp_label_string[30];
        if (temp_label_state)
        {
            sprintf(temp_label_string, "Temperature: %d", fridge_temp);
            lv_obj_del(temp_decrement_button);
            lv_obj_del(temp_increment_button);
        }
        else
        {
            sprintf(temp_label_string, "Target: %d", target_temp);

            lv_coord_t h = lv_obj_get_height(obj);
            temp_increment_button = lv_btn_create(lv_scr_act(), NULL);
            lv_obj_set_style_local_bg_color(temp_increment_button, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
            lv_obj_set_size(temp_increment_button, h, h);
            lv_obj_align(temp_increment_button, obj, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
            lv_obj_set_style_local_value_str(temp_increment_button, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_PLUS);

            temp_decrement_button = lv_btn_create(lv_scr_act(), temp_increment_button);
            lv_obj_align(temp_decrement_button, obj, LV_ALIGN_OUT_LEFT_MID, -5, 0);
            lv_obj_set_style_local_value_str(temp_decrement_button, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, LV_SYMBOL_MINUS);

            lv_obj_set_event_cb(temp_increment_button, temp_increment_button_event_handler);
            lv_obj_set_event_cb(temp_decrement_button, temp_decrement_button_event_handler);
        }
        lv_label_set_text(temp_label, temp_label_string);
        lv_obj_align(temp_label, NULL, LV_ALIGN_CENTER, 0, 0);
    }
}

static void create_temp_label()
{
    lv_obj_t *temp_background = lv_obj_create(lv_scr_act(), NULL);
    lv_obj_set_width(temp_background, 210);
    lv_obj_set_height(temp_background, 36);
    temp_label = lv_label_create(temp_background, NULL);
    char temp_label_string[30];
    sprintf(temp_label_string, "Temperature: %d", fridge_temp);
    lv_label_set_text(temp_label, temp_label_string);
    lv_obj_set_style_local_text_color(temp_label, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_WHITE);
    lv_obj_set_style_local_text_font(temp_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_22);
    lv_obj_set_style_local_bg_color(temp_background, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);

    lv_obj_align(temp_background, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_align(temp_label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_event_cb(temp_label, temp_label_event_handler);
    lv_obj_set_event_cb(temp_background, temp_background_event_handler);
}

void create_camera_init_label(void)
{
    lv_obj_clean(lv_scr_act());
    lv_obj_t *camera_init_label;
    camera_init_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(camera_init_label, "CAMERA INITIALIZATION");
    lv_obj_set_style_local_text_font(camera_init_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_18);
    lv_obj_align(camera_init_label, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
}

void create_server_init_label(void)
{
    lv_obj_clean(lv_scr_act());
    lv_obj_t *camera_init_label;
    camera_init_label = lv_label_create(lv_scr_act(), NULL);
    lv_label_set_text(camera_init_label, "SERVER MODE");
    lv_obj_set_style_local_text_font(camera_init_label, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, &lv_font_montserrat_18);
    lv_obj_align(camera_init_label, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);
}

static void camera_button_event_handler(lv_obj_t *obj, lv_event_t event)
{

    if (event == LV_EVENT_RELEASED)
    {
        create_camera_init_label();
        flag_take_photo = 1;
        tos_task_create_dyn(&k_camera_init, "camera_init", task_camera_init, NULL,
                            2, CAMERA_INIT_SIZE, 0);
    }
}

static void server_button_event_handler(lv_obj_t *obj, lv_event_t event)
{
    if (event == LV_EVENT_RELEASED)
    {
        create_server_init_label();
        flag_server_connect = 1;
        tos_task_create_dyn(&k_tcp_task, "tcp_task", task_tcp_task, NULL,
                            2, TCP_TASK_SIZE, 0);
    }
}

static void create_buttons()
{
    lv_obj_t *label;

    lv_obj_t *camera_button = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_size(camera_button, 80, 40);
    lv_obj_set_style_local_bg_color(camera_button, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_align(camera_button, NULL, LV_ALIGN_IN_BOTTOM_MID, -50, -30);

    label = lv_label_create(camera_button, NULL);
    lv_label_set_recolor(label, true);
    lv_label_set_text(label, "#ffffff CAMERA#");

    lv_obj_t *server_button = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_size(server_button, 80, 40);
    lv_obj_set_style_local_bg_color(server_button, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_BLACK);
    lv_obj_align(server_button, NULL, LV_ALIGN_IN_BOTTOM_MID, 50, -30);

    label = lv_label_create(server_button, NULL);
    lv_label_set_recolor(label, true);
    lv_label_set_text(label, "#ffffff SERVER#");

    lv_obj_set_event_cb(camera_button, camera_button_event_handler);
    lv_obj_set_event_cb(server_button, server_button_event_handler);
}

void update_main_page()
{
    create_clock();
    create_temp_label();
    create_buttons();
}

void clear_main_page()
{
}
