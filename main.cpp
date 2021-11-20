// ---------------------------------------------------------------- //
// Arduino Ultrasoninc Sensor HC-SR04
// Re-writed by Arbi Abdul Jabbaar
// Using Arduino IDE 1.8.7
// Using HC-SR04 Module
// Tested on 17 September 2019
// ---------------------------------------------------------------- //

int critical_zone = 15;
int critical_zone_buzzer = 4;

int warning_zone = 30;
int warning_zone_Led = 5;

#define warn_for 2000; // 2 seconds
#define beep_for 2000; // 2 seconds

#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

void setup() {
   pinMode(warning_zone_Led, OUTPUT);
  pinMode(critical_zone_buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}
void loop() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance/2.54);
  Serial.println(" inches");
  
  if ((distance / 2.54) < warning_zone) {
    Serial.println("If triggerd");
    warning();
    if (distance < critical_zone) {
      beep();
    }
  }
}
void warning() {
  int timer = warn_for;
  for (; timer > 0; timer -= 400) {
    digitalWrite(warning_zone_Led, HIGH);
    delay(200);
    digitalWrite(warning_zone_Led, LOW);
    delay(200);
  }
}
void beep() {
  int timer = beep_for;
  for (; timer > 0; timer -= 700) {
    digitalWrite(critical_zone_buzzer, HIGH);
    delay(500);
    digitalWrite(critical_zone_buzzer, LOW);
    delay(200);
  }
}