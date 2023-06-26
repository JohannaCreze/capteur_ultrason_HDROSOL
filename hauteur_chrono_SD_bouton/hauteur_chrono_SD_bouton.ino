#include <Wire.h> // importation bibliothèque
#include <SD.h>  // importation bilio SD
#include <LiquidCrystal_I2C.h>  // importation biblio LCD

LiquidCrystal_I2C lcd(0x27, 16, 2); // Adresse I2C de l'écran LCD et dimensions du LCD (16 colonnes x 2 lignes)

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

//---------

// Sonar
const int trigPin = 9;  // Broche de déclenchement du sonar
const int echoPin = 8;  // Broche de réception du sonar
float lecture_echo = 0;
float hauteur = 0;
float resultat = 0;
float H = 18; // Hauteur du bac (18 cm)

//-------
const int startStopButtonPin = 2; // Broche du bouton Start/Stop
boolean isRunning = false;        // Indicateur pour le programme en cours d'exécution
unsigned long previousTime = 0;
const unsigned long interval = 5 *60* 1000; // Intervalle de 5 minutes en millisecondes

File dataFile; // Objet pour le fichier de données sur la carte SD
const int chipSelect = 10; // Broche CS (Chip Select) pour la communication avec la carte SD

void setup() {
  // Sonar
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Bouton Start/Stop
  pinMode(startStopButtonPin, INPUT_PULLUP);

  // Carte SD
  if (!SD.begin(chipSelect)) {
    while (1);
  }

  // Écran LCD
  lcd.init();   // initialisation du LCD
  lcd.backlight();   // active le rétroéclairage lcd.noBacklight() pour le désactiver
  lcd.setCursor(0,0);   // mettre le curseur à la première ligne, première colonne
  lcd.print("Bonjour");  // afficher "Bonjour"
  delay(1000);
  lcd.setCursor(0,1);   // mettre le curseur à la deuxième ligne, première colonne
  lcd.print("HYDROSOL");   // afficher "le monde"
}

void loop() {
  if (!isRunning) {
    // Vérifier si le bouton Start/Stop est pressé pour démarrer le programme
    if (digitalRead(startStopButtonPin) == LOW) {
      isRunning = true;
      lcd.clear(); // Effacer l'écran LCD lorsque le programme commence
      previousTime = millis();
      delay(1000);
      lcd.setCursor(0, 0);
      lcd.print("debut du");
      lcd.setCursor(0, 1);
      lcd.print("programme");
    }
  } else {
    unsigned long currentTime = millis();
    if (currentTime - previousTime >= interval) {
      previousTime = currentTime;

      // Mesurer la hauteur avec le sonar
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      // Mesurer la durée de l'écho
      unsigned long duration = pulseIn(echoPin, HIGH);

      // Calculer la distance en fonction de la durée de l'écho
      float distance = duration * 0.034 / 2;  // Vitesse du son dans l'air : 343 m/s (ou 0,034 cm/µs)

      resultat = H - distance;

      // Enregistrer les données sur la carte SD
      dataFile = SD.open("data.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.print(resultat, 5); //resultat en cm
        dataFile.print(",");
        dataFile.println(currentTime/1000); // affiche le temps en secondes
        dataFile.close();
      }

      // Afficher les données sur l'écran LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("H eau:");
      lcd.print(resultat, 2); // Afficher la hauteur avec 2 décimales
      lcd.print("cm");
      lcd.setCursor(0, 1);
      lcd.print("Temps: ");
      lcd.print(currentTime/1000); // Afficher le temps en secondes
      lcd.print(" s");
    }

    // Vérifier si le bouton Start/Stop est pressé pour arrêter le programme
    if (digitalRead(startStopButtonPin) == LOW) {
      isRunning = false;
      lcd.clear(); // Effacer l'écran LCD lorsque le programme est arrêté
      lcd.setCursor(0, 0);
      lcd.print("arret du");
      lcd.setCursor(0, 1);
      lcd.print("programme");
      delay(1000);

    }
  }
}
