#include <SPI.h>
#include <SD.h>
#include "sequencer.h"
#include "tempo.h"
#include "midisequenceadapter.h"
#include <ST7735_t3.h> // Hardware-specific library
#define TFT_CS   6  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define TFT_DC    2  //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define TFT_RST   255 // RST can use any pin
ST7735_t3 tft = ST7735_t3(TFT_CS, TFT_DC, TFT_RST);

tempo tempo(100.0f);
multisequencer multisequencer(tempo);
midisequenceadapter adapter(multisequencer);

sequencer *guitar1sequencer;
sequencer *guitar2sequencer;
sequencer *guitar3sequencer;
sequencer *guitar4sequencer;

#include "TFTPianoDisplay.h"
TFTPianoDisplay pianoDisplay1a(tft, 3, 3, 0, 0); //tft, byte octaves, byte startOctave, byte x, byte y
TFTPianoDisplay pianoDisplay1b(tft, 3, 6, 0, 16); //tft, byte octaves, byte startOctave, byte x, byte y

TFTPianoDisplay pianoDisplay2a(tft, 3, 3, 0, 32); //tft, byte octaves, byte startOctave, byte x, byte y
TFTPianoDisplay pianoDisplay2b(tft, 3, 6, 0, 48); //tft, byte octaves, byte startOctave, byte x, byte y

TFTPianoDisplay pianoDisplay3a(tft, 3, 3, 0, 64); //tft, byte octaves, byte startOctave, byte x, byte y
TFTPianoDisplay pianoDisplay3b(tft, 3, 6, 0, 80); //tft, byte octaves, byte startOctave, byte x, byte y

TFTPianoDisplay pianoDisplay4a(tft, 3, 3, 0, 94); //tft, byte octaves, byte startOctave, byte x, byte y
TFTPianoDisplay pianoDisplay4b(tft, 3, 6, 0, 110); //tft, byte octaves, byte startOctave, byte x, byte y

void sequenceGuitarEvent( sequencerevent *event, TFTPianoDisplay &tftPianoDisplay1, TFTPianoDisplay &tftPianoDisplay2) {
    if (event->isNoteStartEvent) {
        tftPianoDisplay1.keyDown(event->noteNumber);
        tftPianoDisplay2.keyDown(event->noteNumber);
    } else {
        tftPianoDisplay1.keyUp(event->noteNumber);
        tftPianoDisplay2.keyUp(event->noteNumber);
    }
};

void setup() {
    SPI.begin();
    tft.initR(INITR_GREENTAB);
    tft.setRotation(3);
    tft.fillScreen(ST7735_BLACK);
    tft.println("init...");

    Serial.begin(9600);

    while (!(SD.begin(BUILTIN_SDCARD))) {
        // stop here if no SD card, but print a message
        tft.fillScreen(ST7735_BLACK);
        tft.setCursor(0,0);
        tft.println("Unable to access the SD card");
        Serial.println("Unable to access the SD card");
        delay(1000);
    }

    tft.fillScreen(ST7735_BLACK);
    tft.setCursor(0,0);

    guitar1sequencer = multisequencer.newSequencer();
    guitar2sequencer = multisequencer.newSequencer();
    guitar3sequencer = multisequencer.newSequencer();
    guitar4sequencer = multisequencer.newSequencer();

    guitar1sequencer->onevent = [] (sequencerevent *event) { sequenceGuitarEvent(event, pianoDisplay1a, pianoDisplay1b);  };
    guitar2sequencer->onevent = [] (sequencerevent *event) { sequenceGuitarEvent(event, pianoDisplay2a, pianoDisplay2b);  };
    guitar3sequencer->onevent = [] (sequencerevent *event) { sequenceGuitarEvent(event, pianoDisplay3a, pianoDisplay3b);  };
    guitar4sequencer->onevent = [] (sequencerevent *event) { sequenceGuitarEvent(event, pianoDisplay4a, pianoDisplay4b);  };

    int pattern = 0;
    int currentChannel = 0;

    // guitar 1 sequencer
    pattern = guitar1sequencer->addPattern(4); // no beat

    pattern++;
    adapter.loadMidi("innuendo.mid");
    adapter.loadMidifileToNextChannelPattern(currentChannel, 1, 35, 0);  // multicequencer channel number, midi track number, 35 bars long
    guitar1sequencer->setNextPattern(pattern);

    // guitar 2 sequencer
    currentChannel++;
    pattern = guitar2sequencer->addPattern(4); // no beat 4 bars

    pattern++;
    adapter.loadMidifileToNextChannelPattern(currentChannel, 2, 35, 0);  // multicequencer channel number, midi track number, 35 bars long
    guitar2sequencer->setNextPattern(pattern);

    // guitar 3 sequencer
    currentChannel++;
    pattern = guitar3sequencer->addPattern(4); // no hats

    pattern++;
    adapter.loadMidifileToNextChannelPattern(currentChannel, 3, 35, 0);  // multicequencer channel number, midi track number, 35 bars long
    guitar3sequencer->setNextPattern(pattern);

    // guitar 4 sequencer
    currentChannel++;
    pattern = guitar4sequencer->addPattern(4); // no bass

    pattern++;
    adapter.loadMidifileToNextChannelPattern(currentChannel, 4, 35, 0);
    adapter.close();
    guitar4sequencer->setNextPattern(pattern);

    multisequencer.start(micros());
}

float oldBPM = 0.0;
float newBPM = 0.0;
long bpmIndicatorTimer = 0;

void loop() {
    if (pianoDisplay1a.displayNeedsUpdating())
        pianoDisplay1a.drawPiano();
    if (pianoDisplay1b.displayNeedsUpdating())
        pianoDisplay1b.drawPiano();

    if (pianoDisplay2a.displayNeedsUpdating())
        pianoDisplay2a.drawPiano();
    if (pianoDisplay2b.displayNeedsUpdating())
        pianoDisplay2b.drawPiano();

    if (pianoDisplay3a.displayNeedsUpdating())
        pianoDisplay3a.drawPiano();
    if (pianoDisplay3b.displayNeedsUpdating())
        pianoDisplay3b.drawPiano();

    if (pianoDisplay4a.displayNeedsUpdating())
        pianoDisplay4a.drawPiano();
    if (pianoDisplay4b.displayNeedsUpdating())
        pianoDisplay4b.drawPiano();

    bpmIndicatorTimer++;
    if (bpmIndicatorTimer == 100000){
        bpmIndicatorTimer = 0;

        if (newBPM != oldBPM) {
            tft.setTextColor(ST7735_BLACK);
            tft.setCursor(0,0);
            tft.setTextSize(2);
            tft.printf("BPM:\n%.1f", oldBPM);

            tft.setTextColor(ST7735_RED);
            tft.setCursor(0,0);
            tft.setTextSize(2);
            tft.printf("BPM:\n%.1f", newBPM);
            oldBPM = newBPM;
        }
    }

    // put your main code here, to run repeatedly:
    multisequencer.tick(micros());

}

unsigned __exidx_start;
unsigned __exidx_end;