#include "HM10_lib.h"
#include "HTTP_lib.h"

/*
* Software Serial configuration
*/
SoftwareSerial bluekeySerial(RxD, TxD);
BLE_HM10 bluekey(&bluekeySerial);
BLE_HTTP bluecon;

/*
* extern variable
*/
extern bool parseOK;
extern float dtime;
extern String uuid;

/*
* variable on this loop
*/
uint8_t mainState;
uint8_t buttonTogle;
unsigned long timeLock;
const static unsigned long interval2 = 1000;
unsigned long lastRefreshTime2 = 0;

void setup(){
    Serial.begin(115200);
    bluekey.begin();

    /*
     * intterupt button for Lock Button
     */
    pinMode(lockButton, INPUT);
    attachInterrupt(digitalPinToInterrupt(lockButton), interruptButton, RISING);

    mainState = MAIN_STATE_INIT;
}

void loop(){
    switch(mainState){
        case MAIN_STATE_INIT:
            mainState = MAIN_STATE_BLE_SCAN;
            break;
        case MAIN_STATE_BLE_SCAN:
                bluekey.updateData();
                mainState = MAIN_STATE_PARSE_DATA;
            break;
        case MAIN_STATE_PARSE_DATA:
                // bluekey.convertCharData();
                bluekey.timeInterval();
                
                Serial.print("uuid : ");Serial.println(uuid);
                Serial.print("dtime : ");Serial.println(dtime);

                mainState = MAIN_STATE_INIT;
            break;
        case MAIN_STATE_CREATE_JSON:
                bluecon.createJSON();
                mainState = MAIN_STATE_POST_DATA;
            break;
        case MAIN_STATE_POST_DATA:
                if( millis() - lastRefreshTime2 >= interval2 )
                {
                    lastRefreshTime2 += interval2;
                    bluecon.postData();
                }
                mainState = MAIN_STATE_INIT;
            break;
        default:
            break;
    }
}

void interruptButton(){
    timeLock = millis();
    if( buttonTogle == 0){
        buttonTogle = 1;
    }
    else{
        buttonTogle = 0;
    }
    bluekey.setLockTime(timeLock, buttonTogle);
}