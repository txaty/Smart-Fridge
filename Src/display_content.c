#include "display_content.h"
#include "images.h"
#include "tos_k.h"

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
	tos_sleep_ms(2000);
	lv_obj_clean(lv_scr_act());

	name_img = lv_img_create(lv_scr_act(), NULL);
	lv_img_set_src(name_img, &name);
	lv_obj_set_pos(name_img, 10, 70);
	lv_task_handler();
	tos_sleep_ms(2000);
	lv_obj_clean(lv_scr_act());
	lv_task_handler();
}

void update_main_page()
{
	
}