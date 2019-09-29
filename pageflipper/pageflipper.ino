#include <M5StickC.h>

#include "hogpkeyboard.h"
#include "buttonex.h"
#include "state.h"

namespace {

HoGPKeyboard keyboard;
ButtonEx BtnA(M5.BtnA);
ButtonEx BtnB(M5.BtnB);
StateContext state(keyboard, BtnB, BtnA);

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
