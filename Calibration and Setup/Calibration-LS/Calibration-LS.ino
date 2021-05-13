// (c) Michael Schoeffler 2017, http://www.mschoeffler.de
#include <HX711_ADC.h> // https://github.com/olkal/HX711_ADC
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // LiquidCrystal_I2C library

HX711_ADC LoadCell(22, 23); // parameters: dt pin, sck pin<span data-mce-type="bookmark" style="display: inline-block; width: 0px; overflow: hidden; line-height: 0;" class="mce_SELRES_start"></span>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); // 0x27 is the i2c address of the LCM1602 IIC v1 module (might differ)

void setup() {
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(107.8); // calibration factor for load cell => strongly dependent on your individual setup
  lcd.begin(16, 2); // begins connection to the LCD module
  lcd.backlight(); // turns on the backlight
}

void loop() {
  LoadCell.update(); // retrieves data from the load cell
  float i = LoadCell.getData(); // get output value
  lcd.setCursor(0, 0); // set cursor to first row
  lcd.print("Weight[g]:"); // print out to LCD
  lcd.setCursor(0, 1); // set cursor to secon row
  lcd.print(i); // print out the retrieved value to the second row
}
