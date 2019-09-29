#ifndef STATE_H__
#define STATE_H__

class State;
class HoGPKeyboard;
class ButtonEx;
class IPresenter;

class StateContext
{
    State* _state;
    HoGPKeyboard& _keyboard;
    ButtonEx& _btnForward;
    ButtonEx& _btnBackbard;
    IPresenter& _presenter;
public:
    void begin();
    void loop();

    StateContext(HoGPKeyboard& keyboard, ButtonEx& btnForward, ButtonEx& btnBackward,
                 IPresenter& presenter);
private:
    StateContext(const StateContext&);
    void operator=(const StateContext&);
};

#endif //ifndef STATE_H__
