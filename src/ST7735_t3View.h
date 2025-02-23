//
// Created by Moolet on 23/02/2025.
//

#ifndef ARDUINO_TFTPIANOVIEW_ST7735_T3VIEW_H
#define ARDUINO_TFTPIANOVIEW_ST7735_T3VIEW_H

#include <ST7735_t3.h> // Hardware-specific library
#include <View.h>

class ST7735_t3View : public View {
public:
    ST7735_t3View(ST7735_t3 &display) : View(128, 128), _display(display) {
    }

    void Pixel(int16_t x, int16_t y, uint16_t color) override {
        _display.drawPixel(x,y, color);
    }
private:
    ST7735_t3 &_display;
};

#endif //ARDUINO_TFTPIANOVIEW_ST7735_T3VIEW_H
