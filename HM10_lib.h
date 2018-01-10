#include "Arduino.h"
#include "SoftwareSerial.h"

#ifndef HM10_LIB_H
#define HM10_LIB_H

/*
 * pin configuration
 */
#define TxD         D6
#define RxD         D5
#define lockButton  D2

/*
 * bluetooth HM10 command
 */
#define AT_COMMAND               "AT"
#define AT_COMMAND_ROLE_MASTER   "AT+ROLE1"
#define AT_COMMAND_DISI          "AT+DISI?"
#define AT_COMMAND_DISA          "AT+DISA?"
#define AT_COMMAND_IMMEI1        "AT+IMME1"
#define AT_COMMAND_RESET         "AT+RESET"

/*
 * mainState of bluekey
 */
enum mainState{
    MAIN_STATE_INIT = 1,
    MAIN_STATE_BLE_SCAN = 2,
    MAIN_STATE_PARSE_DATA = 3,
    MAIN_STATE_CREATE_JSON = 4,
    MAIN_STATE_POST_DATA = 5
};

/*
 * scanningState of bluekey
 */
enum scanningState{
    SCAN_STATE_INIT = 1,
    SCAN_STATE_SEND_COMMAND = 2,
    SCAN_STATE_GET_DATA = 3
};

/*
 * define state bluekey for scanning
 */
#define BLE_DEVICE_SCAN_DETECTED    0
#define BLE_DEVICE_FIRST_APPEAR     1
#define BLE_DEVICE_DOOR_LOCK        2

class BLE_HM10
{
    public:
        BLE_HM10(SoftwareSerial *softwareserial);
        void begin();
        void resetBluetooth();
        void sendBluetoothScan();
        void getData();
        void filterData();
        void convertCharData();
        void updateData();
        bool filterAuthBLE();
        void timeInterval();
        void setLockTime(unsigned long bleTimeLock, uint8_t statusDoorLock);
    private:
        SoftwareSerial *m_uart;
        uint8_t scanningState;
        bool newData = false;
        bool endData_f = false;
        const static unsigned long interval = 1000;
        unsigned long lastRefreshTime = 0;
        unsigned long last_lock;
        unsigned long previous_time;
        float lastRSSI;
        float drssi;
};

#endif