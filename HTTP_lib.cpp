#include "HTTP_lib.h"
#include "HM10_lib.h"
#include "SoftwareSerial.h"

ESP8266WiFiMulti bleConnection;

extern String uuid, major, minor, rssi, mac_address;
// extern uint8_t bleDeviceStatus;

StaticJsonBuffer<300> JSONBuffer;
JsonArray& list_data = JSONBuffer.createArray();
char JSONMessageBuffer[300];


void BLE_HTTP::begin(){
    bleConnection.addAP(USSD, PASS);
}

void BLE_HTTP::createJSON(){
    JsonObject& JSONencoder = JSONBuffer.createObject();
    
    JSONencoder["mac"]  = mac_address; //string, BLE mac address
    JSONencoder["uuid"] = uuid; //string BLE UUID
    JSONencoder["maj"]  = major; //integer, BLE major version
    JSONencoder["min"]  = minor; //integer, BLE minor version
    JSONencoder["rssi"] = rssi;
    // JSONencoder["drssi"] = drssi;
    // JSONencoder["dtime"] = dtime;    
    // JSONencoder["last_open"] = sendLock;//_last_lock; //elapse time since the lock open
    // JSONencoder["state"] = bleDeviceStatus; //state of blukey.

    list_data.add(JSONencoder);
}

void BLE_HTTP::postData(){
    HTTPClient http;

    http.begin(domain_log);
    http.addHeader("Content-Type", "application/json");

    list_data.prettyPrintTo(JSONMessageBuffer, sizeof(JSONMessageBuffer));
    Serial.println(JSONMessageBuffer);
    
    int httpCode = http.POST(JSONMessageBuffer);
    Serial.println(httpCode);
    http.end();
}
