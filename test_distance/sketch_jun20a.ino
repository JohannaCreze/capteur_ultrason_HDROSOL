// test 

// Sonar
const int trig = 9;
const int echo = 8;
float lecture_echo = 0;

void setup() {
  // Sonar
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  delay(100);
}

void loop() {
  // Sonar
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  lecture_echo = pulseIn(echo, HIGH);
  lecture_echo = (lecture_echo / 2) * 0.00034; // Calcul en mètre
  Serial.println(lecture_echo);

  delay(600);
}































