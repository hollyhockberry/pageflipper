#ifndef SIMPLEPRESENTER_H__
#define SIMPLEPRESENTER_H__

#include "presenter.h"
#include "led.h"

class SimplePresenter : public IPresenter
{
    LED _led;
    bool _isLowBatt;
    int _led_state;
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
};

#endif //ifndef SIMPLEPRESENTER_H__
