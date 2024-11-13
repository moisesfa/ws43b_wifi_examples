#pragma once

//* Creamos una estructura para usarla en la cola 
typedef struct {  
  uint8_t widget_id;
  String new_String;
} lvgl_update_t;

//* Creamos una cola
extern QueueHandle_t lvgl_queue;

#define WAVESHARE_ESP32_S3_TFT_4_3B

void config_gfx_lvgl_init();
//void config_gfx_lvgl_loop();

