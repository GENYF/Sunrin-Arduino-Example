#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup()
{
	lcd.begin();
	lcd.backlight();
  lcd.clear();
	lcd.print("Hello, world!");
}

void loop()
{
	lcd.setCursor(5,1);
  lcd.print("ABC");
}
