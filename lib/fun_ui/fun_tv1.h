#pragma once 

#include <Preferences.h>
#include <lvgl.h>

extern Preferences preferences;
extern String wifi_ssid;
extern String wifi_password;

void create_ui_tv1(lv_obj_t *tv);
