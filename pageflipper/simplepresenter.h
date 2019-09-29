#ifndef SIMPLEPRESENTER_H__
#define SIMPLEPRESENTER_H__

#include "presenter.h"
#include "led.h"

class SimplePresenter : public IPresenter
{
    LED _led;
public:
    virtual void begin();
    virtual void loop();

    virtual void enterStandby();
    virtual void enterAdvertising();
    virtual void enterKeyboard();
};

#endif //ifndef SIMPLEPRESENTER_H__
