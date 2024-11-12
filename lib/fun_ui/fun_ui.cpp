#include "fun_ui.h"
#include <lvgl.h>
#include <Arduino.h>
#include "fun_tv1.h"

static lv_obj_t *dis;


void ui_begin()
{
    
    dis = lv_tileview_create(lv_scr_act());
    lv_obj_align(dis, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_size(dis, LV_PCT(100), LV_PCT(100));
    // lv_obj_remove_style(dis, 0, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(dis, lv_color_black(), LV_PART_MAIN);

    lv_obj_t *tv1 = lv_tileview_add_tile(dis, 0, 0, LV_DIR_VER);
    lv_obj_t *tv2 = lv_tileview_add_tile(dis, 0, 1, LV_DIR_VER);
    lv_obj_t *tv3 = lv_tileview_add_tile(dis, 0, 2, LV_DIR_VER);
    lv_obj_t *tv4 = lv_tileview_add_tile(dis, 0, 3, LV_DIR_VER);

    //**************************  
    //**  page 1 */
    //**************************  

    // lv_obj_t * lbl1 = lv_label_create(tv1);
    // lv_label_set_recolor(lbl1,true);
    // lv_obj_center(lbl1);
    // lv_label_set_text(lbl1, "#0000ff Scroll down 1#");
    
    get_preferences();
    
    if (wifi_ssid == "" || wifi_password == ""){
      Serial.println("No values saved for ssid or password");
      create_ui_tv1(tv1);
    }
    else {  
      lv_wifi_info_gui(tv1);
    }    

    //**************************  
    //**  page 2 */
    //**************************  
    
    lv_obj_t * lbl2 = lv_label_create(tv2);
    //lv_label_set_recolor(lbl2,true);
    lv_obj_set_style_text_color(lbl2, lv_color_hex(0xFF5e5c), LV_PART_MAIN);    
    lv_label_set_text(lbl2, "0xFF5e5c tileview 2");
    lv_obj_center(lbl2);
    
    //**************************  
    //**  page 3 */
    //**************************  
    
    lv_obj_t * lbl3 = lv_label_create(tv3);
    //lv_label_set_recolor(lbl3,true);
    lv_obj_set_style_text_color(lbl3, lv_color_hex(0x5eFF5c), LV_PART_MAIN);    
    lv_label_set_text(lbl3, "0x5eFF5c tileview 3");
    lv_obj_center(lbl3);
    
    //**************************  
    //**  page 4 */
    //**************************  

    lv_obj_t * lbl4 = lv_label_create(tv4);
    //lv_label_set_recolor(lbl4,true);
    lv_obj_set_style_text_color(lbl4, lv_color_hex(0x5e5eFF), LV_PART_MAIN);    
    lv_label_set_text(lbl4, "0x5e5eFF tileview 4");
    lv_obj_center(lbl4);

}
