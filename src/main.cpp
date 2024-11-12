#include <Arduino.h>
#include "expander.h"
#include "config_gfx_lvgl.h"

void setup()
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  delay(2000);
  expander_init();
  delay(500);
  config_gfx_lvgl_init();

}

void loop()
{
  config_gfx_lvgl_loop();
}
