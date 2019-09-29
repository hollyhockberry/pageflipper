#include <M5StickC.h>
#include "state.h"
#include "hogpkeyboard.h"
#include "buttonex.h"
#include "HIDKeyboardTypes.h"
#include "presenter.h"

class State
{
    IPresenter& _presenter;
public:
    void enter() {
        doEnter();
    }
    void exit() {
        doExit();
    }
    State* loop() {
        _presenter.loop();
        return doLoop();
    }
protected:
    virtual void doEnter() {}
    virtual void doExit() {}
    virtual State* doLoop() = 0;

    IPresenter& Presenter() {
        return _presenter;
    }
    State(IPresenter& presenter) : _presenter(presenter) {}
private:
    State(const State&);
    void operator=(const State&);
};

class StandbyState : public State
{
    static StandbyState* _instance;
public:
    static State* Instance();
    StandbyState(IPresenter& presenter);
protected:
    virtual void doEnter();
    virtual State* doLoop();
};

class AdvertisingState : public State
{
    static AdvertisingState* _instance;
    HoGPKeyboard& _keyboard;
public:
    static State* Instance();
    AdvertisingState(IPresenter& presenter, HoGPKeyboard& keyboard);
protected:
    virtual void doEnter();
    virtual State* doLoop();
};

class KeyboardState : public State
{
    static KeyboardState* _instance;
    HoGPKeyboard& _keyboard;
    ButtonEx& _btnForward;
    ButtonEx& _btnBackbard;
    bool _isShow;
    uint32_t _offTimer;
public:
    static State* Instance();
    KeyboardState(IPresenter& presenter, HoGPKeyboard& keyboard, ButtonEx& btnForward, ButtonEx& btnBackward);
protected:
    virtual void doEnter();
    virtual State* doLoop();
private:
    void checkLcdTimer();    
};

StateContext::StateContext(HoGPKeyboard& keyboard,
                           ButtonEx& btnForward, ButtonEx& btnBackward,
                           IPresenter& presenter)
: _state(NULL),
  _keyboard(keyboard),
  _btnForward(btnForward), _btnBackbard(btnBackward),
  _presenter(presenter)
{
}

void StateContext::begin()
{
    new StandbyState(_presenter);
    new AdvertisingState(_presenter, _keyboard);
    new KeyboardState(_presenter, _keyboard, _btnForward, _btnBackbard);

    _btnForward.begin();
    _btnBackbard.begin();
    _presenter.begin();

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
    ButtonEx::update();

    State* next = NULL;
    if (_btnForward.isReadyForSleep() && _btnBackbard.isReadyForSleep())
        next = StandbyState::Instance();
    else
        next = _state->loop();

    if (next != _state) {
        _state->exit();
        next->enter();
        _state = next;
    }
}

StandbyState::StandbyState(IPresenter& presenter)
: State(presenter)
{
    _instance = this;
}

StandbyState* StandbyState::_instance = NULL;

State* StandbyState::Instance()
{
    return _instance;
}

void StandbyState::doEnter()
{
    Presenter().enterStandby();
}

State* StandbyState::doLoop()
{
    // enter sleep
    pinMode(GPIO_NUM_37, INPUT_PULLUP);
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_37, LOW);
    esp_deep_sleep_start();

    return this; // never reach
}

AdvertisingState::AdvertisingState(IPresenter& presenter, HoGPKeyboard& keyboard)
: State(presenter), _keyboard(keyboard)
{
    _instance = this;
}

AdvertisingState* AdvertisingState::_instance = NULL;

State* AdvertisingState::Instance()
{
    return _instance;
}

void AdvertisingState::doEnter()
{
    Presenter().enterAdvertising();
    _keyboard.begin();
}

State* AdvertisingState::doLoop()
{
    if (_keyboard.isConnected())
        return KeyboardState::Instance();
    return this;
}

KeyboardState::KeyboardState(IPresenter& presenter, HoGPKeyboard& keyboard, ButtonEx& btnForward, ButtonEx& btnBackward)
: State(presenter), _keyboard(keyboard), _btnForward(btnForward), _btnBackbard(btnBackward) {
    _instance = this;
}

KeyboardState* KeyboardState::_instance = NULL;

State* KeyboardState::Instance()
{
    return _instance;
}

namespace {
const uint32_t LCD_OFF_INTERVAL_MS = 10000;
} //namespace

void KeyboardState::doEnter()
{
    Presenter().enterKeyboard();
    _isShow = true;
    _offTimer = ::millis() + LCD_OFF_INTERVAL_MS;
}

State* KeyboardState::doLoop()
{
    if (!_keyboard.isConnected())
        return AdvertisingState::Instance();

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

    checkLcdTimer();

    return this;
}

void KeyboardState::checkLcdTimer()
{
    const uint32_t now = ::millis();
    if (_btnBackbard.scan() > 0) {
        _offTimer = _isShow ? now : (now + LCD_OFF_INTERVAL_MS);
    }
    const bool show = _offTimer > now;
    if (show != _isShow) {
        _isShow = show;
        Presenter().showLCD(_isShow);
    }
}
