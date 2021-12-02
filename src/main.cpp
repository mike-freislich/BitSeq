#include <Arduino.h>
#include <LiquidCrystal.h>
#include <ShiftRegister74HC595.h>
#include <pins_arduino.h>


const int rs = 1, en = 4, d4 = 5, d5 = 6, d6 = 7, d7 = 3;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

uint8_t step = 0;
uint8_t x = 0, y = 0;
uint32_t last = 0;
uint8_t bpm = 130;
uint8_t gatePercent = 90;
uint16_t tempoMS = 60.0 / bpm / 4.0 * 1000;

#define trackLength 64
#define numTracks   16
uint8_t patternLength = 16*4;
uint64_t track[numTracks];   

//ShiftRegister74HC595<2> sr(6, 7, 8); // parameters: <number of shift registers> (data pin, clock pin, latch pin)

void updateDisplay();
void setGate(uint8_t percent);

void setup()
{
  //Serial.begin(19200);
  //Serial.println("testing");
  track[0]  = 0b0001000100010001000100010001000100010001000100010001000100010001; // kick drum
  track[1]  = 0b0001000000010000000100000001000000010000000100000001000000010000; // clap / snare
  track[2]  = 0b0100010001000100010001000100010001000100010001000100010001000100; // o-hh
  track[3]  = 0b1011101110111011101110111011101101111011101110111011101110111011; // c-hh
  track[4]  = 0b0001000100010001000100010001000100010001000100010000000000000000; // kick drum
  track[5]  = 0b0001000000010000000100000001000000010000000100000000000000000000; // clap / snare
  track[6]  = 0b0100010001000100010001000100010001000100010001000000000000000000; // o-hh
  track[7]  = 0b1011101110111011101110111011101101111011101110110000000000000000; // c-hh
  track[8]  = 0b0001000100010001000100010001000100000000000000000000000000000000; // kick drum
  track[9]  = 0b0001000000010000000100000001000000000000000000000000000000000000; // clap / snare
  track[10] = 0b0100010001000100010001000100010000000000000000000000000000000000; // o-hh
  track[11] = 0b1011101110111011101110111011101100000000000000000000000000000000; // c-hh
  track[12] = 0b0001000100010001000000000000000000000000000000000000000000000000; // kick drum
  track[13] = 0b0001000000010000000000000000000000000000000000000000000000000000; // clap / snare
  track[14] = 0b0100010001000100000000000000000000000000000000000000000000000000; // o-hh
  track[15] = 0b1011101110111011000000000000000000000000000000000000000000000000; // c-hh
  
  //sr.setAllLow(); 
  byte c1[8] = {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b11111};    
  byte c2[8] = {0b00000,0b00000,0b00000,0b00000,0b00000,0b00000,0b11111,0b11111};
  byte c3[8] = {0b00000,0b00000,0b00000,0b00000,0b00000,0b11111,0b11111,0b11111};
  byte c4[8] = {0b00000,0b00000,0b00000,0b00000,0b11111,0b11111,0b11111,0b11111};
  byte c5[8] = {0b00000,0b00000,0b00000,0b11111,0b11111,0b11111,0b11111,0b11111};
  byte c6[8] = {0b00000,0b00000,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};
  byte c7[8] = {0b00000,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};
  byte c8[8] = {0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111,0b11111};
  
  lcd.begin(16, 2);  
  
  lcd.createChar(0, c1);
  lcd.createChar(1, c2);
  lcd.createChar(2, c3);
  lcd.createChar(3, c4);
  lcd.createChar(4, c5);
  lcd.createChar(5, c6);
  lcd.createChar(6, c7);
  lcd.createChar(7, c8);
  lcd.noCursor();
  //delay(10);
  //setGate(90);
}

bool cleared;

void loop()
{  
  if (step % 16 == 0) {
    setGate(random(10,100));    
  }

  uint32_t now = millis();
  uint32_t elapsed = now - last;

  if (!cleared && (elapsed >= tempoMS * gatePercent/100.0)) {
    //sr.setAllLow();
    cleared = true;
  }

  if (elapsed >= tempoMS)
  {
    step = (step + 1) % patternLength;
    last = now;
    updateDisplay();
    cleared = false;
  }

  delay(10);
}

void setGate(uint8_t percent) {

  if (gatePercent == percent) return;
  gatePercent = percent;
 
}

void updateDisplay()
{
  byte bar = step/16 + 1;
  byte quarter = step/4 % 4 + 1;
  byte sixteenth = step%4 + 1; 
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.noCursor();
  lcd.print(String(bar) + "-" + String(quarter) + "-" + String(sixteenth));
  lcd.setCursor(9,0);
  lcd.print("bpm:" + String(bpm));

  uint16_t stepData;

  for (byte i = 0; i < numTracks; i++)
  {
    if (bitRead(track[i], step)) {
      bitSet(stepData, i);
      lcd.setCursor(i, 1);            
      lcd.write(byte((byte) (8.0 * (int)gatePercent / (float)100)));
    }  
    
  }

  //uint8_t regData[] = {(uint8_t) (stepData << 8), (uint8_t) stepData};
  //sr.setAll(regData);
  
}