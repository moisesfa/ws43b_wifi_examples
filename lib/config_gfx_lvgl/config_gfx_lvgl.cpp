/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/9.2/integration/framework/arduino.html  */

#include <lvgl.h>

/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 *Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 *as the examples and demos are now part of the main LVGL library. */

#include <Arduino_GFX_Library.h>
#include "config_gfx_lvgl.h"
#include "fun_ui_template.h"
#include "fun_ui.h"
#include "fun_tv1.h"

//#include <demos/lv_demos.h>
#ifdef WAVESHARE_ESP32_S3_TFT_4_3B

Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
  5 /* DE */,3 /* VSYNC */,46 /* HSYNC */,7 /* PCLK */,
  1 /* R0 */,2 /* R1 */,42 /* R2 */,41 /* R3 */,40 /* R4 */,
  39 /* G0 */,0 /* G1 */,45 /* G2 */,48 /* G3 */,47 /* G4 */,21 /* G5 */,
  14 /* B0 */,38 /* B1 */,18 /* B2 */,17 /* B3 */,10 /* B4 */,
  
  // Esta configuración es la que mejor funciona de momento
  0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 8 /* hsync_back_porch */,
  0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
  1 /* pclk_active_neg */, 16000000 /* prefer_speed */);
  
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
  800 /* width */,
  480 /* height */,
  rgbpanel,
  0 /* rotation */,
  true /* auto_flush */
);

#endif

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

/*******************************************************************************
 * Please config the touch panel in touch.h
 ******************************************************************************/
#include "touch.h"

uint32_t screenWidth;
uint32_t screenHeight;
uint32_t bufSize;
lv_display_t *disp;
lv_color_t *disp_draw_buf;

//* Creamos una cola
QueueHandle_t lvgl_queue;


//* Implementamos la tarea lvgl que se encarga de procesar lv_task_handler y leer los mensajes de la cola
void task_lvgl(void *pvParameter){
  // Referencia a la estructura
  lvgl_update_t update_widget;
  
  //* Inicialización de los widges, en este caso una etiqueta
  ui_begin();  
  
  while (1)
  {
    //* Procesa lvgl cada 5 ms
    lv_task_handler();
    vTaskDelay(pdMS_TO_TICKS(5));

    //* Revisa la cola para ver si hay actualizaciones que realizar 
    if (xQueueReceive(lvgl_queue, &update_widget, 0) == pdPASS){
        //* Actualiza el widget correspondiente
        if (update_widget.widget_id == 1){  //* Id del widget especifico
          lv_dropdown_set_options_static(dropdown_field, update_widget.new_String.c_str());
          lv_obj_del(spinner_scan);
        }
        if (update_widget.widget_id == 2){  //* Id del widget especifico
          lv_label_set_text(text_label_conn, update_widget.new_String.c_str());
          lv_obj_clear_state(button_retry, LV_STATE_DISABLED);
        }

    }

  }
      
}


#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf)
{
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}
#endif

uint32_t millis_cb(void)
{
  return millis();
}

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);
  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);

  /*Call it to tell LVGL you are ready*/
  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
  if (touch_has_signal())
  {
    if (touch_touched())
    {
      data->state = LV_INDEV_STATE_PRESSED;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
    }
    else if (touch_released())
    {
      data->state = LV_INDEV_STATE_RELEASED;
    }
  }
  else
  {
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void config_gfx_lvgl_init()
{
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX LVGL_Arduino_v9 example ");
  String LVGL_Arduino = String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println(LVGL_Arduino);

  // Init Display
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(BLACK);

  // Init touch device
  touch_init(gfx->width(), gfx->height(), gfx->getRotation());

  lv_init();

  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(millis_cb);

  /* register print function for debugging */
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print);
#endif

  screenWidth = gfx->width();
  screenHeight = gfx->height();

  bufSize = screenWidth * 40;

  disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize*2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (!disp_draw_buf)
  {
    // remove MALLOC_CAP_INTERNAL flag try again
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize*2, MALLOC_CAP_8BIT);
  }
  if (!disp_draw_buf)
  {
    Serial.println("LVGL disp_draw_buf allocate failed!");
  }
  else
  {
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize*2, LV_DISPLAY_RENDER_MODE_PARTIAL);

    /*Initialize the (dummy) input device driver*/
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
    lv_indev_set_read_cb(indev, my_touchpad_read);

    /* Option 1: Create a simple label
     * ---------------------
     */
    // lv_obj_t *label = lv_label_create(lv_scr_act());
    // lv_label_set_text(label, "Hello Arduino, I'm LVGL!(V" GFX_STR(LVGL_VERSION_MAJOR) "." GFX_STR(LVGL_VERSION_MINOR) "." GFX_STR(LVGL_VERSION_PATCH) ")");
    // lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

    /* Option 2: Try an example. See all the examples
     *  - Online: https://docs.lvgl.io/master/examples.html
     *  - Source codes: https://github.com/lvgl/lvgl/tree/master/examples
     * ----------------------------------------------------------------
     */
    // lv_example_btn_1();

    /* Option 3: Or try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMOS_WIDGETS
     * -------------------------------------------------------------------------------------------
     */
    // lv_demo_widgets();
    // lv_demo_benchmark();
    // lv_demo_keypad_encoder();
    // lv_demo_music();
    // lv_demo_stress();

    // *+****************
    // * UI 
    // *+****************
    //wifi_test();  
    //lv_create_main_ui_template();

    //* Crea la cola con espacio para 10 mensajes de tipo lvgl_update_t
    lvgl_queue = xQueueCreate(10,sizeof(lvgl_update_t));    

    //* Creamos las tareas de lvgl y la de actualización 
    xTaskCreate(task_lvgl, "Task lvgl", 1024*5, NULL, 5, NULL); 
    
  }

  Serial.println("Setup done");
}

// void config_gfx_lvgl_loop()
// {
//   lv_task_handler(); /* let the GUI do its work */
//   delay(5);
// }