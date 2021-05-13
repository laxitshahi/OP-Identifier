// the libaries used
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// defines ports needed for color sensor
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 9
#define LED 10

// assigns integers based on tested frequency
int red = 16;
int blue = 20;
int green = 16;//

// sets up LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
void setup() {

  lcd.begin(16, 2); //position the lcd
  lcd.clear();

  //sets pins on Color Sensor to OUTPUT/INPUT accordingly)
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(sensorOut, INPUT);

  //Set frequnecy-scaling to 20%
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);
  Serial.begin(9600);
}


void loop() {
  // turns on LED
  digitalWrite(LED, HIGH);
  
  //sets starting point of cursor
  lcd.setCursor(0, 0);
  lcd.print("  Sensor Ready");//printing name
  color(); //see below for function
  
  //Prints intensities on serial monitor for debugging
  Serial.print("R Intensity:");
  Serial.print(red, DEC);
  Serial.print(" G Intensity: ");
  Serial.print(green, DEC);
  Serial.print(" B Intensity : ");
  Serial.print(blue, DEC);


  //If statements check the frequency of each color and outputs the Color on the LCD, based on the correct statement
  if (red < blue && red < green && red < 50)
  {

    Serial.println(" - (Color: Red)");
    lcd.setCursor(0, 1);
    lcd.print("   Color: RED   ");
    delay(250);
  }

  else if (blue < red && blue < green && blue < 80 && blue > 20 )
  {
    Serial.println(" - (Color: Blue)");
    lcd.setCursor(0, 1);
    lcd.print("  Color: Blue   ") ;
    delay(250);
  }

  else if (green < red && green < blue && green < 50)
  {
    Serial.println(" - (Color: Green)");
    lcd.setCursor(0, 1);
    lcd.print("  Color: Green   ");
    delay(250);
  }
  else if(green && blue && red < 10){
    Serial.println(" - (Color: White)");
    lcd.setCursor(0, 1);
    lcd.print("  Color: White   ");
    delay(250);
  }
  // if all conditions fail print "Out of Range"
  else {
    Serial.println();
    lcd.setCursor(0, 1);
    lcd.print("Out of Range o_o");
    delay (250);
  }
}


void color()
{
  //set photodiodes to read red (see diagram on doc for reference)
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  //count OUT, pRed, RED
  red = pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
  
  //set photodiodes to read blue
  digitalWrite(S3, HIGH);
  //count OUT, pBLUE, BLUE
  blue = pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);

  //set photodioes to read blue
  digitalWrite(S2, HIGH);
  //count OUT, pGreen, GREEN
  green = pulseIn(sensorOut, digitalRead(sensorOut) == HIGH ? LOW : HIGH);
}
