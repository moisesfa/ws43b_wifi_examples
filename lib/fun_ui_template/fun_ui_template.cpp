#include "fun_ui_template.h"
#include <lvgl.h>
#include <Arduino.h>

lv_obj_t * scr_1;
lv_obj_t * scr_2;

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
      //lv_screen_load_anim(scr_2,LV_SCR_LOAD_ANIM_MOVE_LEFT, 200, 100, false);
      break;
    case LV_DIR_RIGHT:
      lv_screen_load_anim(scr_1,LV_SCR_LOAD_ANIM_NONE, 0, 0, false);
      //lv_screen_load_anim(scr_1,LV_SCR_LOAD_ANIM_MOVE_RIGHT, 200, 100, false);
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
    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(scr_1, lv_color_hex(0x003a57), LV_PART_MAIN);
    lv_obj_set_style_text_color(scr_1, lv_color_hex(0xffffff), LV_PART_MAIN);

    //lv_obj_set_style_bg_opa(scr_1,LV_OPA_50,0);
    lv_obj_t *screen_1_label=lv_label_create(scr_1);
    lv_label_set_text(screen_1_label, "This is screen_1");
    lv_obj_center(screen_1_label);
    lv_obj_add_event_cb(scr_1, my_event_template, LV_EVENT_GESTURE, NULL);    
}

// ***************************
// **  SCREEN 2 
// ***************************
void create_scr_2(void)
{
    scr_2=lv_obj_create(NULL);
    /*Change the active screen's background color*/
    lv_obj_set_style_bg_color(scr_2, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_style_text_color(scr_2, lv_color_hex(0x003a57), LV_PART_MAIN);

    // lv_obj_set_style_bg_color(scr_2, lv_color_hex(0x003a57), LV_PART_MAIN);
    // lv_obj_set_style_text_color(scr_2, lv_color_hex(0xffffff), LV_PART_MAIN);

    //lv_obj_set_style_bg_opa(scr_2,LV_OPA_50,0);
    lv_obj_t *screen_2_label=lv_label_create(scr_2);
    lv_label_set_text(screen_2_label, "This is screen_2");
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
  
    //lv_screen_load(scr_1);
    lv_screen_load_anim(scr_1,LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 100, 50, false);

    // lv_obj_t *tv4 = lv_obj_create(NULL);
    // lv_obj_set_style_bg_color(tv4, lv_color_hex(0x003a57), LV_PART_MAIN);
    
    // String txt = "Use ESP Touch App Configure your network\n";
    // txt += "1. Install ESPTouch App\n";
    // txt += "2. Turn on ESPTouch -> Click EspTouch\n";
    // txt += "3. Enter Your WiFi Password,\n\tOther setting use default\n";
    // txt += "4. Confirm\n";
    // txt += "5. Click Config WiFi Button\n";
    // txt += "6. Wait config done\n";

    // lv_obj_t *label, *tips_label;
    // tips_label = lv_label_create(tv4);
    // lv_obj_set_width(tips_label, LV_PCT(100));
    // lv_label_set_long_mode(tips_label, LV_LABEL_LONG_SCROLL);
    // lv_obj_set_style_text_color(tips_label, lv_color_white(), LV_PART_MAIN);
    // lv_label_set_text(tips_label, txt.c_str());
    // lv_obj_align(tips_label, LV_ALIGN_TOP_MID, 0, 20);

    // const char *android_url = "https://play.google.com/store/apps/details?id=com.fyent.esptouch.android&pcampaignid=web_share";
    // const char *ios_url     = "https://apps.apple.com/cn/app/espressif-esptouch/id1071176700";

    // lv_color_t bg_color = lv_palette_lighten(LV_PALETTE_GREEN, 5);
    // lv_color_t fg_color = lv_palette_darken(LV_PALETTE_NONE, 4);

    // lv_coord_t size            = 120;
    // //lv_obj_t  *android_rq_code = lv_qrcode_create(tv4, size, fg_color, bg_color);
    // lv_obj_t  *android_rq_code = lv_qrcode_create(tv4);
    // lv_qrcode_update(android_rq_code, android_url, strlen(android_url));
    // lv_obj_set_pos(android_rq_code, 440, 280);
    // // lv_obj_align_to(android_rq_code, tips_label, LV_ALIGN_OUT_BOTTOM_LEFT, 30, 5);
    // /*Add a border with bg_color*/
    // lv_obj_set_style_border_color(android_rq_code, bg_color, 0);
    // lv_obj_set_style_border_width(android_rq_code, 5, 0);
    // label = lv_label_create(tv4);
    // lv_label_set_text(label, "Android");
    // lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    // lv_obj_align_to(label, android_rq_code, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    // //lv_obj_t *ios_rq_code = lv_qrcode_create(tv4, size, fg_color, bg_color);
    // lv_obj_t *ios_rq_code = lv_qrcode_create(tv4);
    // lv_qrcode_update(ios_rq_code, ios_url, strlen(ios_url));
    // lv_obj_align_to(ios_rq_code, android_rq_code, LV_ALIGN_OUT_RIGHT_MID, 20, -5);

    // /*Add a border with bg_color*/
    // lv_obj_set_style_border_color(ios_rq_code, bg_color, 0);
    // lv_obj_set_style_border_width(ios_rq_code, 5, 0);
    // label = lv_label_create(tv4);
    // lv_label_set_text(label, "IOS");
    // lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    // lv_obj_align_to(label, ios_rq_code, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);

    // // BTN
    // lv_obj_t *btn = lv_btn_create(tv4);
    // label         = lv_label_create(btn);
    // lv_label_set_text(label, "Config WiFi");
    // lv_obj_set_width(btn, 250);
    // // lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    // lv_obj_set_style_text_color(label, lv_color_white(), LV_PART_MAIN);
    // lv_obj_align_to(label, btn, LV_ALIGN_CENTER, 0, 0);
    // //lv_obj_add_event_cb(btn, wifi_config_event_handler, LV_EVENT_CLICKED, label);
    // lv_obj_set_pos(btn, 50, 360);

    // lv_screen_load(tv4);

    /*
    static void wifi_config_event_handler(lv_event_t *e)
{
    lv_event_code_t code  = lv_event_get_code(e);
    lv_obj_t       *btn   = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t       *label = (lv_obj_t *)lv_event_get_user_data(e);
    if (code == LV_EVENT_CLICKED) {
        if (smartConfigStart) {
            lv_label_set_text(label, "Config Start");
            if (wifi_timer) {
                lv_timer_del(wifi_timer);
                wifi_timer = NULL;
            }
            WiFi.stopSmartConfig();
            lv_obj_clear_state(btn, LV_STATE_CHECKED);
            Serial.println("return smart Config has Start;");
            smartConfigStart = false;
            return;
        }
        WiFi.disconnect();
        smartConfigStart = true;
        WiFi.beginSmartConfig();
        lv_label_set_text(label, "Config Stop");
        lv_obj_add_state(btn, LV_STATE_CHECKED);

        wifi_timer = lv_timer_create([](lv_timer_t *t) {
            lv_obj_t *btn     = (lv_obj_t *)t->user_data;
            lv_obj_t *label   = lv_obj_get_child(btn, 0);
            bool      destory = false;
            wifi_timer_counter++;
            if (wifi_timer_counter > wifi_connnect_timeout && !WiFi.isConnected()) {
                Serial.println("Connect timeout!");
                destory = true;
                lv_label_set_text(label, "Time Out");
            }
            if (WiFi.isConnected()) {
                Serial.println("WiFi has connected!");
                Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
                Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
                destory   = true;
                String IP = WiFi.localIP().toString();
                lv_label_set_text(label, IP.c_str());
            }
            if (destory) {
                WiFi.stopSmartConfig();
                smartConfigStart = false;
                lv_timer_del(wifi_timer);
                wifi_timer         = NULL;
                wifi_timer_counter = 0;
                lv_obj_clear_state(btn, LV_STATE_CHECKED);
            }
            // Every seconds check conected
        },
        1000, btn);
    }
}

    */
}