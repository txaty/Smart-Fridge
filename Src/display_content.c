#include "display_content.h"
#include "stm32f1xx.h"
#include "stdio.h"
#include "images.h"
#include "tos_k.h"
#include "task.h"

LV_IMAGE_DECLARE(team_logo);
LV_IMAGE_DECLARE(name);
lv_obj_t *team_logo_img = NULL;
lv_obj_t *name_img = NULL;

void show_init_image()
{
    team_logo_img = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(team_logo_img, &team_logo);
    lv_obj_set_pos(team_logo_img, 24, 20);
    lv_task_handler();
    HAL_Delay(500);
    lv_obj_clean(lv_scr_act());

    name_img = lv_img_create(lv_scr_act(), NULL);
    lv_img_set_src(name_img, &name);
    lv_obj_set_pos(name_img, 40, 70);
    lv_task_handler();
    HAL_Delay(1000);
    lv_obj_clean(lv_scr_act());
    lv_task_handler();
}

lv_obj_t *time_label;
lv_obj_t *temp_label;

static void clock_event_update(void *p)
{
    k_err_t err;
    err = tos_mutex_pend(&rtc_update_locker);
    if (err == K_ERR_NONE)
    {
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
        tos_mutex_post(&rtc_update_locker);
    }
}

static void create_clock()
{
    static lv_style_t clock_digit_style;
    lv_style_init(&clock_digit_style);

    lv_style_set_radius(&clock_digit_style, LV_STATE_DEFAULT, 5);
    lv_style_set_bg_color(&clock_digit_style, LV_STATE_DEFAULT, LV_COLOR_SILVER);

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
    lv_obj_align(time_label, NULL, LV_ALIGN_IN_TOP_MID, 0, 10);
    lv_task_create(clock_event_update, 5000, LV_TASK_PRIO_LOW, NULL);
}

static  temp_label_event_update(void *p)
{
    k_err_t err;
    err = tos_mutex_pend(&temp_update_locker);
    if (err == K_ERR_NONE)
    {
        char temp_label_string[30];
        sprintf(temp_label_string, "Temperature: %d", fridge_temp);
        lv_label_set_text(temp_label, temp_label_string);
        tos_mutex_post(&temp_update_locker);
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

    lv_obj_align(temp_background, NULL, LV_ALIGN_IN_TOP_MID, 0, 70);
    lv_obj_align(temp_label, NULL, LV_ALIGN_CENTER, 0, 0);
    lv_task_create(temp_label_event_update, 2000, LV_TASK_PRIO_MID, NULL);
}

void update_main_page()
{
    create_clock();
    create_temp_label();
}