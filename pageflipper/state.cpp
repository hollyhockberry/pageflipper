#include <M5StickC.h>
#include "state.h"
#include "hogpkeyboard.h"
#include "buttonex.h"
#include "HIDKeyboardTypes.h"

class State
{
public:
    virtual void enter() {}
    virtual void exit() {}
    virtual State* loop() = 0;
protected:
    State(){}
private:
    State(const State&);
    void operator=(const State&);
};

class StandbyState : public State
{
    static StandbyState* _instance;
public:
    static State* Instance();
    virtual void enter();
    virtual State* loop();
    StandbyState();
};

class AdvertisingState : public State
{
    static AdvertisingState* _instance;
    HoGPKeyboard& _keyboard;
public:
    static State* Instance();
    virtual void enter();
    virtual State* loop();
    AdvertisingState(HoGPKeyboard& keyboard);
};

class KeyboardState : public State
{
    static KeyboardState* _instance;
    HoGPKeyboard& _keyboard;
    ButtonEx& _btnForward;
    ButtonEx& _btnBackbard;
public:
    static State* Instance();
    virtual State* loop();
    KeyboardState(HoGPKeyboard& keyboard, ButtonEx& btnForward, ButtonEx& btnBackward);
};

StateContext::StateContext(HoGPKeyboard& keyboard, ButtonEx& btnForward, ButtonEx& btnBackward)
: _state(NULL),
  _keyboard(keyboard),
  _btnForward(btnForward), _btnBackbard(btnBackward)
{
}

void StateContext::begin()
{
    new StandbyState();
    new AdvertisingState(_keyboard);
    new KeyboardState(_keyboard, _btnForward, _btnBackbard);

    _btnForward.begin();
    _btnBackbard.begin();

    const esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason){
    case ESP_SLEEP_WAKEUP_EXT0:
    case ESP_SLEEP_WAKEUP_EXT1:
    case ESP_SLEEP_WAKEUP_TIMER:
    case ESP_SLEEP_WAKEUP_TOUCHPAD:
    case ESP_SLEEP_WAKEUP_ULP:
    case ESP_SLEEP_WAKEUP_GPIO:
    case ESP_SLEEP_WAKEUP_UART:
        _state = AdvertisingState::Instance();
        break;
    default:
        _state = StandbyState::Instance();
        break;
    }
    _state->enter();
}

void StateContext::loop()
{
    State* next = _state->loop();

    if (next != _state) {
        _state->exit();
        next->exit();
        _state = next;
    }
}

StandbyState::StandbyState()
{
    _instance = this;
}

StandbyState* StandbyState::_instance = NULL;

State* StandbyState::Instance()
{
    return _instance;
}

void StandbyState::enter()
{
    M5.Axp.ScreenBreath(0);
}

State* StandbyState::loop()
{
    // enter sleep
    pinMode(GPIO_NUM_37, INPUT_PULLUP);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_37, LOW);
    esp_deep_sleep_start();

    return this; // never reach
}

AdvertisingState::AdvertisingState(HoGPKeyboard& keyboard)
: _keyboard(keyboard)
{
    _instance = this;
}

AdvertisingState* AdvertisingState::_instance = NULL;

State* AdvertisingState::Instance()
{
    return _instance;
}

void AdvertisingState::enter()
{
    M5.Axp.ScreenBreath(0);
    _keyboard.begin();
}

State* AdvertisingState::loop()
{
    if (_keyboard.isConnected())
        return KeyboardState::Instance();
    return this;
}

KeyboardState::KeyboardState(HoGPKeyboard& keyboard, ButtonEx& btnForward, ButtonEx& btnBackward)
: _keyboard(keyboard), _btnForward(btnForward), _btnBackbard(btnBackward) {
    _instance = this;
}

KeyboardState* KeyboardState::_instance = NULL;

State* KeyboardState::Instance()
{
    return _instance;
}

State* KeyboardState::loop()
{
    if (!_keyboard.isConnected())
        return AdvertisingState::Instance();

    ButtonEx::update();

    switch (_btnForward.scan()) {
    case 1:
        _keyboard.sendKey(LEFT_ARROW);
        break;
    case 2:
        _keyboard.sendKey(RIGHT_ARROW);
        break;
    default:
        break;
    }

    if (_btnForward.isReadyForSleep() && _btnBackbard.isReadyForSleep())
        return StandbyState::Instance();

    return this;
}
