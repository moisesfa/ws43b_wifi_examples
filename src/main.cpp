#include <Arduino.h>
#include "config_gfx_lvgl.h"

void setup()
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  
  config_gfx_lvgl_init();

}

void loop()
{
  config_gfx_lvgl_loop();
}
