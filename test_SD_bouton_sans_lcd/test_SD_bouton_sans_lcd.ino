#include <Wire.h>
#include <SD.h>

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
const unsigned long long interval = 5ULL * 60ULL * 1000ULL; // Intervalle de 5 minutes en millisecondes

File dataFile; // Objet pour le fichier de données sur la carte SD
const int chipSelect = 10; // Broche CS (Chip Select) pour la communication avec la carte SD

void setup() {
  // Sonar
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // Bouton Start/Stop
  pinMode(startStopButtonPin, INPUT_PULLUP);

  // Carte SD
  if (!SD.begin(chipSelect)) {
    while (1);
  }

  delay(1000);
}

void loop() {
  if (!isRunning) {
    // Vérifier si le bouton Start/Stop est pressé pour démarrer le programme
    if (digitalRead(startStopButtonPin) == LOW) {
      isRunning = true;
      previousTime = millis();
      delay(1000);
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

      // Enregistrer les données sur la carte SD
      dataFile = SD.open("data.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.print(resultat, 5);
        dataFile.print(",");
        dataFile.println(currentTime/60000); // affihe le temps en minutes
        dataFile.close();
      }
    }

    // Vérifier si le bouton Start/Stop est pressé pour arrêter le programme
    if (digitalRead(startStopButtonPin) == LOW) {
      isRunning = false;
      delay(1000);
    }
  }
}
