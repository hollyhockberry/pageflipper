#include <M5StickC.h>
#include "simplepresenter.h"

void SimplePresenter::begin()
{
    _led.begin();
}

void SimplePresenter::loop()
{
    _led.loop();
}

void SimplePresenter::enterStandby()
{
    M5.Axp.ScreenBreath(0);
}

void SimplePresenter::enterAdvertising()
{
    M5.Axp.ScreenBreath(12);
    _led.blink(1000);
}

void SimplePresenter::enterKeyboard()
{
    M5.Axp.ScreenBreath(0);
    _led.turnOff();
}
