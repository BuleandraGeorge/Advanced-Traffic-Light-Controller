#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <string.h>
#include "ESP_DATA_STRUCTURE.h"
#include "ESP_UTILITY_FUNCTIONS.h"
const char* ssid = "THE ESP"; // Your chosen access point name
const char* password = "ENGD2104";
ESP8266WebServer server(80);
#include "VIEW_HANDLERS.h"
void setup() {

  WiFi.softAP(ssid, password);
  server.on("/", handleRoot);
  server.on("/style.css", handleStyle);
  server.on("/equal_priority", handleEqualP);
  server.on("/set1_priority", handleSET1);
  server.on("/set2_priority", handleSET2);
  server.on("/maintenance", handleMAINTENANCE);
  server.on("/barrier_crossing", handleBARRIER_CROSSING);
  server.on("/motor_racing", handleMOTOR_RACING);
  server.on("/ledoff", handleLED_OFF);
  server.on("/led_amber", handleLED_AMBER);
  server.on("/led_green", handleLED_GREEN);
  server.on("/led_blue", handleLED_BLUE);
  server.on("/led_red", handleLED_RED);
  server.on("/get_status", handleGET_STATUS);
  server.begin();
  strcpy(TEXT_FOR_TEMPLATE[0], "no-tilt");
  for (int i = 1; i < 9; i++)
  {
    strcpy(TEXT_FOR_TEMPLATE[i], "CALCULATING...");
  }
  //////////////////////////////////////
  leaveHigh(DEMAND);
  pinMode(GRANTED, INPUT_PULLUP);
  Wire.begin(D6, D7);
  Serial.begin(9600);
  ESP_STATE = IDDLE;
  COMMUNICATION_STATE = CHECK_COMMAND_REQUEST;
  COMMAND = MAINTENANCE;
  get_status = false;
  send_command = false;
  trigger = false;
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  server.handleClient();
  switch (ESP_STATE)
  {
    case IDDLE:
      leaveHigh(DEMAND);
      if (trigger) ESP_STATE = REQUESTING_ACCESS;
      else ESP_STATE = IDDLE;
      digitalWrite(LED_BUILTIN, HIGH); 
      break;
    case REQUESTING_ACCESS:
      pullLow(DEMAND);
      if (trigger)
      {
        if (!granted()) ESP_STATE = REQUESTING_ACCESS;
        else ESP_STATE = MASTER;
      }
      else ESP_STATE = IDDLE;
      break;
    case MASTER:
      pullLow(DEMAND);
      digitalWrite(LED_BUILTIN, LOW); 
      if (trigger) ESP_STATE = MASTER;
      else ESP_STATE = IDDLE;
      break;
      

  }
  Serial.println(ESP_STATE);
  switch (COMMUNICATION_STATE)
  {
    case CHECK_COMMAND_REQUEST:
      if (((long)(millis() - GET_STATUS_CLOCK)) > 650)
      {
        COMMUNICATION_STATE = WAIT_FOR_MASTER;
        get_status = true;
        trigger = true;
      }
      if (send_command == true)
      {
        COMMUNICATION_STATE = WAIT_FOR_MASTER;
        trigger = true;
      }
      break;
    case WAIT_FOR_MASTER:
      if (ESP_STATE == MASTER) COMMUNICATION_STATE = SEND_COMMAND;
      else COMMUNICATION_STATE = WAIT_FOR_MASTER;
      break;
    case SEND_COMMAND:
      if (send_command)
      {
        Wire.beginTransmission(8);
        Wire.write(COMMAND);
        Wire.endTransmission(false);
        Wire.requestFrom(8, 1, true);
        if (Wire.available() > 0)
        {
          ptrCOMMAND_STATUS = (byte*)&COMMAND_STATUS;
          for (int i = 0; i < sizeof(COMMAND_STATUS); i++)
          {
            ptrCOMMAND_STATUS[i] = Wire.read();
          }

        }
      }
      if (get_status)
      {
        Wire.beginTransmission(8);
        Wire.write(GET_STATUS);
        Wire.endTransmission(false);
        Wire.requestFrom(8, 11, true);
        byte* ptr = (byte*)&STATUS_DATA_RESPONSE;

        if (Wire.available() > 0)
        {
          for (int i = 0; i < sizeof(STATUS_DATA_RESPONSE); i++)
          {
            unsigned char ch = Wire.read();
            ptr[i] = ch;
          }

          unsigned char* tPptr = (unsigned char*)&timePast;
          for (int i = 0; i < sizeof(timePast); i++)
          {
            tPptr[i] = STATUS_DATA_RESPONSE[7 + i];
          }
        }
      }

      trigger = false;
      get_status = false;
      send_command = false;
      GET_STATUS_CLOCK = millis();
      COMMUNICATION_STATE = CHECK_COMMAND_REQUEST;
      break;
  }


}
