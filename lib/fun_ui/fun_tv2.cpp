#include "fun_tv2.h"
#include "fun_tv1.h"
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Store date and time
String current_date;
String current_time;

// Store hour, minute, second
static int32_t hour = 0;
static int32_t minute = 0;
static int32_t second = 0;
bool sync_time_date = false;
// Specify the timezone you want to get the time for: https://worldtimeapi.org/api/timezone
// Timezone example for Portugal: "Europe/Lisbon"
const char *timezone = "Europe/Lisbon";
TaskHandle_t xGetDateTimeTaskHandle = NULL;

static lv_obj_t *text_label_time;
static lv_obj_t *text_label_date;

void get_date_and_time();

// Tarea de escaneo WiFi que se ejecutará una vez
void getDateTimeTask(void *parameter)
{
  while (1)
  {
    //Espera notificación 
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    Serial.println("getDateTimeTask running sync_time_date");
    //* Espero hasta que sincronizo fecha y hora  
    while(!sync_time_date) {
      get_date_and_time();
      vTaskDelay(pdMS_TO_TICKS(2000));
    }
    sync_time_date = false;
    vTaskDelay(pdMS_TO_TICKS(10));
  }  
  // Elimina la tarea actual, nunca debe llegar aqui  (self-delete) 
  vTaskDelete(NULL);
}

String format_time(int time)
{
  return (time < 10) ? "0" + String(time) : String(time);
}

static void timer_cb(lv_timer_t *timer)
{
  LV_UNUSED(timer);
  second++;
  if (second > 59)
  {
    //xTaskNotifyGive(xGetDateTimeTaskHandle); // Para pruebas
    second = 0;
    minute++;
    if (minute > 59)
    {
      minute = 0;
      hour++;
      if (hour > 23)
      {
        hour = 0;
      }
      xTaskNotifyGive(xGetDateTimeTaskHandle);
    }
  }

  String hour_time_f = format_time(hour);
  String minute_time_f = format_time(minute);
  String second_time_f = format_time(second);

  String final_time_str = String(hour_time_f) + ":" + String(minute_time_f) + ":" + String(second_time_f);
  // Serial.println(final_time_str);
  lv_label_set_text(text_label_time, final_time_str.c_str());
  lv_label_set_text(text_label_date, current_date.c_str());
}

void create_ui_tv2(lv_obj_t *tv)
{
  // Task to get Date Time
  xTaskCreate(
    getDateTimeTask,          // Función que ejecuta la tarea
    "getDateTimeTask",        // Nombre de la tarea
    1024 * 3,                 // Tamaño de la pila en bytes
    NULL,                     // Parámetro opcional para pasar a la función
    0,                        // Prioridad de la tarea
    &xGetDateTimeTaskHandle   // Puntero opcional para identificar la tarea
  );

  Serial.println("Current Time: " + current_time);
  Serial.println("Current Date: " + current_date);

  lv_timer_t *timer = lv_timer_create(timer_cb, 1000, NULL);
  lv_timer_ready(timer);

  LV_FONT_DECLARE(lv_font_alibaba_sans_bold_80);
  LV_FONT_DECLARE(lv_font_alibaba_sans_bold_140);
  // Create a text label for the time aligned center
  text_label_time = lv_label_create(tv);
  lv_obj_set_style_text_color((lv_obj_t *)text_label_time, lv_palette_main(LV_PALETTE_GREY), 0);  
  lv_label_set_text(text_label_time, "");
  lv_obj_align(text_label_time, LV_ALIGN_CENTER, 0, -80);
  // Set font type and size
  static lv_style_t style_text_label;
  lv_style_init(&style_text_label);
  // lv_style_set_text_font(&style_text_label, &lv_font_montserrat_48);
  lv_style_set_text_font(&style_text_label, &lv_font_alibaba_sans_bold_140);
  lv_obj_add_style(text_label_time, &style_text_label, 0);

  // Create a text label for the date aligned center
  text_label_date = lv_label_create(tv);
  lv_obj_set_style_text_color((lv_obj_t *)text_label_date, lv_color_hex(0x5e5e5c), LV_PART_MAIN);  
  lv_label_set_text(text_label_date, current_date.c_str());
  lv_obj_align(text_label_date, LV_ALIGN_CENTER, 0, 80);

  // Set font type and size
  static lv_style_t style_text_label2;
  lv_style_init(&style_text_label2);
  // lv_style_set_text_font(&style_text_label2, &lv_font_montserrat_30);
  lv_style_set_text_font(&style_text_label2, &lv_font_alibaba_sans_bold_80);
  lv_obj_add_style(text_label_date, &style_text_label2, 0);
}

void get_date_and_time()
{
  if (connect_wifi_status)
  {
    HTTPClient http;
    // Construct the API endpoint
    String url = String("http://worldtimeapi.org/api/timezone/") + timezone;
    http.begin(url);
    int httpCode = http.GET(); // Make the GET request

    if (httpCode > 0)
    {
      // Check for the response
      if (httpCode == HTTP_CODE_OK)
      {
        String payload = http.getString();
        Serial.println("Time information:");
        Serial.println(payload);
        //  Parse the JSON to extract the time
        JsonDocument doc;
        DeserializationError error = deserializeJson(doc, payload);
        if (!error)
        {
          const char *datetime = doc["datetime"];
          // Split the datetime into date and time
          String datetime_str = String(datetime);
          int splitIndex = datetime_str.indexOf('T');
          current_date = datetime_str.substring(0, splitIndex);
          current_time = datetime_str.substring(splitIndex + 1, splitIndex + 9); // Extract time portion
          hour = current_time.substring(0, 2).toInt();
          minute = current_time.substring(3, 5).toInt();
          second = current_time.substring(6, 8).toInt();
          sync_time_date = true;
        }
        else
        {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.c_str());
        }
      }
    }
    else
    {
      Serial.printf("GET request failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end(); // Close connection
  }
  else
  {
    Serial.println("Not connected to Wi-Fi");
  }
}
