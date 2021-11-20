
int critical_zone = 30;
int critical_zone_buzzer = 4;

int warning_zone = 40;
int warning_zone_Led = 5;

#define alarm_time 2000;
int temp_alrm_time = alarm_time;
// temp_alrm_time = alarm_time;
#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 // attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
long duration; // variable for the duration of sound wave travel
int distance;  // variable for the distance measurement

void setup() {
  pinMode(warning_zone_Led, OUTPUT);
  pinMode(critical_zone_buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);  // Sets the echoPin as an INPUT
  Serial.begin(
      9600); // // Serial Communication is starting with 9600 of baudrate speed
  Serial.println(
      "Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
}
void loop() {
  // Clears the trigPin condition
  update_distance();
  Serial.print("Distance: ");
  Serial.print(distance / 2.54);
  Serial.println(" inches");
  check_warning_distance();
  check_critical_distance();
  temp_alrm_time = alarm_time;
}
void update_distance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Displays the distance on the Serial Monitor
}
void check_critical_distance() {
  update_distance();
  if (distance < critical_zone) {
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
  for (; temp_alrm_time > 0; temp_alrm_time -= 400) {
    digitalWrite(critical_zone_buzzer, HIGH);
    delay(200);
    digitalWrite(critical_zone_buzzer, LOW);
    delay(200);
  }
}