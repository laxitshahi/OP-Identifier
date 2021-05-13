// the libaries used
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdio.h>
#include <string.h>

int letters[ ][4] = 
{{8,2,0,0}, //a
{2,8,8,8},  //b
{2,8,2,8},  //c
{2,8,8,0},  //d
{8,0,0,0},  //e
{8,8,2,8},  //f
{2,2,8,0},  //g
{8,8,8,8},  //h
{8,8,0,0},  //i
{8,2,2,2},  //j
{2,8,2,0},  //k
{8,2,8,8},  //l
{2,2,0,0},  //m
{2,8,0,0},  //n
{2,2,2,0},  //o
{8,2,2,8},  //p
{2,2,8,2},  //q
{8,2,8,0},  //r
{8,8,8,0},  //s
{2,0,0,0},  //t
{8,8,2,0},  //u
{8,8,8,2},  //v
{8,2,2,0},  //w
{2,8,8,2},  //x
{2,8,2,2},  //y
{2,2,8,8}}; //z

// defines ports needed for color sensor
#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 9
#define LED 10

// assigns integers based on tested frequency
int red = 135;
int blue = 200;
int green = 150;//

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

  //Set frequnecy-scaling to 2%
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
  if (red < blue && red < green && red < 300)
  {

    Serial.println(" - (Color: Red)");
    lcd.setCursor(0, 1);
    lcd.print("   Color: RED   ");
//    char red[20] = "red";
//    for(int i = 0; i < strlen(red); i++){
//    morseOut(red[i]);
//    delay(500);
//    }
    delay(500);
  }

  else if (blue < red && blue < green && blue < 300)
  {
    Serial.println(" - (Color: Blue)");
    lcd.setCursor(0, 1);
    lcd.print("  Color: Blue   ") ;
    delay(500);
  }

  else if (green < red && green < blue && green < 300)
  {
    Serial.println(" - (Color: Green)");
    lcd.setCursor(0, 1);
    lcd.print("  Color: Green   ");
    delay(500);
  }
  else if(green && blue && red < 60){
    Serial.println(" - (Color: White)");
    lcd.setCursor(0, 1);
    lcd.print("  Color: White   ");
    delay(500);
  }
  else if(green && blue && red > 350 && green && blue && red < 2000 ){
    Serial.println(" - (Color: Black)");
    lcd.setCursor(0, 1);
    lcd.print("  Color: Black   ");
    delay(500);
  }
  // if all conditions fail print "Out of Range"
  else {
    Serial.println();
    lcd.setCursor(0, 1);
    lcd.print("Out of Range o_o");
    delay (500);
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

void morseOut(char c){
  for(int i = 0; i < 4; i++){
    if(letters[(int)c - 97][i] == 0){
      continue;
    }
    int noteDuration = 1000/letters[(int)c - 97][i];
    tone(8, 262 , noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
//    noTone(2);
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
