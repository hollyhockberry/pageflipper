#include <M5StickC.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include "BLE2902.h"
#include "BLEHIDDevice.h"
#include "HIDTypes.h"
#include "HIDKeyboardTypes.h"

namespace {

BLEHIDDevice* _hid;
BLECharacteristic* _input;
BLECharacteristic* _output;
bool _connected = false;

class ServerCallbacks : public BLEServerCallbacks
{
public:
    void onConnect(BLEServer* pServer);
    void onDisconnect(BLEServer* pServer);
};


void ServerCallbacks::onConnect(BLEServer* pServer)
{
    _connected = true;
    BLE2902* desc = (BLE2902*)_input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(true);
}

void ServerCallbacks::onDisconnect(BLEServer* pServer)
{
    _connected = false;
    BLE2902* desc = (BLE2902*)_input->getDescriptorByUUID(BLEUUID((uint16_t)0x2902));
    desc->setNotifications(false);  
}

void taskentry(void*)
{
    BLEDevice::init("HID_TRIAL");

    BLEServer* server = BLEDevice::createServer();
    server->setCallbacks(new ServerCallbacks());

    _hid = new BLEHIDDevice(server);
    _input = _hid->inputReport(1);      // <-- input REPORTID from report map
    _output = _hid->outputReport(1);    // <-- output REPORTID from report map

    std::string name = "kmgnkhr";
    _hid->manufacturer()->setValue(name);

    _hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
    _hid->hidInfo(0x00,0x02);

    BLESecurity* security = new BLESecurity();
    //  security->setKeySize();
    security->setAuthenticationMode(ESP_LE_AUTH_BOND);

    const uint8_t report[] = {
        USAGE_PAGE(1),      0x01,                 // Usage Page (Generic Desktop)
        USAGE(1),           0x06,                 // Usage (Keyboard)
        COLLECTION(1),      0x01,                 // Collection (Application)
        USAGE_PAGE(1),      0x07,                 // Usage Page (Key Codes)
        USAGE_MINIMUM(1),   0xe0,
        USAGE_MAXIMUM(1),   0xe7,
        LOGICAL_MINIMUM(1), 0x00,
        LOGICAL_MAXIMUM(1), 0x01,
        REPORT_SIZE(1),     0x01,
        REPORT_COUNT(1),    0x08,
        HIDINPUT(1),        0x02,                 // Input (Data, Variable, Absolute)
    
        REPORT_COUNT(1),    0x01,
        REPORT_SIZE(1),     0x08,
        HIDINPUT(1),        0x01,                 // Input (Constant) reserved byte(1)
    
        REPORT_COUNT(1),    0x05,
        REPORT_SIZE(1),     0x01,
        USAGE_PAGE(1),      0x08,                 // Usage Page (Page# for LEDs)
        USAGE_MINIMUM(1),   0x01,
        USAGE_MAXIMUM(1),   0x05,
        HIDOUTPUT(1),       0x02,                 // Output (Data, Variable, Absolute), Led report
        REPORT_COUNT(1),    0x01,
        REPORT_SIZE(1),     0x03,
        HIDOUTPUT(1),       0x01,                 // Output (Data, Variable, Absolute), Led report padding
    
        REPORT_COUNT(1),    0x06,
        REPORT_SIZE(1),     0x08,
        LOGICAL_MINIMUM(1), 0x00,
        LOGICAL_MAXIMUM(1), 0x65,
        USAGE_PAGE(1),      0x07,                 // Usage Page (Key codes)
        USAGE_MINIMUM(1),   0x00,
        USAGE_MAXIMUM(1),   0x65,
        HIDINPUT(1),        0x00,                 // Input (Data, Array) Key array(6 bytes)
    
        USAGE(1),           0x05,                 // Usage (Vendor Defined)
        LOGICAL_MINIMUM(1), 0x00,
        LOGICAL_MAXIMUM(1), 0xFF, 0x00,
        REPORT_SIZE(1),     0x08,
        REPORT_COUNT(1),    0x02,
        FEATURE(1),         0x02,                 // Feature (Data, Variable, Absolute)
    
        END_COLLECTION(0)
    };

    _hid->reportMap((uint8_t*)report, sizeof(report));
    _hid->startServices();

    BLEAdvertising* advertising = server->getAdvertising();
    advertising->setAppearance(HID_KEYBOARD);
    advertising->addServiceUUID(_hid->hidService()->getUUID());
    advertising->start();
    _hid->setBatteryLevel(7);

    delay(portMAX_DELAY);
}

bool sendKey(uint8_t keycode)
{
    if (!_connected)
        return false;

    KEYMAP map = keymap[keycode];
    uint8_t msg[] = {map.modifier, 0x0, map.usage, 0x0,0x0,0x0,0x0,0x0};
    _input->setValue(msg, sizeof(msg));
    _input->notify();

    uint8_t msg1[] = {0x0, 0x0, 0x0, 0x0,0x0,0x0,0x0,0x0};
    _input->setValue(msg1, sizeof(msg1));
    _input->notify();
            
    return true;
}

} //namespace


void setup()
{
    M5.begin();
    xTaskCreate(taskentry, "server", 20000, NULL, 5, NULL);    
}

void loop()
{
    M5.update();

    if (M5.BtnA.wasPressed()) {
        ::sendKey(LEFT_ARROW);
    }

    if (M5.BtnB.wasPressed()) {
        ::sendKey(RIGHT_ARROW);
    }    

}