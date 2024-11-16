#pragma once 

#include <Arduino.h>
#include <lvgl.h>
//extern Preferences preferences;

extern lv_obj_t * dropdown_field;
extern lv_obj_t * spinner_scan;
extern lv_obj_t *text_label_conn;
extern lv_obj_t *button_retry;

extern String wifi_ssid;
extern String wifi_password;
extern bool connect_wifi_status;

void create_ui_tv1(lv_obj_t *tv);
void lv_wifi_info_gui(lv_obj_t *tv);
void get_preferences(void);

