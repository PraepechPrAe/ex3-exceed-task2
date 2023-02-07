#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#define GREEN 33
#define YELLOW 25
#define RED 26
TaskHandle_t TaskA = NULL;
TaskHandle_t TaskB = NULL;

const String baseUrl = "https://exceed-hardware-stamp465.koyeb.app/leddelay";
int de;

void Connect_Wifi()
{
  const char *ssid = "Qwerty";
  const char *password = "12345678";
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("OK! IP=");
  Serial.println(WiFi.localIP());
  Serial.println("----------------------------------");
}

void GET_value()
{
  DynamicJsonDocument doc(1000);
  HTTPClient http;
  http.begin(baseUrl);
  int httpResponseCode = http.GET();
  if (httpResponseCode == 200)
  {
    String payload = http.getString();
    deserializeJson(doc, payload);
    de = (doc["value"]);
    Serial.println(de);
    Serial.println("Done");
  }
  else
  {
    Serial.print("Error ");
    Serial.println(httpResponseCode);
  }

  Serial.println("----------------------------------");
}

void GET_5S(void *param)
{
  while (1)
  {
    GET_value();
    vTaskDelay( 5000/ portTICK_PERIOD_MS);
  }
  
}

void LED(void *param)
{
  while (1)
  {
    digitalWrite(GREEN,1);
    digitalWrite(YELLOW,1);
    digitalWrite(RED,1);
    vTaskDelay((int)de / portTICK_PERIOD_MS);
    digitalWrite(GREEN,0);
    digitalWrite(YELLOW,0);
    digitalWrite(RED,0);
    vTaskDelay((int)de / portTICK_PERIOD_MS);
  }
  
}


void setup() {
  Serial.begin(115200);
  pinMode(GREEN, OUTPUT);
  digitalWrite(GREEN,0);
  pinMode(YELLOW, OUTPUT);
  digitalWrite(YELLOW,0);  
  pinMode(RED, OUTPUT);
  digitalWrite(RED,0);
  Connect_Wifi();
  xTaskCreatePinnedToCore(LED, "LED", 1000, NULL, 0, &TaskA, 0);
  xTaskCreatePinnedToCore(GET_5S, "GetDelay", 10000, NULL, 1, &TaskB, 1);
  }

void loop() {
  
}