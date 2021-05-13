#include <LiquidCrystal.h>

#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 9
#define LED 10

int frequency = 0;
void setup() {
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
  digitalWrite(LED, HIGH);
  //set red filter photodiodes to be read
  digitalWrite(S2,LOW); 
  digitalWrite(S3,LOW);
  //read output frequency
  frequency = pulseIn(sensorOut, LOW);
  //prints value on the serial monitor
  Serial.print("R= "); //prints name
  Serial.print(frequency);  //red  color freq
  Serial.print(" ");
  delay(100);


  //set green filter photodiodes to be read
  digitalWrite(S2,HIGH); 
  digitalWrite(S3,HIGH);
  //read output frequency
  frequency = pulseIn(sensorOut, LOW);
  //prints value on the serial monitor
  Serial.print("G= "); //prints name
  Serial.print(frequency);//red? color freq
  Serial.print(" ");
  delay(100);


  //set blue filter photodiodes to be read
  digitalWrite(S2,LOW); 
  digitalWrite(S3,HIGH);
  //read output frequency
  frequency = pulseIn(sensorOut, LOW);
  //prints value on the serial monitor
  Serial.print("B= "); //prints name
  Serial.print(frequency); //red? color freq
  Serial.println(" ");
  delay(100);
  
}
