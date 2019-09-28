#ifndef HOGPKEYBOARD_H__
#define HOGPKEYBOARD_H__

class HoGPKeyboard
{
public:
    bool isConnected() const;
    void begin();
    bool sendKey(uint8_t keycode);
    HoGPKeyboard();
private:
    HoGPKeyboard(const HoGPKeyboard&);
    void operator=(const HoGPKeyboard&);
};

#endif //ifndef HOGPKEYBOARD_H__
