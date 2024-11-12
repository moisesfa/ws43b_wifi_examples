#include <Arduino.h>
#include <ESP_IOExpander_Library.h>
#include "expander.h"
#include "Wire.h"

ESP_IOExpander *expander;

void bL_blink(void);

void expander_init() {
  Serial.println("IO expander init...");
  pinMode(GPIO_INPUT_IO_4, OUTPUT);
  
  Wire.begin();
  delay(50);

  expander = new ESP_IOExpander_CH422G(I2C_MASTER_NUM, ESP_IO_EXPANDER_I2C_CH422G_ADDRESS_000);
  expander->init();
  expander->begin();
  expander->multiPinMode(TP_RST | LCD_BL | LCD_RST | SD_CS | USB_SEL, OUTPUT);
  bL_blink();

  //gt911 initialization, must be added, otherwise the touch screen will not be recognized  
  //initialization begin
  expander->multiDigitalWrite(TP_RST | LCD_RST, LOW);
  delay(100);
  digitalWrite(GPIO_INPUT_IO_4, LOW);
  delay(100);
  expander->multiDigitalWrite(TP_RST | LCD_RST, HIGH);
  delay(200);
  //initialization end
}

void bL_set(byte state){
    expander->digitalWrite(LCD_BL, state);
}

void bL_blink(void){
  //Turn off backlight
  bL_set(LOW);
  delay(100);
  bL_set(HIGH);
  delay(100);
}