#include "HM10_lib.h"

const byte numChars = 100;
char receivedData[numChars];
String bleStringData;
String uuid, major, minor, mac_address, rssi;
unsigned long sendTimeLock;
uint8_t bleDeviceStatus;
bool parseOK;
unsigned long currentTime;
float dtime;

BLE_HM10::BLE_HM10(SoftwareSerial *softwareserial){
    m_uart = softwareserial;
}

void BLE_HM10::begin(){
    m_uart->begin(115200);
    resetBluetooth();
    scanningState = SCAN_STATE_INIT;
}

void BLE_HM10::resetBluetooth(){
    m_uart->print(AT_COMMAND_RESET);
    delay(200);
}

void BLE_HM10::sendBluetoothScan(){
    m_uart->print(AT_COMMAND_DISI);
    endData_f = false;
}

void BLE_HM10::getData(){
    if(m_uart->available()){
        filterData();
    }
    else if((m_uart->available() == 0) && endData_f){
        endData_f = false;
        scanningState = SCAN_STATE_INIT;
    }
}

void BLE_HM10::filterData(){
    static bool recvInProgress = false;
    static uint8_t index = 0;

    char startSeparator = ':';
    char endSeparator = 'O';
    char endOfData = 'E';
    char characterRecv;

    while ((m_uart->available() > 0) && (newData == false)){

        characterRecv = m_uart->read();

        if(recvInProgress == true){
            if(characterRecv != endSeparator){
                receivedData[index] = characterRecv;
                index++;
                if(index >= numChars){
                    index = numChars - 1;
                }
            }
            else{
                receivedData[index] = '\0';
                recvInProgress = false;
                index = 0;
                newData = true;
            }
        }
        else if(characterRecv == startSeparator){
            recvInProgress = true;
        }
        else if(characterRecv == endOfData){
            endData_f = true;
        }
    }
}

void BLE_HM10::convertCharData(){
    if(newData == true){
        bleStringData = String(receivedData);

        currentTime = millis();

        if( bleStringData.length() >= 70 ){
            uuid = bleStringData.substring(9,41);
            major = bleStringData.substring(42,46);
            minor = bleStringData.substring(46,50);
            mac_address = bleStringData.substring(53,65);
            rssi = bleStringData.substring(66,70);
        }

        drssi = (rssi.toFloat() - lastRSSI);
        dtime = (currentTime - previous_time);

        newData = false;
    }

    lastRSSI = rssi.toFloat();
    previous_time = currentTime;
    bleDeviceStatus = BLE_DEVICE_SCAN_DETECTED;
}

bool BLE_HM10::filterAuthBLE()
{
    bool filterAuthBLEreturn;
    if( uuid != "0000000000000000" && mac_address == "0CF3EE0D9D9E"){
        //go to create JSON
        filterAuthBLEreturn = true;
    }
    else{
        //back to scan BLE again
        filterAuthBLEreturn = false;
    }
    return filterAuthBLEreturn;
}

void BLE_HM10::updateData(){
    switch(scanningState){
        case SCAN_STATE_INIT:
                scanningState = SCAN_STATE_SEND_COMMAND;
            break;
        case SCAN_STATE_SEND_COMMAND:
                sendBluetoothScan();
                scanningState = SCAN_STATE_GET_DATA;
            break;
        case SCAN_STATE_GET_DATA:
                getData();
            break;
        default:
            break;
    }
}

void BLE_HM10::timeInterval(){
    if( millis() - lastRefreshTime >= interval){
        lastRefreshTime += interval;
        convertCharData();
    }
}

void BLE_HM10::setLockTime(unsigned long bleTimeLock, uint8_t statusDoorLock){
    sendTimeLock = bleTimeLock - last_lock;

    last_lock = bleTimeLock;

    if( statusDoorLock != 0){
        bleDeviceStatus = BLE_DEVICE_DOOR_LOCK;
    }
    else{
        bleDeviceStatus = BLE_DEVICE_SCAN_DETECTED;
    }
}