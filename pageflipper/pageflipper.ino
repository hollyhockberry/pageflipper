#include <M5StickC.h>

#include "hogpkeyboard.h"
#include "buttonex.h"
#include "state.h"
#include "simplepresenter.h"

namespace {

HoGPKeyboard keyboard;
ButtonEx BtnA(M5.BtnA);
ButtonEx BtnB(M5.BtnB);
SimplePresenter presenter;
StateContext state(keyboard, BtnB, BtnA, presenter);

} //namespace

void setup()
{
    M5.begin();
    state.begin();
}

void loop()
{
    state.loop();
}
