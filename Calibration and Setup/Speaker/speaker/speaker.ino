#include <stdio.h>
#include <string.h>
/*
  Melody on pin 8 of DUE

http://arduino.cc/en/Tutorial/Tone

*/


// notes in the melody:

//int melody[] = { 262,262,262,0,262, 0, 262, 262, 262
//  /*NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4*/ };
//int melodyGreen[] = {  

// note durations: 4 = quarter note, 8 = eighth note, etc.:
//int noteDurations[] = {
//  8, 4, 8, 8,4,8,8,8,8 };

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
void setup() {
}

void loop() {
  // iterate over the notes of the melody:

char abc[20] = "sos";
for(int i = 0; i < strlen(abc); i++){
  morseOut(abc[i]);
  delay(500);
}
 delay(3000);
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
