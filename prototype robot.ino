#include <QTRSensors.h>
// QTR LINE SENSOR

QTRSensors qtr;

const uint8_t SensorCount = 4;
uint16_t sensorValues[SensorCount];

const uint8_t qtrPins[SensorCount] = {10, 11, 12, 13};

// ULTRASONIC SENSOR

const int trigPin = 3;
const int echoPin = 2;

long duration;
int distance;

// MOTOR DRIVER

const int ENA = 9;
const int IN1 = 8;
const int IN2 = 7;

const int IN3 = 6;
const int ENB = 5;
const int IN4 = 4;


// TCS230 COLOUR SENSOR

#define S0 A0
#define S1 A1
#define S2 A2
#define S3 A3
#define sensorOut A4
#define OE A5

int frequency;


void setup() {

  Serial.begin(9600);

  // QTR SENSOR 

  qtr.setTypeRC();
  qtr.setSensorPins(qtrPins, SensorCount);

  delay(500);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }

  digitalWrite(LED_BUILTIN, LOW);

  Serial.println("Calibration Minimum Values:");

  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.println(qtr.calibrationOn.minimum[i]);
  }

  Serial.println();

  Serial.println("Calibration Maximum Values:");

  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.println(qtr.calibrationOn.maximum[i]);
  }

  Serial.println();


  //ULTRASONIC 

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);


  //MOTORS

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(IN3, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN4, OUTPUT);


  //TCS230

  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(sensorOut, INPUT);
  pinMode(OE, OUTPUT);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  digitalWrite(OE, LOW);

  delay(1000);
}


void loop() {

  // QTR LINE SENSOR

  uint16_t position = qtr.readLineBlack(sensorValues);

  Serial.print("Sensors: ");

  for (uint8_t i = 0; i < SensorCount; i++) {
    Serial.print(sensorValues[i]);
    Serial.print("\t");
  }

  Serial.print("Position: ");
  Serial.println(position);


  //ULTRASONIC 

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");


  //MOTOR CONTROL 

  if (distance > 0 && distance <= 10) {

    moveForward(150);

  } else {

    stopMotors();

  }


  //RED

  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  frequency = pulseIn(sensorOut, LOW);

  Serial.print("R = ");
  Serial.print(frequency);
  Serial.print("  ");

  delay(100);


  // GREEN 

  digitalWrite(S2, HIGH);
  digitalWrite(S3, HIGH);

  frequency = pulseIn(sensorOut, LOW);

  Serial.print("G = ");
  Serial.print(frequency);
  Serial.print("  ");

  delay(100);


  //BLUE 

  digitalWrite(S2, LOW);
  digitalWrite(S3, HIGH);

  frequency = pulseIn(sensorOut, LOW);

  Serial.print("B = ");
  Serial.println(frequency);

  delay(250);
}


// MOTOR FUNCTIONS

void moveForward(int speedValue) {

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, speedValue);
  analogWrite(ENB, speedValue);
}


void stopMotors() {

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
