#include <Arduino.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

uint32_t count = 0;
uint8_t x = 0, y = 0;
uint32_t last = 0;

String buffer1 = "                ";
String buffer2 = "                ";

void make_smiley(byte index);
void updateDisplay();
void updateBuffer();

void setup()
{
  lcd.begin(16, 2);
}

void loop()
{
  updateBuffer();
  updateDisplay();
  delay(500);
}

void make_smiley(byte index)
{
  byte smiley[8] = {
      B00000,
      B10001,
      B00000,
      B00000,
      B10001,
      B01110,
      B11111,
      B11111
  };

  lcd.createChar(index, smiley);
}

void updateBuffer()
{

  count++;
  buffer1 = "PTN: " + (String(count));

  buffer2[x] = char(32);
  x = (x + 1) % 16;
  buffer2[x] = char(255);  
}

void updateDisplay()
{
  uint32_t now = millis();
  uint32_t elapsed = now - last;
  if (elapsed >= 100)
  {
    last = now + 100-elapsed;        
    lcd.setCursor(0, 0);
    lcd.print(buffer1);
    lcd.setCursor(0, 1);
    lcd.print(buffer2);
  }
}