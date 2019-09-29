#include <M5StickC.h>
#include "simplepresenter.h"

void SimplePresenter::begin()
{
}

void SimplePresenter::update()
{
}

void SimplePresenter::enterStandby()
{
    M5.Axp.ScreenBreath(0);
}

void SimplePresenter::enterAdvertising()
{
    M5.Axp.ScreenBreath(12);
}

void SimplePresenter::enterKeyboard()
{
    M5.Axp.ScreenBreath(0);
}
