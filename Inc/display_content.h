#ifndef _DISPLAY_CONTENT
#define _DISPLAY_CONTENT

#include "lvgl.h"

extern int flag_lvgl_enable;

extern lv_obj_t *time_label;
extern lv_obj_t *temp_label;
extern uint8_t temp_label_state;

void show_init_image(void);
void update_main_page(void);

#endif