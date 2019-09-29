#ifndef PRESENTER_H__
#define PRESENTER_H__

class IPresenter
{
public:
    virtual void begin() = 0;
    virtual void update() = 0;

    virtual void enterStandby() = 0;
    virtual void enterAdvertising() = 0;
    virtual void enterKeyboard() = 0;
};

#endif //ifndef PRESENTER_H__
