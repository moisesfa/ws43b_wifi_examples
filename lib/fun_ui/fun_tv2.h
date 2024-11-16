#pragma once 
#include <Arduino.h>
#include <lvgl.h>

extern TaskHandle_t xGetDateTimeTaskHandle;
extern bool sync_time_date;

void create_ui_tv2(lv_obj_t *tv); 