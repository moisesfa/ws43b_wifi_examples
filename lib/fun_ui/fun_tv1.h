#pragma once 

#include <Arduino.h>
#include <lvgl.h>
//extern Preferences preferences;
extern String wifi_ssid;
extern String wifi_password;

void create_ui_tv1(lv_obj_t *tv);
void lv_wifi_info_gui(lv_obj_t *tv);
void get_preferences(void);

