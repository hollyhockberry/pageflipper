#include <M5StickC.h>
#include "simplepresenter.h"

void SimplePresenter::begin()
{
    _led.begin();
    _isLowBatt = false;
    _led_state = -1;
}

void SimplePresenter::loop()
{
    const bool lowBatt = M5.Axp.GetWarningLeve() == 1;
    if (_isLowBatt != lowBatt) {
        _isLowBatt = lowBatt;
        if (_isLowBatt) {
            _led.turnOn();
        } else if (_led_state < 0) {
            _led.turnOff();
        } else if (_led_state == 0) {
            _led.turnOn();
        } else {
            _led.blink(static_cast<uint32_t>(_led_state));
        }
    }
    _led.loop();
}

void SimplePresenter::enterStandby()
{
    M5.Axp.ScreenBreath(0);
    turnOffLED();
}

void SimplePresenter::enterAdvertising()
{
    M5.Axp.ScreenBreath(12);
    blinkLED(1000);
}

void SimplePresenter::enterKeyboard()
{
    M5.Axp.ScreenBreath(0);
    turnOffLED();
}

void SimplePresenter::turnOnLED()
{
    _led_state = 0;
    _led.turnOn();
}

void SimplePresenter::turnOffLED()
{
    _led_state = -1;
    if (!_isLowBatt) _led.turnOff();
}

void SimplePresenter::blinkLED(uint32_t interval_ms)
{
    _led_state = static_cast<int>(interval_ms);
    if (!_isLowBatt) _led.blink(interval_ms);
}
