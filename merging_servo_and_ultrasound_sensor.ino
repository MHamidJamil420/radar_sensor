// servo start

#include <Servo.h>
Servo Myservo;
int pos;

// servo end

// Ultrasound start
int critical_zone = 25;
int critical_zone_buzzer = 4;
int rotation_speed_delay = 20; // angle (++ or --) after (rotation_speed)ms
// so increasing it will slow down rotation speed
int warning_zone = 50;
int warning_zone_Led = 6;

#define alarm_time 2000;
int temp_alrm_time = alarm_time;
int display_reading_after = 18; //  (180/display_reading_after) = 10,
                                //  so after 10 degree readings will be printed
#define echoPin 12              //  attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 11              // attach pin D3 Arduino to pin Trig of HC-SR04

#define echoPin2 9  //  attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin2 10 // attach pin D3 Arduino to pin Trig of HC-SR04

long duration; // variable for the duration of sound wave travel
int distance;  // variable for the distance measurement

long duration2; // variable for the duration of sound wave travel
int distance2;  // variable for the distance measurement
// ultrasound end

void setup() {
   pinMode(LED_BUILTIN, OUTPUT);
  Myservo.attach(2);
  pinMode(warning_zone_Led, OUTPUT);
  pinMode(critical_zone_buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);  // Sets the echoPin as an INPUT

  pinMode(trigPin2, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin2, INPUT);  // Sets the echoPin as an INPUT
  Serial.begin(
      9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println(
      "Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}
void loop() {
  servoRotation();
  // Clears the trigPin condition
  update_distance();

  //  check_warning_distance();
  //  check_critical_distance();2
  temp_alrm_time = alarm_time;
}
void servoRotation() {

  delay(300);
  for (pos = 0; pos <= 180; pos++) {
    Myservo.write(pos);
    delay(rotation_speed_delay);
    if (pos % display_reading_after == 0) {
      blynk(20);
      update_distance();
    }
  }
  delay(300);

  for (pos = 180; pos >= 0; pos--) {
    Myservo.write(pos);
    delay(rotation_speed_delay);
    if (pos % display_reading_after == 0) {
      blynk(20);
      update_distance();
    }
  }
}
void update_distance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  Serial.print("Distance 1 : ");
  Serial.print(distance / 2.54);
  Serial.print(", Distance 2 : ");
  Serial.print(distance2 / 2.54);
  Serial.println(" inches");
}
void check_critical_distance() {
  update_distance();
  if ((distance / 2.54) < critical_zone) {
    Serial.print("Critical distance");
    beep();
  }
}

void check_warning_distance() {
  update_distance();
  if ((distance / 2.54) < warning_zone) {
    Serial.print("warning distance");
    // int temp_alrm_time = warn_for;
    for (; temp_alrm_time > 0; temp_alrm_time -= 200) {
      digitalWrite(warning_zone_Led, HIGH);
      delay(100);
      check_critical_distance();
      digitalWrite(warning_zone_Led, LOW);
      delay(100);
      check_critical_distance();
    }
  }
}
void beep() {
  //   int temp_alrm_time = beep_for;
  digitalWrite(warning_zone_Led, HIGH);
  for (; temp_alrm_time > 0;
       temp_alrm_time -= 400) { // decrement should be 100 (50(HIGH)+50(LOW))
                                // but i use 400 to finish it earlier
    digitalWrite(critical_zone_buzzer, HIGH);
    delay(50);
    digitalWrite(critical_zone_buzzer, LOW);
    delay(50);
  }
  digitalWrite(warning_zone_Led, LOW);
}
void blynk(int defined_delay) {
   digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(defined_delay);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  // delay(1000);    
  // digitalWrite(warning_zone_Led, HIGH);
  // delay(defined_delay);
  // digitalWrite(warning_zone_Led, LOW);
}