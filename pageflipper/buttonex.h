#ifndef BUTTONEX_H__
#define BUTTONEX_H__

class Button;

class ButtonEx
{
    Button& _button;
    int _phase;
    int _lastPeriod;
public:
    void begin();
    int scan();

    ButtonEx(Button& button);
private:
    ButtonEx(const ButtonEx&);
    void operator=(const ButtonEx&);
};

#endif //ifndef BUTTONEX_H__
