#include <ST7735_t3.h> // Hardware-specific library
#include "TFTPianoDisplay.h"
#include <ST7735_t3View.h>
#ifdef BUILD_FOR_OPENGL_EMULATOR
#include "st7735_opengl.h"
st7735_opengl tft = st7735_opengl(true, 10);
void my_yield() {
    if(!tft.shouldClose()) {
        tft.update();
    }
}
#else
#define TFT_CS   6  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define TFT_DC    2  //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define TFT_RST   255 // RST can use any pin
ST7735_t3 tft = ST7735_t3(TFT_CS, TFT_DC, TFT_RST);


ST7735_t3View view(tft);
#endif

TFTPianoDisplay pianoDisplay1(view, 2, 3, 0, 0); //tft, byte octaves, byte startOctave, byte x, byte y
TFTPianoDisplay pianoDisplay2(view, 2, 3, 0, 16, 5); //tft, byte octaves, byte startOctave, byte x, byte y
TFTPianoDisplay pianoDisplay3(view, 2, 3, 0, 48, 4); //tft, byte octaves, byte startOctave, byte x, byte y
TFTPianoDisplay pianoDisplay4(view, 1, 3, 0, 70, 3); //tft, byte octaves, byte startOctave, byte x, byte y

bool useFrameBuffer = true;
u_long lastMillisKey1,lastMillisKey2, lastMillisKey3, lastMillisKey4  = 0;
byte key1, key2, key3, key4 = 35;

void setup() {
#ifdef BUILD_FOR_LINUX
    initialize_mock_arduino();
    yield_impl = my_yield;
#endif

    //SPI.begin();
    tft.initR(INITR_GREENTAB);
    tft.setRotation(3);
    if (useFrameBuffer)
        tft.useFrameBuffer(true);

    if (useFrameBuffer)
        tft.updateScreenAsync(false);

    tft.fillScreen(ST7735_BLACK);
    tft.println("init...");

    Serial.begin(9600);

    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0,0);
}


void loop() {
    if (useFrameBuffer)
        tft.updateScreenAsync(false);

    if (pianoDisplay1.displayNeedsUpdating())
        pianoDisplay1.drawPiano();
    if (pianoDisplay2.displayNeedsUpdating())
        pianoDisplay2.drawPiano();
    if (pianoDisplay3.displayNeedsUpdating())
        pianoDisplay3.drawPiano();
    if (pianoDisplay4.displayNeedsUpdating())
        pianoDisplay4.drawPiano();

    unsigned long currentMillis = millis();
    if(currentMillis - lastMillisKey1 > 50) {
        lastMillisKey1 = currentMillis;

        pianoDisplay1.keyUp(key1);
        key1 ++;
        if (key1 > 60) key1 = 35;
        pianoDisplay1.keyDown(key1);
    }
    if(currentMillis - lastMillisKey2 > 100) {
        lastMillisKey2 = currentMillis;

        pianoDisplay2.keyUp(key2);
        key2 ++;
        if (key2 > 60) key2 = 35;
        pianoDisplay2.keyDown(key2);
    }
    if(currentMillis - lastMillisKey3 > 200) {
        lastMillisKey3 = currentMillis;

        pianoDisplay3.keyUp(key3);
        key3 ++;
        if (key3 > 60) key3 = 35;
        pianoDisplay3.keyDown(key3);
    }
    if(currentMillis - lastMillisKey4 > 400) {
        lastMillisKey4 = currentMillis;

        pianoDisplay4.keyUp(key4);
        key4 ++;
        if (key4 > 47) key4 = 35;
        pianoDisplay4.keyDown(key4);
    }

    if (useFrameBuffer) {
        tft.updateScreenAsync(true);
        tft.updateScreen();
    }

}

#ifdef BUILD_FOR_LINUX
int main() {
    setup();
    while (!tft.shouldClose()) {
        loop();
    }
}
#endif

unsigned __exidx_start;
unsigned __exidx_end;