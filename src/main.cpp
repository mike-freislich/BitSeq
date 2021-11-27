#include <Arduino.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

uint8_t step = 0;
uint8_t x = 0, y = 0;
uint32_t last = 0;
byte bpm = 78;
uint16_t tempoMS = 60.0 / bpm / 4.0 * 1000;// 125;

#define trackLength 64
#define numTracks   16
uint8_t patternLength = 16*4;
uint64_t track[numTracks];   

void updateDisplay();

void setup()
{
  track[0]  = 0b0001000100010001000100010001000100010001000100010001000100010001; // kick drum
  track[1]  = 0b0001000000010000000100000001000000010000000100000001000000010000; // clap / snare
  track[2]  = 0b0100010001000100010001000100010001000100010001000100010001000100; // o-hh
  track[3]  = 0b1011101110111011101110111011101101111011101110111011101110111011; // c-hh
  track[4]  = 0b0001000100010001000100010001000100010001000100010001000100010001; // kick drum
  track[5]  = 0b0001000000010000000100000001000000010000000100000001000000010000; // clap / snare
  track[6]  = 0b0100010001000100010001000100010001000100010001000100010001000100; // o-hh
  track[7]  = 0b1011101110111011101110111011101101111011101110111011101110111011; // c-hh
  track[8]  = 0b0001000100010001000100010001000100010001000100010001000100010001; // kick drum
  track[9]  = 0b0001000000010000000100000001000000010000000100000001000000010000; // clap / snare
  track[10] = 0b0100010001000100010001000100010001000100010001000100010001000100; // o-hh
  track[11] = 0b1011101110111011101110111011101101111011101110111011101110111011; // c-hh
  track[12] = 0b0001000100010001000100010001000100010001000100010001000100010001; // kick drum
  track[13] = 0b0001000000010000000100000001000000010000000100000001000000010000; // clap / snare
  track[14] = 0b0100010001000100010001000100010001000100010001000100010001000100; // o-hh
  track[15] = 0b1011101110111011101110111011101101111011101110111011101110111011; // c-hh
  lcd.begin(16, 2);
}

void loop()
{

  uint32_t now = millis();
  uint32_t elapsed = now - last;

  if (elapsed >= tempoMS)
  {
    step = (step + 1) % patternLength;
    last = now;
    updateDisplay();
  }

  delay(10);
}

void updateDisplay()
{
  byte bar = step/16 + 1;
  byte quarter = step/4 % 4 + 1;
  byte sixteenth = step%4 + 1; 
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("bpm:" + String(bpm) + " | " + String(bar) + "-" + String(quarter) + "-" + String(sixteenth));

  for (byte i = 0; i < numTracks; i++)
  {
    char c = char(32);
    if (bitRead(track[i], step))
      c = char(255);
    lcd.setCursor(i, 1);
    lcd.print(c);
  }
}