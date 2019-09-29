#ifndef LED_H__
#define LED_H__

class LED
{
    struct led_t {
        bool on;
        int interval;
        int counter;
        uint32_t last;
    } _led;
public:
    void begin();
    void loop();
    void turnOn();
    void turnOff();
    void blink(uint32_t interval_ms);
    LED();
private:
    void update();
    LED(const LED&);
    void operator=(const LED&);
};



#endif //ifndef LED_H__
