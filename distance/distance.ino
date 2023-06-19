//tableau lcd
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 0;
const int colorG = 255;
const int colorB = 0;


//---------
//sonar
const int trig=9;
const int echo=8;
float lecture_echo=0;

//-------
void setup() {
  // put your setup code here, to run once:
  //sonar
  Serial.begin(9600);
  pinMode(trig,OUTPUT);
  pinMode(echo,INPUT);
  
  //------
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);

  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  //sonar
  lcd.clear();
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  lecture_echo=pulseIn(echo,HIGH);
  //delay(600);
  lecture_echo=((lecture_echo)/2)*0.00034;//calcul en mêtre
  Serial.println(lecture_echo);

  //tableau
  lcd.print(lecture_echo);
  delay(600);
  
}