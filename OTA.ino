// Initialize Esp8266 OTA

#include <ArduinoOTA.h>

#define OTA_PASS "13098ard"


void inicializarOTA() {
  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);
  // Hostname defaults  esp8266-[ChipID]
  ArduinoOTA.setHostname(HOST_NAME);
  // No authentication by default
  ArduinoOTA.setPassword((const char *)OTA_PASS);

  ArduinoOTA.onStart([]() {
    Serial.println("*OTA: Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\n*OTA: End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("*OTA: Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("*OTA: Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}



void chkOTA(){
    ArduinoOTA.handle();
}

