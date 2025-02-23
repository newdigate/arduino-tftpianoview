#include "TFTPianoDisplay.h"

TFTPianoDisplay::TFTPianoDisplay(View &tft, unsigned char octaves, unsigned char startOctave, unsigned char x,
                                 unsigned char y, unsigned char size) : _tft(tft){
    _x = x;
    _y = y;
    setSizeFactor(size);
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

void TFTPianoDisplay::reset() {
    _shouldUpdatePiano = true;
    for (unsigned int i=0; i < sizeof(_keysWhichArePressed); i++) {
        _keysWhichArePressed[i] = 0x00;
        _oldkeysWhichArePressed[i] = 0x00;
    }
}

void TFTPianoDisplay::setPosition(unsigned char x, unsigned char y) {
    _x = x;
    _y = y;
}

void TFTPianoDisplay::keyDown(unsigned char key) {
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

void TFTPianoDisplay::keyUp(unsigned char key) {
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

void TFTPianoDisplay::drawFullPiano() {
    _forceFullKeyboardRedraw = true;
    drawPiano();
}

void TFTPianoDisplay::drawPiano() {
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

bool TFTPianoDisplay::displayNeedsUpdating() {
    return _shouldUpdatePiano || _forceFullKeyboardRedraw;
}

void TFTPianoDisplay::setBaseOctave(uint8_t octave) {
    _startOctave = octave;
    _forceFullKeyboardRedraw = true;
}
