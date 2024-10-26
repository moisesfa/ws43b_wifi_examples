#include "fun_ui_template.h"
#include <lvgl.h>

lv_obj_t * scr_1;
lv_obj_t * scr_2;
lv_obj_t * scr_3;

void my_event_template(lv_event_t * e);
void create_scr_1(void);
void create_scr_2(void);

void my_event_template(lv_event_t * e)
{
  lv_obj_t * screen = (lv_obj_t *)lv_event_get_current_target(e);
  lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());
  LV_LOG_USER("Dir: %d\n", dir);
  switch(dir) {
    case LV_DIR_LEFT:
      lv_screen_load_anim(scr_2,LV_SCR_LOAD_ANIM_NONE, 0, 0, false);
      break;
    case LV_DIR_RIGHT:
      lv_screen_load_anim(scr_1,LV_SCR_LOAD_ANIM_NONE, 0, 0, false);
      break;
    case LV_DIR_TOP:
      break;
    case LV_DIR_BOTTOM:
      break;
  }
}

// ***************************
// **  SCREEN 1 
// ***************************
void create_scr_1(void)
{
    scr_1 = lv_obj_create(NULL);
    lv_obj_set_style_bg_opa(scr_1,LV_OPA_50,0);
    lv_obj_t *screen_1_label=lv_label_create(scr_1);
    lv_label_set_text(screen_1_label, "this is screen_1");
    lv_obj_center(screen_1_label);
    lv_obj_add_event_cb(scr_1, my_event_template, LV_EVENT_GESTURE, NULL);    
}

// ***************************
// **  SCREEN 2 
// ***************************
void create_scr_2(void)
{
    scr_2=lv_obj_create(NULL);
    lv_obj_set_style_bg_opa(scr_2,LV_OPA_50,0);
    lv_obj_t *screen_2_label=lv_label_create(scr_2);
    lv_label_set_text(screen_2_label, "this is screen_2");
    lv_obj_center(screen_2_label);
    lv_obj_add_event_cb(scr_2, my_event_template, LV_EVENT_GESTURE, NULL);   
}

void lv_create_main_ui_template(void)
{

    // lv_obj_t *label = lv_label_create(lv_scr_act());
    // lv_label_set_text(label, "Hello Arduino, I'm LVGL!(V" GFX_STR(LVGL_VERSION_MAJOR) "." GFX_STR(LVGL_VERSION_MINOR) "." GFX_STR(LVGL_VERSION_PATCH) ")");
    // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
    
    create_scr_1();
    create_scr_2();
  
    lv_screen_load(scr_1);
}