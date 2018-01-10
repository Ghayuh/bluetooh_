#include "Arduino.h"
#include "ESP8266WiFi.h"
#include "ESP8266WiFiMulti.h"
#include "ESP8266HTTPClient.h"
#include "ArduinoJson.h"

#define HTTP_Serial Serial

/*
 * USSD and PASS set
 */
#define USSD "Trojan"
#define PASS "99g4FeyTaKb1P$A"

/*
 * domain get, post and some configuration about server
 */
#define domain_get "http://5-dot-dev-inno-cloud.appspot.com/blukey/test_get"
#define domain_post "http://5-dot-dev-inno-cloud.appspot.com/blukey/test_post"
#define domain_log "http://5-dot-dev-inno-cloud.appspot.com/blukey/logs"
#define fingerprintkey "7D B0 D8 9D 60 C2 8E A5 97 77 F5 59 63 61 A6 72 EF 9E 28 52"

class BLE_HTTP{
    public:
        void begin();
        void createJSON();
        void postData();
    private:
};