#ifndef TFTPianoDisplay_h
#define TFTPianoDisplay_h

#include "Arduino.h"
#include <ST7735_t3.h>    // Core graphics library

class TFTPianoDisplay
{
public:
    TFTPianoDisplay(ST7735_t3 &tft, byte octaves, byte startOctave, byte x, byte y) :
            TFTPianoDisplay(tft, octaves, startOctave, x, y, 6) {}

    TFTPianoDisplay(ST7735_t3 &tft, byte octaves, byte startOctave, byte x, byte y, byte asize_factor);
    void setPosition(byte x, byte y);
    void keyDown(byte key);
    void keyUp(byte key);
    void drawPiano();
    bool displayNeedsUpdating();

private:
    ST7735_t3 *_tft;
    byte _x, _y;
    byte _height, _width;
    byte _offsetKeyZero;
    unsigned int _octaves, _startOctave;

    bool _shouldUpdatePiano, _forceFullKeyboardRedraw  = true;
    byte _keysWhichArePressed[22];
    byte _oldkeysWhichArePressed[22];

    bool isKeyPressed(byte key);
    bool wasKeyPressed(byte key);
    void setWasKeyPressed(byte key, bool value);
    bool isAnyKeyPressed(byte key);

    void setSizeFactor(int asizeFactor);

    int _sizeFactor;
    int _whiteKeyWidth;
    int _blackKeyWidth1;
    int _blackKeyWidth2;
    int _two_thirds_key_height ;
    int _one_thirds_key_height;
    int _key_offset_c;
    int _key_offset_c_sharp;
    int _key_offset_c_sharp_end ;
    int _key_offset_d;
    int _key_offset_d_sharp;
    int _key_offset_d_sharp_end;
    int _key_offset_e;
    int _key_offset_f;
    int _key_offset_f_sharp;
    int _key_offset_f_sharp_end;
    int _key_offset_g ;
    int _key_offset_g_sharp;
    int _key_offset_g_sharp_end ;
    int _key_offset_a;
    int _key_offset_a_sharp;
    int _key_offset_a_sharp_end;
    int _key_offset_b;
    int _key_offset_b_end;
};

#endif
