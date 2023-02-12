#ifndef TFTPianoDisplay_h
#define TFTPianoDisplay_h

#include "Arduino.h"

template <typename TDisplay>
class TFTPianoDisplay
{
public:
    TFTPianoDisplay(TDisplay &tft, unsigned char octaves, unsigned char startOctave, unsigned char x, unsigned char y)  : _tft(tft){
        _x = x;
        _y = y;
        _height = 32;
        _width = 128;
        _octaves = octaves;
        _startOctave = startOctave;
        _offsetKeyZero = ( 12 * _startOctave );
        _shouldUpdatePiano = true;
        for (unsigned int i=0; i < sizeof(_keysWhichArePressed); i++) {
            _keysWhichArePressed[i] = 0x00;
            _oldkeysWhichArePressed[i] = 0x00;
        }
    }

    void reset() {
        _shouldUpdatePiano = true;
        for (unsigned int i=0; i < sizeof(_keysWhichArePressed); i++) {
            _keysWhichArePressed[i] = 0x00;
            _oldkeysWhichArePressed[i] = 0x00;
        }
    }
    
    void setPosition(unsigned char x, unsigned char y) {
        _x = x;
        _y = y;
    }
    void keyDown(unsigned char key) {
        if (key < _offsetKeyZero) return;
        if (key >= _offsetKeyZero+(_octaves*12)) return;

        unsigned char index = key - _offsetKeyZero;

        unsigned char byteNumberOfKey = index / 8;
        unsigned char bitNumberOfKey = index % 8;

        //Serial.printf("ON: %d,%d -%d\n", byteNumberOfKey, bitNumberOfKey, key);

        //Serial.printf("before ON: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
        bitSet( _keysWhichArePressed[byteNumberOfKey], bitNumberOfKey);
        //Serial.printf("ON: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
        //_keysWhichArePressed[byteNumberOfKey] = b;
        _shouldUpdatePiano = true;
    }
    void keyUp(unsigned char key)  {
        if (key < _offsetKeyZero) return;
        if (key >= _offsetKeyZero+(_octaves*12)) return;

        unsigned char index = key - _offsetKeyZero;
        unsigned char byteNumberOfKey = index / 8;
        unsigned char bitNumberOfKey = index % 8;

        //Serial.printf("before OFF: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, b);
        bitClear( _keysWhichArePressed[byteNumberOfKey], bitNumberOfKey);

        //Serial.printf("OFF: %d,%d ::%x\n", byteNumberOfKey, bitNumberOfKey, _keysWhichArePressed[byteNumberOfKey]);
        _shouldUpdatePiano = true;
    }
    void drawFullPiano()  {
        _forceFullKeyboardRedraw = true;
        drawPiano();
    }
    
    void drawPiano()  {
        #define     RGB565_RED          0xF800
        #define     RGB565_BLACK        0x0000
        #define     RGB565_WHITE        0xFFFF
        for (int octave=0; octave < _octaves; octave++) {
            const int octaveOffset = (octave * _whiteKeyWidth * 7)/10;

            for (int i = 0; i < 12; i++) {
                int key = i + (octave * 12) + _offsetKeyZero;
                bool add = false;
                bool isDown = isKeyPressed(key);
                if (_forceFullKeyboardRedraw)
                    add = true;
                else {

                    bool wasKeyDown = wasKeyPressed(key);

                    if (isDown != wasKeyDown)
                        add = true;
                }

                if (add) {
                    int16_t color = isDown ? RGB565_RED : RGB565_WHITE;
                    int16_t color2 = isDown ? RGB565_RED : RGB565_BLACK;

                    switch (i % 12) {
                        case 0: //c
                            _tft.fillRect(_x + octaveOffset + _key_offset_c, _y,
                                          _key_offset_c_sharp, _two_thirds_key_height,
                                          color);

                            _tft.fillRect(_x + octaveOffset + _key_offset_c, _y + _two_thirds_key_height,
                                          _key_offset_d-_key_offset_c-1, _one_thirds_key_height,
                                          color);
                            break;

                        case 2: //d

                            _tft.fillRect(_x + octaveOffset + _key_offset_c_sharp_end, _y,
                                          _key_offset_d_sharp - _key_offset_c_sharp_end,  _two_thirds_key_height,
                                          color);

                            _tft.fillRect(_x + octaveOffset + _key_offset_d, _y + _two_thirds_key_height,
                                          _key_offset_e-_key_offset_d-1, _one_thirds_key_height,
                                          color);
                            break;

                        case 4: //e
                            _tft.fillRect(_x + octaveOffset + _key_offset_d_sharp_end, _y,
                                          _key_offset_f-_key_offset_d_sharp_end-1, _two_thirds_key_height,
                                          color);

                            _tft.fillRect(_x + octaveOffset + _key_offset_e, _y + _two_thirds_key_height,
                                          _key_offset_f-_key_offset_e-1, _one_thirds_key_height,
                                          color);
                            break;

                        case 5: //f
                            _tft.fillRect(_x + octaveOffset + _key_offset_f, _y,
                                          _key_offset_f_sharp-_key_offset_f, _two_thirds_key_height,
                                          color);

                            _tft.fillRect(_x + octaveOffset + _key_offset_f, _y + _two_thirds_key_height,
                                          _key_offset_g-_key_offset_f-1, _one_thirds_key_height,
                                          color);
                            break;

                        case 7: //g
                            _tft.fillRect(_x + octaveOffset + _key_offset_f_sharp_end, _y,
                                          _key_offset_g_sharp - _key_offset_f_sharp_end, _two_thirds_key_height,
                                          color);

                            _tft.fillRect(_x + octaveOffset + _key_offset_g, _y + _two_thirds_key_height,
                                          _key_offset_a-_key_offset_g-1, _one_thirds_key_height,
                                          color);
                            break;

                        case 9: // a
                            _tft.fillRect(_x + octaveOffset + _key_offset_g_sharp_end, _y,
                                          _key_offset_a_sharp - _key_offset_g_sharp_end, _two_thirds_key_height,
                                          color);

                            _tft.fillRect(_x + octaveOffset + _key_offset_a, _y + _two_thirds_key_height,
                                          _key_offset_b-_key_offset_a-1, _one_thirds_key_height,
                                          color);
                            break;

                        case 11: // b white note
                            _tft.fillRect(_x + octaveOffset + _key_offset_a_sharp_end, _y,
                                          _key_offset_b_end-_key_offset_a_sharp_end, _two_thirds_key_height,
                                          color);

                            _tft.fillRect(_x + octaveOffset + _key_offset_b, _y + _two_thirds_key_height,
                                          _key_offset_b_end-_key_offset_b, _one_thirds_key_height,
                                          color);
                            break;

/*--------------------------------------------------------------------------------------*/
                        case 1: // c#
                            _tft.fillRect(_x + octaveOffset + _key_offset_c_sharp, _y,
                                          _blackKeyWidth1/10, _two_thirds_key_height,
                                          color2);
                            break;

                        case 3: // d#
                            _tft.fillRect(_x + octaveOffset + _key_offset_d_sharp, _y,
                                          _blackKeyWidth1/10, _two_thirds_key_height,
                                          color2);
                            break;

                        case 6: // f#
                            _tft.fillRect(_x + octaveOffset + _key_offset_f_sharp, _y,
                                          _blackKeyWidth2/10, _two_thirds_key_height,
                                          color2);
                            break;

                        case 8: // g#
                            _tft.fillRect(_x + octaveOffset + _key_offset_g_sharp, _y,
                                          _blackKeyWidth2/10, _two_thirds_key_height,
                                          color2);
                            break;

                        case 10: // a#
                            _tft.fillRect(_x + octaveOffset + _key_offset_a_sharp, _y,
                                          _blackKeyWidth2/10, _two_thirds_key_height,
                                          color2);
                            break;
                    }
                }
            }
        }

        for (int j=0; j<22; j++)
            _oldkeysWhichArePressed[j] = _keysWhichArePressed[j];

        if(_forceFullKeyboardRedraw)
            _forceFullKeyboardRedraw = false;

        _shouldUpdatePiano = false;
    }
    bool displayNeedsUpdating() {
        return _shouldUpdatePiano || _forceFullKeyboardRedraw;
    }
    void setBaseOctave(uint8_t octave) {
        _startOctave = octave;
        _forceFullKeyboardRedraw = true;
    }

private:
    TDisplay &_tft;
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

    const int _sizeFactor = 6;
    int _whiteKeyWidth = (35*10)/_sizeFactor;
    int _blackKeyWidth1 = (21*10)/_sizeFactor;
    int _blackKeyWidth2 = (20*10)/_sizeFactor;
    int _two_thirds_key_height = (70*2/3)/_sizeFactor;
    int _one_thirds_key_height = (70*1/3)/_sizeFactor;
    int _key_offset_c = 0;
    int _key_offset_c_sharp = (_blackKeyWidth1)/10;
    int _key_offset_c_sharp_end = (2*_blackKeyWidth1)/10;
    int _key_offset_d = (_whiteKeyWidth)/10;
    int _key_offset_d_sharp = (_blackKeyWidth1*3)/10;
    int _key_offset_d_sharp_end = (4*_blackKeyWidth1)/10;
    int _key_offset_e = (_whiteKeyWidth*2)/10;
    int _key_offset_f = (_whiteKeyWidth*3)/10;
    int _key_offset_f_sharp = _key_offset_f + (_blackKeyWidth2)/10;
    int _key_offset_f_sharp_end = _key_offset_f_sharp + (_blackKeyWidth2)/10;
    int _key_offset_g = (_whiteKeyWidth*4)/10;
    int _key_offset_g_sharp = _key_offset_f + (_blackKeyWidth2*3)/10;
    int _key_offset_g_sharp_end = _key_offset_g_sharp + (_blackKeyWidth2)/10;
    int _key_offset_a = (_whiteKeyWidth*5)/10;
    int _key_offset_a_sharp = _key_offset_f + (_blackKeyWidth2*5)/10;
    int _key_offset_a_sharp_end = _key_offset_a_sharp + (_blackKeyWidth2)/10;
    int _key_offset_b = (_whiteKeyWidth*6)/10;
    int _key_offset_b_end = ((_whiteKeyWidth*7)/10) - 1;
};
#endif
