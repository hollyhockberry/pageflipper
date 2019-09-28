#include <M5StickC.h>
#include "buttonex.h"

enum PHASE
{
    IDLE,
    WAIT_RELEASE,
    WAIT_PRESS,
};

ButtonEx::ButtonEx(Button& button)
: _button(button), _phase(IDLE), _lastPeriod(0)
{
}

void ButtonEx::begin()
{
    _phase = IDLE;
    _lastPeriod = ::millis();
}

int ButtonEx::scan()
{
    const int now = ::millis();
    const int delta = now - _lastPeriod;

    int clicks = 0;
    int next = _phase;

    switch (_phase) {
    case IDLE:
        if (_button.wasPressed()) {
            next = WAIT_RELEASE;
        }
        break;
    case WAIT_RELEASE:
        if (_button.wasReleased()) {
            next = WAIT_PRESS;
        } else if (delta > 150) {
            clicks = 1;
        }
        break;
    case WAIT_PRESS:
        if (_button.wasPressed()) {
            clicks = 2;
        } else if (delta > 200) {
            clicks = 1;
        }
        break;
    }

    if (clicks > 0)
        next = IDLE;

    if (next != _phase) {
        _phase = next;
        _lastPeriod = now;
    }
    return clicks;
}
