#include <M5StickC.h>

#include "hogpkeyboard.h"
#include "buttonex.h"
#include "HIDKeyboardTypes.h"

namespace {

HoGPKeyboard keyboard;
ButtonEx BtnA(M5.BtnA);
ButtonEx BtnB(M5.BtnB);

} //namespace

void setup()
{
    M5.begin();

    keyboard.begin();
    BtnA.begin();
    BtnB.begin();
}

void loop()
{
    M5.update();

    switch (BtnA.scan()) {
    case 1:
        keyboard.sendKey(LEFT_ARROW);
        break;
    case 2:
        keyboard.sendKey(RIGHT_ARROW);
        break;
    default:
        break;
    }

}