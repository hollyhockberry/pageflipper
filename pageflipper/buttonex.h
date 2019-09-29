#ifndef BUTTONEX_H__
#define BUTTONEX_H__

class Button;

class ButtonEx
{
    Button& _button;
    int _phase;
    uint32_t _lastPeriod, _beginTime;
public:
    void begin();
    int scan();

    static void update();
    bool isReadyForSleep() const;

    ButtonEx(Button& button);
private:
    ButtonEx(const ButtonEx&);
    void operator=(const ButtonEx&);
};

#endif //ifndef BUTTONEX_H__
