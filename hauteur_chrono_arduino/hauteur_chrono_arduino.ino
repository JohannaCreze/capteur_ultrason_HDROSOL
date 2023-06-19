#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

//---------
//sonar
const int trig = 9;
const int echo = 8;
float lecture_echo = 0;
float distance = 0;
float resultat = 0;
int H = 18 * pow(10, -2); // hauteur du bac ici 18 cm

//--------
unsigned long previousTime = 0;
const unsigned long interval = 5 * 60 * 1000;  // toutes les 5 minutes
const unsigned long totalDuration = 5 * 60 * 60 * 1000;  // durée totale de 5 heures

// Données enregistrées
struct Mesure {
  float resultat;
  unsigned long chronometre;
};

const int nombreMesures = (totalDuration / interval);  // Nombre total de mesures
Mesure mesures[nombreMesures];
int indexMesure = 0;

void setup() {
  Serial.begin(9600);
  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);

  delay(100);
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - previousTime >= interval) {
    lcd.clear();
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    lecture_echo = pulseIn(echo, HIGH);
    hauteur = ((lecture_echo) / 2) * 0.034 * pow(10, -2);
    resultat = H - hauteur;
    
    Serial.println(resultat, 5);
    
    lcd.print("hauteur mesurée");
    lcd.setCursor(0, 1);
    lcd.print(resultat);
    
    // Enregistrement des données
    mesures[indexMesure].resultat = resultat;
    mesures[indexMesure].chronometre = currentTime;
    indexMesure++;
    
    if (indexMesure >= nombreMesures) {
      // Fin de l'enregistrement, afficher ou utiliser les données enregistrées
      
      // Exemple d'affichage des données
      for (int i = 0; i < nombreMesures; i++) {
        Serial.print("Mesure ");
        Serial.print(i+1);
        Serial.print(": Hauteur=");
        Serial.print(mesures[i].resultat, 5);
        Serial.print(", Chronomètre=");
        Serial.println(mesures[i].chronometre);
      }
      
      // Réinitialisation des variables pour une nouvelle série de mesures
      indexMesure = 0;
      memset(mesures, 0, sizeof(mesures));  // Effacer les données enregistrées
      
      // Vous pouvez également ajouter du code pour traiter les données enregistrées selon vos besoins
      
      // ...
    }
    
    previousTime = currentTime;
  }
  
  // Autres tâches à effectuer régulièrement pendant les 5 minutes (si nécessaire)
  
  //...
  
  delay(100);
}
