#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>
#include "html.h"

#define VERSION "1.1.1"

#define SSID "Motogymkhana39"
#define PWD  "Motogymkhana39" // WiFi password has to be at least 8 characters

#define CHANNEL 8

#define WEB_SERVER_IP 192, 168, 1, 1

#define TRIGGER_PIN D2

ESP8266WebServer server(80);

WebSocketsServer webSocket = WebSocketsServer(81);

int8_t webSocketNo = -1;
long threshold_ms = 3000;

volatile uint64_t timeRun   = 0;
volatile uint64_t timeISR   = 0;
         uint64_t timeFixed = 0;

bool is_start     = false;
bool LedState     = false;
bool LedStateSent = true;

uint64_t bestRun = 0xFFFFFFFFL;
int runN = 0;

/*IRAM_ATTR*/ ICACHE_RAM_ATTR void OnSignalInterrupt(){
  if (!digitalRead(TRIGGER_PIN)){
    timeISR = millis();
    LedState = false;
  }
  else
    LedState = true;
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length)
{
  switch(type) {
    case WStype_DISCONNECTED:{
      Serial.printf("[%u] Disconnected!\n", num);
      webSocketNo = -1;
    }break;

    case WStype_CONNECTED:{
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      webSocketNo = num;
      // send message to client
      webSocket.sendTXT(num, String("Thold ") + (threshold_ms / 1000));
      is_start = false;
    }break;

    case WStype_TEXT:{
      Serial.printf("[%u] get Text: %s\n", num, payload);
      if (!strcmp((const char*) payload, "Reset")){
        is_start = false;
        LedState = digitalRead(TRIGGER_PIN);
        LedStateSent = !LedState;
        bestRun = 0xFFFFFFFFL;
        runN = 0;
      }
      else if (!strncmp((const char*) payload, "Thold", 5)) threshold_ms = atoi((const char*)&payload[5]) * 1000;
    }break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  IPAddress apIP(WEB_SERVER_IP);  // server IP
  IPAddress subnet(255,255,255,0); // subnet mask

  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, subnet); // subnet FF FF FF 00  
  WiFi.softAP(SSID, PWD, CHANNEL);
  Serial.printf("\nStarted AccessPoint \"%s\" with IP: ", SSID);
  Serial.println(WiFi.softAPIP());

  // start Web server
  server.on(
      "/",
      [](){ server.send(200, "text/html", CHtml::getPage()); }
    );

  server.onNotFound(
      [](){ server.send(404, "text/plain", ""); }
    );

  server.begin();

  // start webSocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  pinMode(TRIGGER_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(TRIGGER_PIN), OnSignalInterrupt, CHANGE);

  LedState = digitalRead(TRIGGER_PIN);
  LedStateSent = !LedState;
}

void loop() {
  server.handleClient();
  webSocket.loop();
  triggerTick();
}

void triggerTick(){
  if (LedState != LedStateSent)
  {
    if (webSocketNo >= 0){
      webSocket.sendTXT(webSocketNo, String("LedState ") + (LedState ? "1" : "0"));
      LedStateSent = LedState;
    }
  }

  if (timeISR != timeFixed) {
    timeRun = (timeISR > timeFixed) ? timeISR - timeFixed : (0xFFFFFFFFL - timeFixed) + timeISR;

    if (timeRun < threshold_ms) // should be applied and at start, and at finish
      return;

    is_start = !is_start;

    String txt;
    if (is_start){
      runN++;

      uint32_t low = timeISR % 0xFFFFFFFF;
      uint32_t high = (timeISR >> 32) % 0xFFFFFFFF;
      txt = String("Start ") + runN + " " + low+high;

      if (webSocketNo >= 0) webSocket.sendTXT(webSocketNo, txt);
      Serial.println(txt);
    }
    else{
      uint32_t timeRun_part_m  = timeRun / 1000 / 60;
      uint32_t timeRun_part_s  = timeRun / 1000 % 60;
      uint32_t timeRun_part_ms = timeRun % 1000;

      txt = String("Finish ") + runN + " " + timeRun_part_m + ":";

      if (timeRun_part_s < 10) txt += "0";
      txt += timeRun_part_s;
      txt += ".";

      if (timeRun_part_ms < 100) txt += "0";
      if (timeRun_part_ms < 10)  txt += "0";
      txt += timeRun_part_ms;

      if (timeRun < bestRun) {
        bestRun = timeRun;
        txt += String(" record");
      }

      if (webSocketNo >= 0) webSocket.sendTXT(webSocketNo, txt);
      Serial.println(txt);
    }

    timeFixed = timeISR;
  }
}
