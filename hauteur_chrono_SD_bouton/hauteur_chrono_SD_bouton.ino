#include <Wire.h>
#include "rgb_lcd.h"
#include <SD.h>

rgb_lcd lcd;
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

//---------
// Sonar
const int trig = 9;
const int echo = 8;
float lecture_echo = 0;
float hauteur = 0;
float resultat = 0;
int H = 18 * pow(10, -2); // Hauteur du bac (18 cm)

//-------
const int startStopButtonPin = 2; // Broche du bouton Start/Stop
boolean isRunning = false;        // Indicateur pour le programme en cours d'exécution
unsigned long previousTime = 0;
const unsigned long interval = 5 * 60 * 1000; // Intervalle de 5 minutes en millisecondes

File dataFile; // Objet pour le fichier de données sur la carte SD
const int chipSelect = 10; // Broche CS (Chip Select) pour la communication avec la carte SD

void setup() {
  // Sonar
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Bouton Start/Stop
  pinMode(startStopButtonPin, INPUT_PULLUP);

  // LCD
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);

  // Carte SD
  if (!SD.begin(chipSelect)) {
    lcd.print("Erreur SD");
    while (1);
  }

  lcd.print("SD OK");
  delay(1000);
  lcd.clear();
}

void loop() {
  if (!isRunning) {
    // Vérifier si le bouton Start/Stop est pressé pour démarrer le programme
    if (digitalRead(startStopButtonPin) == LOW) {
      isRunning = true;
      previousTime = millis();
      lcd.print("Programme demarre");
      delay(1000);
      lcd.clear();
    }
  } else {
    unsigned long currentTime = millis();
    if (currentTime - previousTime >= interval) {
      previousTime = currentTime;

      // Mesurer la hauteur avec le sonar
      digitalWrite(trig, HIGH);
      delayMicroseconds(10);
      digitalWrite(trig, LOW);
      lecture_echo = pulseIn(echo, HIGH);
      hauteur = ((lecture_echo) / 2) * 0.034 * pow(10, -2);
      resultat = H - hauteur;

      // Afficher la hauteur sur l'écran LCD
      lcd.clear();
      lcd.print("Hauteur mesuree");
      lcd.setCursor(0, 1);
      lcd.print(resultat);

      // Enregistrer les données sur la carte SD
      dataFile = SD.open("donnees.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.print(resultat, 5);
        dataFile.print(",");
        dataFile.println(currentTime);
        dataFile.close();
      } else {
        lcd.clear();
        lcd.print("Erreur enregistrement");
        delay(2000);
        lcd.clear();
      }
    }

    // Vérifier si le bouton Start/Stop est pressé pour arrêter le programme
    if (digitalRead(startStopButtonPin) == LOW) {
      isRunning = false;
      lcd.print("Programme arrete");
      delay(1000);
      lcd.clear();
    }
  }
}

