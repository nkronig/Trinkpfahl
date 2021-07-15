/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/
#define pMAX 4
#define pMIN 3

#define LED_MAX 255
#define LED_MIN 0

#define OPEN_TIME 1000

#define VALVE 8
#define PUMP 3
#define BUTTON A5
#define LED 11

int ledFadeValue = 0;
bool ledFadeDir = true;
float p;
float pAvg;
float pAvgArr[5];
int pAvgArrIndex = 0;
bool pumpOn = false;
float pOld;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);
  
  pinMode(LED, OUTPUT);
  pinMode(A7, INPUT);
  pinMode(BUTTON, INPUT);
  pinMode(PUMP, OUTPUT);
  pinMode(VALVE, OUTPUT);
  
  digitalWrite(PUMP, LOW);
  digitalWrite(VALVE, LOW);
}

// the loop function runs over and over again forever
void loop() {
  readPressure();
  Serial.println(pAvg);
  if(pAvg <= pMIN){
    digitalWrite(PUMP, HIGH);
    pumpOn = true;
  }
  if(pAvg >= pMAX){
    digitalWrite(PUMP, LOW);
    pumpOn = false;
  }
  if(!pumpOn && !digitalRead(BUTTON)){
    digitalWrite(VALVE, HIGH);
    long delayTime = millis();
    while(delayTime + OPEN_TIME >= millis()){
      digitalWrite(LED, LOW);
      delay(100);
      digitalWrite(LED, HIGH);
      delay(100);
    }
    ledFadeValue = LED_MIN;
    ledFadeDir = true;
    digitalWrite(VALVE, LOW);
  }
  else{
    digitalWrite(VALVE, LOW);
  }
  
if(!pumpOn){
  if(ledFadeValue < LED_MAX && ledFadeDir){
    ledFadeValue ++;
  }
  else if(ledFadeValue > LED_MIN && !ledFadeDir){
     ledFadeValue --;
  }
  else{
    ledFadeDir = !ledFadeDir;
  }
  analogWrite(LED, ledFadeValue);
}
else{

  if(ledFadeValue < LED_MAX){
     ledFadeValue ++;
  }
  analogWrite(LED, ledFadeValue);
}
  delay(5);                       // wait for a second
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

void readPressure(){
  float tmp = analogRead(A7)/204.6;    // turn the LED off by making the voltage LOW
  p = mapfloat(tmp,0.5,4.5,1.0,12.0);
  pAvgArr[pAvgArrIndex] = p;
  if(pAvgArrIndex >= 4){
    pAvgArrIndex = 0;
    pAvg = 0;
    for(int i = 0; i<= 4; i++){
      pAvg = pAvg + pAvgArr[i];
    }
    pAvg = pAvg / 5;
  }
  else pAvgArrIndex++;
}
