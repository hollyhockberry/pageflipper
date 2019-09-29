#ifndef SIMPLEPRESENTER_H__
#define SIMPLEPRESENTER_H__

#include "presenter.h"

class SimplePresenter : public IPresenter
{
public:
    virtual void begin();
    virtual void update();

    virtual void enterStandby();
    virtual void enterAdvertising();
    virtual void enterKeyboard();
};

#endif //ifndef SIMPLEPRESENTER_H__
