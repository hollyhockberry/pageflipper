#ifndef SIMPLEPRESENTER_H__
#define SIMPLEPRESENTER_H__

#include "presenter.h"
#include "led.h"

class SimplePresenter : public IPresenter
{
    LED _led;
    bool _isLowBatt;
    int _led_state;

    bool _isShowBatteryLevel;
    double _vBat;
    uint32_t _lastUpdateBatteryLevel;
public:
    virtual void begin();
    virtual void loop();

    virtual void enterStandby();
    virtual void enterAdvertising();
    virtual void enterKeyboard();
private:
    void turnOnLED();
    void turnOffLED();
    void blinkLED(uint32_t interval_ms);
    void drawBatteryLevel(bool force = false);
};

#endif //ifndef SIMPLEPRESENTER_H__
