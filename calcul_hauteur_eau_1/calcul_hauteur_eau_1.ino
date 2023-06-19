//calcul du hauteur avec un capteur ultrason en cm
//tableau lcd
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;


//---------
//sonar
const int trig=9;
const int echo=8;
float lecture_echo=0;//valeur mesurée du temps
float distance = 0;
float resultat = 0;
int H = 18*pow(10,-2); //hauteur du bac ici 18 cm


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
  //lcd.print("hello, world!");
  
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  //sonar
  lcd.clear();
  
  digitalWrite(resultat,HIGH);
  delayMicroseconds(10);
  digitalWrite(resultat,LOW);
  lecture_echo=pulseIn(echo,HIGH);
  //delay(600);
  hauteur = ((lecture_echo)/2)*0.034*pow(10,-2); //convertion en metre
  resultat=H-hauteur
  Serial.println(resultat,5);
  //tableau
  lcd.print("hauteur mesurée");
  lcd.setCursor(0, 1);
  lcd.print(resultat);
  delay(600);
  
}