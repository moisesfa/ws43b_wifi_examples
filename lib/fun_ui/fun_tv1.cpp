#include "fun_tv1.h"
#include <Arduino.h>
#include "config_gfx_lvgl.h"
#include <WiFi.h>
#include <Preferences.h>


static lv_obj_t *kb;
lv_obj_t *ssid_text_area;
lv_obj_t *password_text_area;
lv_obj_t *dropdown_field;
lv_obj_t *tv_act;
lv_obj_t *text_label_conn;
lv_obj_t *button_retry;
lv_obj_t * spinner_scan;

Preferences preferences;
String wifi_ssid_list;
String wifi_ssid;
String wifi_password;
String text_connect;

// Tarea de escaneo WiFi que se ejecutará una vez
void scanWiFiTask(void *parameter)
{
  Serial.println("scan start");
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  wifi_ssid_list = "";
  Serial.println("scan done");

  if (n == 0)
  {
    wifi_ssid_list = wifi_ssid_list + "No networks found\n";
    Serial.println("No networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");

    for (int i = 0; i < n; ++i)
    {
      wifi_ssid_list = wifi_ssid_list + WiFi.SSID(i) + '\n';
    }
    Serial.println(wifi_ssid_list);
  }
  
  //* Prepara la estructura de mensaje 
  lvgl_update_t update_widget = {
    .widget_id = 1,
    .new_String = wifi_ssid_list
   };
   //* Envia el mensaje a la cola 
   xQueueSend(lvgl_queue, &update_widget, portMAX_DELAY);

  // Delete the scan result to free memory for code below.
  WiFi.scanDelete();

  Serial.println("finish scanWiFiTask");
  vTaskDelay(pdMS_TO_TICKS(100));

  // Elimina la tarea actual (self-delete)
  vTaskDelete(NULL);
}

// Tarea de conexión WiFi que se ejecutará una vez
void connectWiFiTask(void *parameter)
{

  // Try to connect to Wi-Fi
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());
  Serial.print("Connecting to WiFi ..");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i < 10)
  {
    Serial.print('.');
    vTaskDelay(pdMS_TO_TICKS(1000));
    i++;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Failed to connect");
    text_connect = "Failed to connect " LV_SYMBOL_CLOSE;
  }
  else
  {
    Serial.println("Connected to Wi-Fi");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
    String local_ip = "IP ADDRESS: " + WiFi.localIP().toString();
   
    text_connect = "Connected to Wi-Fi \n";
    text_connect += wifi_ssid.c_str();
    text_connect += "  " LV_SYMBOL_OK "\n";
    text_connect += local_ip + "\n";
    text_connect += "RSSI: " + String(WiFi.RSSI()) + "\n";
  
  }

  //* Prepara la estructura de mensaje 
  lvgl_update_t update_widget = {
    .widget_id = 2,
    .new_String = text_connect
   };
  //* Envia el mensaje a la cola 
  xQueueSend(lvgl_queue, &update_widget, portMAX_DELAY);

  Serial.println("finish connectWiFiTask");
  vTaskDelay(pdMS_TO_TICKS(100));
  // Elimina la tarea actual (self-delete)
  vTaskDelete(NULL);
}

static void text_area_event_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t *text_area = (lv_obj_t *)lv_event_get_target(e);
  if (code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED)
  {
    // Focus on the clicked text area
    lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
    if (kb != NULL)
      lv_keyboard_set_textarea(kb, text_area);
  }
  else if (code == LV_EVENT_READY)
  {
    // lv_obj_add_flag(kb,LV_OBJ_FLAG_HIDDEN);
    char buf[64];
    lv_dropdown_get_selected_str(dropdown_field, buf, sizeof(buf));
    LV_LOG_USER("SSID %s", buf);
    LV_LOG_USER("PASSWORD %s", lv_textarea_get_text(password_text_area));

    preferences.begin("credentials", false);

    preferences.putString("wifi_ssid", String(buf));
    wifi_ssid = preferences.getString("wifi_ssid", "");
    Serial.print("ssid: ");
    Serial.println(wifi_ssid);

    preferences.putString("wifi_password", String(lv_textarea_get_text(password_text_area)));
    wifi_password = preferences.getString("wifi_password", "");
    Serial.print("pass: ");
    Serial.println(wifi_password);

    preferences.end();
    lv_wifi_info_gui(tv_act);
  }
}

static void button_delete_credentials_cb(lv_event_t *e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED)
  {
    preferences.begin("credentials", false);
    preferences.clear();
    preferences.end();
    ESP.restart();
  }
}

void lv_wifi_info_gui(lv_obj_t *tv)
{

  // Try to connect to Wi-Fi
  xTaskCreate(
      connectWiFiTask, // Función que ejecuta la tarea
      "Connect WiFi",  // Nombre de la tarea
      1024 * 3,        // Tamaño de la pila en bytes
      NULL,            // Parámetro opcional para pasar a la función
      0,               // Prioridad de la tarea
      NULL             // Puntero opcional para identificar la tarea
  );

  // Clear screen
  lv_obj_clean(tv);

  // // Create a text label aligned center on top ("Wi-Fi Manager")
  lv_obj_t *text_label_0 = lv_label_create(tv);
  lv_obj_set_style_text_color(text_label_0, lv_color_hex(0x5e5e5c), LV_PART_MAIN);
  lv_label_set_text(text_label_0, LV_SYMBOL_WIFI " Wi-Fi Manager");
  lv_obj_set_style_text_align(text_label_0, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(text_label_0, LV_ALIGN_TOP_MID, 0, 20);

  static lv_style_t style_text_label;
  lv_style_init(&style_text_label);
  lv_style_set_text_font(&style_text_label, &lv_font_montserrat_28);
  lv_obj_add_style(text_label_0, &style_text_label, 0);

  text_label_conn = lv_label_create(tv);
  lv_label_set_long_mode(text_label_conn, LV_LABEL_LONG_WRAP);
  lv_obj_set_style_text_color(text_label_conn, lv_color_hex(0x5e5e5c), LV_PART_MAIN);
  text_connect = "Connecting . . . " LV_SYMBOL_CALL;
  lv_label_set_text(text_label_conn, text_connect.c_str());
  // lv_label_set_text(text_label_conn, "Connecting . . . " LV_SYMBOL_CALL);
  lv_obj_set_style_text_align(text_label_conn, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(text_label_conn, LV_ALIGN_CENTER, 0, -40);
  lv_obj_add_style(text_label_conn, &style_text_label, 0);

  button_retry = lv_button_create(tv);                                                 // Add a button the current screen
  lv_obj_set_size(button_retry, 120, 50);                                                  // Set its size
  lv_obj_add_event_cb(button_retry, button_delete_credentials_cb, LV_EVENT_CLICKED, NULL); // Assign a callback to the button
  lv_obj_align(button_retry, LV_ALIGN_BOTTOM_MID, 0, -20);
  lv_obj_add_state(button_retry, LV_STATE_DISABLED);
  lv_obj_t *button_label_retry = lv_label_create(button_retry); // Add a label to the button
  lv_label_set_text(button_label_retry, "RETRY");               // Set the labels text
  lv_obj_center(button_label_retry);
}

void create_ui_tv1(lv_obj_t *tv)
{
  tv_act = tv;
  // Task scan WiFi
  xTaskCreate(
      scanWiFiTask, // Función que ejecuta la tarea
      "Scan WiFi",  // Nombre de la tarea
      1024 * 3,     // Tamaño de la pila en bytes
      NULL,         // Parámetro opcional para pasar a la función
      0,            // Prioridad de la tarea
      NULL          // Puntero opcional para identificar la tarea
  );

  // Create a text label aligned center on top
  lv_obj_t *text_label = lv_label_create(tv);
  lv_label_set_text(text_label, LV_SYMBOL_WIFI " Wi-Fi Manager");
  lv_obj_set_style_text_align(text_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_set_style_text_color(text_label, lv_color_hex(0x5e5e5c), LV_PART_MAIN); //
  lv_obj_align(text_label, LV_ALIGN_TOP_MID, 0, 20);

  static lv_style_t style_text_label;
  lv_style_init(&style_text_label);
  lv_style_set_text_font(&style_text_label, &lv_font_montserrat_28);
  lv_obj_add_style(text_label, &style_text_label, 0);

  // lv_obj_t * oneline_label = lv_label_create(tv_act);
  // lv_label_set_text(oneline_label, "SSID");
  // lv_obj_align_to(oneline_label, ssid_text_area, LV_ALIGN_TOP_RIGHT, 0, 20);

  dropdown_field = lv_dropdown_create(tv);
  lv_dropdown_set_options_static(dropdown_field, wifi_ssid_list.c_str());
  lv_obj_set_width(dropdown_field, lv_pct(50));
  lv_obj_align(dropdown_field, LV_ALIGN_TOP_MID, 0, 80);

  static lv_style_t style_text_ssid_pass;
  lv_style_init(&style_text_ssid_pass);
  lv_style_set_text_font(&style_text_ssid_pass, &lv_font_montserrat_20);
  lv_obj_add_style(dropdown_field, &style_text_ssid_pass, 0);
  lv_obj_t *list = lv_dropdown_get_list(dropdown_field); // Get the list/
  lv_obj_add_style(list, &style_text_ssid_pass, 0);      // Add the styles to the list/}

  // Create a label and position it above the text box
  lv_obj_t *password_label = lv_label_create(tv);
  lv_obj_set_style_text_color(password_label, lv_color_hex(0x5e5e5c), LV_PART_MAIN); //
  lv_label_set_text(password_label, "PASSWORD");
  lv_obj_align_to(password_label, password_text_area, LV_ALIGN_TOP_MID, 0, 140);

  // Create the password box
  password_text_area = lv_textarea_create(tv);
  lv_textarea_set_text(password_text_area, "");
  lv_textarea_set_password_mode(password_text_area, true);
  lv_textarea_set_one_line(password_text_area, true);
  lv_obj_set_width(password_text_area, lv_pct(40));
  lv_obj_set_pos(password_text_area, 5, 70);
  lv_obj_add_event_cb(password_text_area, text_area_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_align(password_text_area, LV_ALIGN_TOP_MID, 0, 180);
  lv_obj_add_style(password_text_area, &style_text_ssid_pass, 0); // Add the styles to the list/}

   /*Create a spinner*/
  spinner_scan = lv_spinner_create(tv);
  lv_obj_set_size(spinner_scan, 100, 100);
  lv_obj_align(spinner_scan, LV_ALIGN_CENTER, 0, 80);
  lv_spinner_set_anim_params(spinner_scan, 1000, 200);

  // Create a keyboard
  kb = lv_keyboard_create(tv);
  lv_obj_set_size(kb, LV_HOR_RES, LV_VER_RES / 2);
  lv_keyboard_set_textarea(kb, password_text_area); // Focus it on one of the text areas to start
  lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
}

void get_preferences(void){
    // Open the credentials namespace and try to retrieve an SSID/Password
    preferences.begin("credentials", false);
    wifi_ssid = preferences.getString("wifi_ssid", ""); 
    wifi_password = preferences.getString("wifi_password", "");
    // Close the Preferences
    preferences.end();    
}