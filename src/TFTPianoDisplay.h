#ifndef TFTPianoDisplay_h
#define TFTPianoDisplay_h

#include "Arduino.h"
#include "View.h"

class TFTPianoDisplay
{
public:
    TFTPianoDisplay(View &tft, unsigned char octaves, unsigned char startOctave, unsigned char x, unsigned char y, unsigned char size = 6);

    void reset();
    
    void setPosition(unsigned char x, unsigned char y);
    void keyDown(unsigned char key);
    void keyUp(unsigned char key);
    void drawFullPiano();
    
    void drawPiano();
    bool displayNeedsUpdating();
    void setBaseOctave(uint8_t octave);

private:
    View &_tft;
    unsigned char _x, _y;
    unsigned char _height, _width;
    unsigned char _offsetKeyZero;
    unsigned int _octaves, _startOctave;

    bool _shouldUpdatePiano, _forceFullKeyboardRedraw  = true;
    unsigned char _keysWhichArePressed[22];
    unsigned char _oldkeysWhichArePressed[22];

    bool isKeyPressed(unsigned char key)  {
        if (key < _offsetKeyZero) return false;
        if (key >= _offsetKeyZero+(_octaves*12)) return false;

        unsigned char index = key - _offsetKeyZero;
        unsigned char byteNumberOfKey = index / 8;
        unsigned char bitNumberOfKey = index % 8;
        return bitRead( _keysWhichArePressed[byteNumberOfKey], bitNumberOfKey);
    }
    bool wasKeyPressed(unsigned char key) {
        if (key < _offsetKeyZero) return false;
        if (key >= _offsetKeyZero+(_octaves*12)) return false;

        unsigned char index = key - _offsetKeyZero;
        unsigned char byteNumberOfKey = index / 8;
        unsigned char bitNumberOfKey = index % 8;
        return bitRead( _oldkeysWhichArePressed[byteNumberOfKey], bitNumberOfKey);
    }
    void setWasKeyPressed(unsigned char key, bool value)  {
        if (key < _offsetKeyZero) return;
        if (key >= _offsetKeyZero+(_octaves*12)) return;

        unsigned char index = key - _offsetKeyZero;
        unsigned char byteNumberOfKey = index / 8;
        unsigned char bitNumberOfKey = index % 8;
        bitWrite( _oldkeysWhichArePressed[byteNumberOfKey], bitNumberOfKey, value);
    }
    //bool isAnyKeyPressed(unsigned char key);
    void setSizeFactor(int asizeFactor) {
        _sizeFactor = asizeFactor;
        _whiteKeyWidth = (35 * 10) / _sizeFactor;
        _blackKeyWidth1 = (21 * 10) / _sizeFactor;
        _blackKeyWidth2 = (20 * 10) / _sizeFactor;
        _two_thirds_key_height = (70 * 2 / 3) / _sizeFactor;
        _one_thirds_key_height = (70 * 1 / 3) / _sizeFactor;
        _key_offset_c = 0;
        _key_offset_c_sharp = (_blackKeyWidth1) / 10;
        _key_offset_c_sharp_end = (2 * _blackKeyWidth1) / 10;
        _key_offset_d = (_whiteKeyWidth) / 10;
        _key_offset_d_sharp = (_blackKeyWidth1 * 3) / 10;
        _key_offset_d_sharp_end = (4 * _blackKeyWidth1) / 10;
        _key_offset_e = (_whiteKeyWidth * 2) / 10;
        _key_offset_f = (_whiteKeyWidth * 3) / 10;
        _key_offset_f_sharp = _key_offset_f + (_blackKeyWidth2) / 10;
        _key_offset_f_sharp_end = _key_offset_f_sharp + (_blackKeyWidth2) / 10;
        _key_offset_g = (_whiteKeyWidth * 4) / 10;
        _key_offset_g_sharp = _key_offset_f + (_blackKeyWidth2 * 3) / 10;
        _key_offset_g_sharp_end = _key_offset_g_sharp + (_blackKeyWidth2) / 10;
        _key_offset_a = (_whiteKeyWidth * 5) / 10;
        _key_offset_a_sharp = _key_offset_f + (_blackKeyWidth2 * 5) / 10;
        _key_offset_a_sharp_end = _key_offset_a_sharp + (_blackKeyWidth2) / 10;
        _key_offset_b = (_whiteKeyWidth * 6) / 10;
        _key_offset_b_end = ((_whiteKeyWidth * 7) / 10) - 1;
    }

    int _sizeFactor;
    int _whiteKeyWidth;
    int _blackKeyWidth1;
    int _blackKeyWidth2;
    int _two_thirds_key_height;
    int _one_thirds_key_height;
    int _key_offset_c;
    int _key_offset_c_sharp;
    int _key_offset_c_sharp_end;
    int _key_offset_d;
    int _key_offset_d_sharp;
    int _key_offset_d_sharp_end;
    int _key_offset_e;
    int _key_offset_f;
    int _key_offset_f_sharp;
    int _key_offset_f_sharp_end;
    int _key_offset_g;
    int _key_offset_g_sharp;
    int _key_offset_g_sharp_end;
    int _key_offset_a;
    int _key_offset_a_sharp;
    int _key_offset_a_sharp_end;
    int _key_offset_b;
    int _key_offset_b_end;
};
#endif
