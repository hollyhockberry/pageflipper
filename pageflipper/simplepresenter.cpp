#include <M5StickC.h>
#include "simplepresenter.h"

void SimplePresenter::begin()
{
    _led.begin();
    _isLowBatt = false;
    _led_state = -1;

    M5.Lcd.setRotation(1);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextFont(1);
}

void SimplePresenter::loop()
{
    const bool lowBatt = M5.Axp.GetWarningLeve() == 1;
    if (_isLowBatt != lowBatt) {
        _isLowBatt = lowBatt;
        if (_isLowBatt) {
            _led.turnOn();
        } else if (_led_state < 0) {
            _led.turnOff();
        } else if (_led_state == 0) {
            _led.turnOn();
        } else {
            _led.blink(static_cast<uint32_t>(_led_state));
        }
    }
    _led.loop();

    drawBatteryLevel();
}

void SimplePresenter::drawBatteryLevel(bool force)
{
    const double v = M5.Axp.GetVbatData() * 1.1 / 1000;
    const uint32_t now = ::millis();

    if (!force) {
        if (!_isShowBatteryLevel) return;
        if ((now - _lastUpdateBatteryLevel) < 500) return;
        if (_vBat == v ) return;
    }

    _vBat = v;
    _lastUpdateBatteryLevel = now;
    M5.Lcd.setCursor(6, 2);

    int p = (int)((_vBat - 3.0) * 100.0 / (4.2 - 3.0));
    M5.Lcd.printf("%3d%%", p);
}

void SimplePresenter::enterStandby()
{
    _isShowBatteryLevel = false;
    M5.Axp.ScreenBreath(0);
    turnOffLED();
}

void drawCenteringString(const char* str)
{
    M5.Lcd.setCursor(
        (M5.Lcd.width() - M5.Lcd.textWidth(str, 1)) / 2,
        (M5.Lcd.height() - M5.Lcd.fontHeight(1)) / 2);
    M5.Lcd.printf(str);
}

void SimplePresenter::enterAdvertising()
{
    M5.Axp.ScreenBreath(12);
    M5.Lcd.fillScreen(WHITE);
    M5.Lcd.setTextColor(RED, WHITE);
    drawCenteringString("DISCONNECTED");

    drawBatteryLevel(true);
    M5.Lcd.setTextColor(BLACK, WHITE);
    drawBatteryLevel(true);

    blinkLED(1000);
}

void SimplePresenter::enterKeyboard()
{
    M5.Axp.ScreenBreath(10);
    M5.Lcd.fillScreen(BLACK);

    M5.Lcd.setTextColor(WHITE, BLACK);
    drawCenteringString("CONNECTED");

    _isShowBatteryLevel = true;
    drawBatteryLevel(true);

    turnOffLED();
}

void SimplePresenter::turnOnLED()
{
    _led_state = 0;
    _led.turnOn();
}

void SimplePresenter::turnOffLED()
{
    _led_state = -1;
    if (!_isLowBatt) _led.turnOff();
}

void SimplePresenter::blinkLED(uint32_t interval_ms)
{
    _led_state = static_cast<int>(interval_ms);
    if (!_isLowBatt) _led.blink(interval_ms);
}
