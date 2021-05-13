// libaries used
// Tone library was directly implemented
#include <HX711_ADC.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <string.h>

//List of letters to use with Tone library
int letters[ ][4] = 
{{6,2,0,0}, //a
{2,6,6,6},  //b
{2,6,2,6},  //c
{2,6,6,0},  //d
{6,0,0,0},  //e
{6,6,2,6},  //f
{2,2,6,0},  //g
{6,6,6,6},  //h
{6,6,0,0},  //i
{6,2,2,2},  //j
{2,6,2,0},  //k
{6,2,6,6},  //l
{2,2,0,0},  //m
{2,6,0,0},  //n
{2,2,2,0},  //o
{6,2,2,6},  //p
{2,2,6,2},  //q
{6,2,6,0},  //r
{6,6,6,0},  //s
{2,0,0,0},  //t
{6,6,2,0},  //u
{6,6,6,2},  //v
{6,2,2,0},  //w
{2,6,6,2},  //x
{2,6,2,2},  //y
{2,2,6,6}}; //z

int on = 255;
int off = 0;
int rOUT;
int gOUT;
int yOUT;
String color = "UNKWN"; // Default value when no color has been detected

// defines ports needed for color sensor
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 9
#define LED1 10 //Color Sensor LED
#define rLED 11 //RED LED
#define gLED 12 //GREEN LED
#define yLED 13 //YELLOW LED (representing the color blue)


// calibrated frequencies for each main color on RGB sensor
int red = 135;
int blue = 200;
int green = 150;//

float weight = 0.00;


//sets up load cell (weight sensor)
HX711_ADC LoadCell(22, 23); 
// sets up LCD
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

void setup() {
  lcd.begin(16, 2); // position the lcd
  lcd.backlight(); // turns on the backlight
  lcd.clear(); // 
 
  LoadCell.begin(); // start connection to HX711
  LoadCell.start(2000); // load cells gets 2000ms of time to stabilize
  LoadCell.setCalFactor(107.8); //claibatrion number based on digital scale
  
  //sets pins on Color Sensor to OUTPUT/INPUT accordingly)
  pinMode(S0, OUTPUT); 
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(rLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(yLED, OUTPUT);
  pinMode(sensorOut, INPUT);

  //Set frequnecy-scaling to 2%
  digitalWrite(S0, LOW);
  digitalWrite(S1, HIGH);
}

void loop() {
  // turns on LED on RGB sensor

  digitalWrite(LED1, HIGH);
  analogWrite(rLED,rOUT); 
  analogWrite(gLED,gOUT);
  analogWrite(yLED,yOUT);

  
  //sets starting point of cursor
  lcd.setCursor(0, 0);
  lcd.print("Color:");//printing name
  lcd.print("   ");
  lcd.print("Weight:");
  delay(200); //to give color sensor time to read the object
  colors(); //see below for function

  weight = 0;
  LoadCell.update(); // retrieves data from the load cell
  weight = LoadCell.getData(); // get output value
  
  //If  to statements decide if weight and color match a specific object
  //Sets object to 
  if (weight > 215 && weight < 225 && color == "YELLOW"){
    object("BANANA");
  }
  else if(weight > 126 && weight < 128 && color == "BLUE"){
    object("CALC");
  }
  else if(weight > 105 && weight < 107 && color == "ORANGE"){
    object("ORANGE");
  }
  else if(weight > 97 && weight <99.9 && color == "BLUE" ){ 
    object("CARDS");
  }
  else if(weight > 29.5 && weight < 32.1 && color == "BLACK"){
    object("BOX");
  }
  else if(weight > 23 && weight < 25 && color == "WHITE"){
    object("ERASER");
  }

  
  
  lcd.setCursor(0, 1); //set Cursor to second line

  
  //If statements to determine the color the sensor detects
  if (red < blue && red < green && blue < green && red < 300 && red > 100)
  {
    setColor("RED");
  }
  else if (red < blue && green < blue && red < 200 && red > 50){
    if(blue > 200 && green > 170){
      setColor("ORANGE");
    }
    else{
      setColor("YELLOW");
    } 
  } 
  else if (blue < red && blue < green && blue < 400 && blue > 100)
  {
    setColor("BLUE");
  }
  
  else if (green < red && green < blue && green < 300 && green > 100)
  {
    setColor("GREEN");
  }
  else if(green && blue && red < 80){
    setColor("WHITE");
  }
  else if(green > 400 && blue > 420 && red > 400 && green < 2000 && blue < 2000 && red < 2000){
    delay(800);
    if(green > 400 && blue > 400 && red > 400 && green < 2000 && blue < 2000 && red < 2000){
      setColor("BLACK");
    }
  }
  else{
    lcd.print(color);
    lcd.print("     ");
    lcd.print(weight);
  }
}

void object(char* object){ // Displays name of object visually and audiably when detected
    color = "UNKWN";
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("OBJECT FOUND:");
    lcd.setCursor(0,1);
    lcd.print(object);
    morseOut(object);  
}

void setColor(char* col){ //sets global color varible and outputs color/weight on LCD
  if(red < blue && red < green){
    rOUT = on;
    yOUT = off;
    gOUT = off;
  }
  else if(blue < red && blue < green){
    rOUT = off;
    yOUT = on;
    gOUT = off;
  }
  else if(green < blue && green < red){
    rOUT = off;
    yOUT = off;
    gOUT = on;
  }
    color = col;
    lcd.print(col);
    lcd.print("      ");
    lcd.print(weight);
    morseOut(col);
}

void colors()
{
  //set photodiodes to read red
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

// Uses tone function to loop through given array of characters (input) to output the given input in morse code
void morseOut(char* input){ 
  for(int j = 0; j < strlen(input); j++){
      for(int i = 0; i < 4; i++){
    if(letters[(int)input[j] - 65][i] == 0){
      continue;
    }
    int noteDuration = 1000/letters[(int)input[j] - 65][i];
    tone(8, 262 , noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(10);
  }
  delay(100);
  }
}


/*
Tone generator
v1  use timer, and toggle any digital pin in ISR
   funky duration from arduino version
   TODO use FindMckDivisor?
   timer selected will preclude using associated pins for PWM etc.
    could also do timer/pwm hardware toggle where caller controls duration
*/


// timers TC0 TC1 TC2   channels 0-2 ids 0-2  3-5  6-8     AB 0 1
// use TC1 channel 0
#define TONE_TIMER TC1
#define TONE_CHNL 0
#define TONE_IRQ TC3_IRQn

// TIMER_CLOCK4   84MHz/128 with 16 bit counter give 10 Hz to 656KHz
//  piano 27Hz to 4KHz

static uint8_t pinEnabled[PINS_COUNT];
static uint8_t TCChanEnabled = 0;
static boolean pin_state = false ;
static Tc *chTC = TONE_TIMER;
static uint32_t chNo = TONE_CHNL;

volatile static int32_t toggle_count;
static uint32_t tone_pin;

// frequency (in hertz) and duration (in milliseconds).

void tone(uint32_t ulPin, uint32_t frequency, int32_t duration)
{
    const uint32_t rc = VARIANT_MCK / 256 / frequency;
    tone_pin = ulPin;
    toggle_count = 0;  // strange  wipe out previous duration
    if (duration > 0 ) toggle_count = 2 * frequency * duration / 1000;
     else toggle_count = -1;

    if (!TCChanEnabled) {
      pmc_set_writeprotect(false);
      pmc_enable_periph_clk((uint32_t)TONE_IRQ);
      TC_Configure(chTC, chNo,
        TC_CMR_TCCLKS_TIMER_CLOCK4 |
        TC_CMR_WAVE |         // Waveform mode
        TC_CMR_WAVSEL_UP_RC ); // Counter running up and reset when equals to RC
  
      chTC->TC_CHANNEL[chNo].TC_IER=TC_IER_CPCS;  // RC compare interrupt
      chTC->TC_CHANNEL[chNo].TC_IDR=~TC_IER_CPCS;
       NVIC_EnableIRQ(TONE_IRQ);
                         TCChanEnabled = 1;
    }
    if (!pinEnabled[ulPin]) {
      pinMode(ulPin, OUTPUT);
      pinEnabled[ulPin] = 1;
    }
    TC_Stop(chTC, chNo);
                TC_SetRC(chTC, chNo, rc);    // set frequency
    TC_Start(chTC, chNo);
}

void noTone(uint32_t ulPin)
{
  TC_Stop(chTC, chNo);  // stop timer
  digitalWrite(ulPin,LOW);  // no signal on pin
}

// timer ISR  TC1 ch 0
void TC3_Handler ( void ) {
  TC_GetStatus(TC1, 0);
  if (toggle_count != 0){
    // toggle pin  TODO  better
    digitalWrite(tone_pin,pin_state= !pin_state);
    if (toggle_count > 0) toggle_count--;
  } else {
    noTone(tone_pin);
  }
}
