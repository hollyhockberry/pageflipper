#include <M5StickC.h>
#include "led.h"

namespace {
const uint32_t TIKER_INTERVAL_MS = 10;
} //namespace

LED::LED()
{
}

void LED::begin()
{
    pinMode(10, OUTPUT);
    digitalWrite(10, LOW);
    _led.interval = 0;
    _led.on = false;
}

void LED::loop()
{
    update();
}

void LED::update()
{
    const int tick = (::millis() - _led.last) / TIKER_INTERVAL_MS;
    if (tick <= 0) return;

    _led.last = ::millis();

    const bool on = _led.on;
    if (_led.interval == 0) {
        _led.on = true;
    } else if (_led.interval < 0) {
        _led.on = false;
    } else {
        _led.counter -= tick;
        if (_led.counter < -_led.interval)
            _led.counter = _led.interval;
        _led.on = _led.counter > 0;
    }

    if (_led.on == on) return;

    if (_led.on) {
        digitalWrite(10, LOW);
    } else {
        digitalWrite(10, HIGH);
    }
}

void LED::turnOn()
{
    _led.interval = 0;
}

void LED::turnOff()
{
    _led.interval = -1;
}

void LED::blink(uint32_t interval_ms)
{
    _led.on = false;
    _led.interval = _led.counter = interval_ms / 2 / TIKER_INTERVAL_MS;
}
