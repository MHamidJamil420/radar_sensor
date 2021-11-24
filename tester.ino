// servo start

#include <Servo.h>
Servo Myservo;
int pos;
bool ArraysInitialized = false;
bool warningLED = false;
bool BuzzerBeeping = true;
bool servo_Rotaion = true;
int d1[19];
int d2[19];
// servo end
int choice = 0;
int delayed = 10;
int input_timeout = 10000;
// Ultrasound start
int critical_zone = 25;
int critical_zone_buzzer = 4;

int rotation_speed_delay = 30; // angle (++ or --) after (rotation_speed)ms
// so increasing it will slow down rotation speed

int warning_zone = 50;
int warning_zone_Led = 6;

int alarm_time = 2000;
int temp_alrm_time = alarm_time;
int display_reading_after = 10; //  (180/display_reading_after) = x,(18)
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

void inputHandler(int choice) {
  Serial.println("input Handler call");
  // choice = Serial.parseInt();
  // choice = getString().toInt();
  if (choice == 1) {
    Serial.println("Changing setting....");
    Serial.println("Avaiable variable to change : ");
    Serial.println("1: critical_zone ," + String(critical_zone));
    Serial.println("2: warning_zone ," + String(warning_zone));
    Serial.println("3: alarm_time ," + String(alarm_time));
    Serial.println("4: input_timeout ," + String(input_timeout));
    Serial.println("5: rotation_speed_delay ," + String(rotation_speed_delay));
    // Serial.println("6: display_reading_after ," +
    // String(display_reading_after));

    choice = getString().toInt();
    Serial.println("we got : " + String(choice));
    if (choice == 1) {
      // Serial.println("old value : " + String(critical_zone));
      choise_handler(&critical_zone);
      // Serial.println("new value: " + String(critical_zone));
    } else if (choice == 2) {
      choise_handler(&warning_zone);
    } else if (choice == 3) {
      choise_handler(&alarm_time);
    } else if (choice == 4) {
      choise_handler(&input_timeout);
    } else if (choice == 5) {
      choise_handler(&rotation_speed_delay);
    }
    // if (choice == 1) {
    //   Serial.println("Old value of critical_zone = " + critical_zone);
    //   Serial.println("Enter new value : ");
    //   choice = getint();
    // }
  } else if (choice == 2) {
    Serial.println("Direct call...");
    Serial.println("Enter 1 for LED  ");
    Serial.println("Enter 2 for Buzzer ");
    Serial.println("Enter 3 for Servo operations ");
    choice = getString().toInt();
    if (choice == 1) {
      Serial.println("Enter 1 enable warning led blynk ");
      Serial.println("Enter 2 disable warning led blynk ");
      Serial.println("Enter 3 Turn LED on ");
      Serial.println("Enter 4 Turn LED off ");
      choice = getString().toInt();
      if (choice == 1) {
        warningLED = true;
      } else if (choice == 2) {
        warningLED = false;
      } else if (choice == 3) {
        digitalWrite(warning_zone_Led, HIGH);
      } else if (choice == 4) {
        digitalWrite(warning_zone_Led, LOW);
      }
    } else if (choice == 2) {
      Serial.println("Enter 1 to Turn Buzzer on ");
      Serial.println("Enter 2 to turn onbeeping  ");
      Serial.println("Enter 3 to force off Buzzer  ");
      choice = getString().toInt();
      if (choice == 1) {
        Serial.println("Enter delay time ");
        choice = getString().toInt();
        digitalWrite(critical_zone_buzzer, HIGH);
        delay(choice);
        digitalWrite(critical_zone_buzzer, LOW);
      } else if (choice == 2) {
        BuzzerBeeping = true;
      } else if (choice == 3) {
        BuzzerBeeping = false;
      }
    } else if (choice == 3) {
      Serial.println("Enter 1 to stop servo_Rotaion");
      Serial.println("Enter 2 to Start servo_Rotaion");
      choice = getString().toInt();
      if (choice == 1) {
        servo_Rotaion = false;
      } else if (choice == 2) {
        servo_Rotaion = true;
      }
    }
  } else if (choice == 4) {
    Serial.println("Displaying array data : ");
    int alfa = 0;
    // Serial.print("Data of Array 1");
    Serial.println("Angle , Distance 1 | Distance 2");
    for (; alfa < 19; alfa++) {
      Serial.println(
          manage_spacing(String(alfa), 3) + manage_spacing("-->", 6) +
          manage_spacing(String(d1[alfa]), 7) + manage_spacing("|", 8) +
          manage_spacing(String(d2[alfa]), 5));
    }
    Serial.println("");
  }
  choice = 0;
  Serial.println("Handler out");
}
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
  if (Serial.available() >= 1) {
    choice = Serial.parseInt();
    if (choice >= 1) {
      inputHandler(choice);
    }
  }
  servoRotation();
  // Clears the trigPin condition
  update_distance(false);

  //  check_warning_distance();
  //  check_critical_distance();2
  temp_alrm_time = alarm_time;
}
void servoRotation() {

  if (servo_Rotaion) {
    delay(300);
  }
  for (pos = 0; pos <= 180; pos++) {
    if (Serial.available() >= 1) {
      choice = Serial.parseInt();
      if (choice >= 1) {
        inputHandler(choice);
      }
    }
    if (servo_Rotaion) {
      Myservo.write(pos);
      delay(rotation_speed_delay);
    }
    if (pos % display_reading_after == 0) {
      blynk(20);
      if (servo_Rotaion) {
        Serial.print("Angle : " + String(pos) + " -> ");
      }
      update_distance(true);
    }
  }
  if (servo_Rotaion) {
    delay(300);
  }
  for (pos = 180; pos >= 0; pos--) {
    if (Serial.available() >= 1) {
      choice = Serial.parseInt();
      if (choice >= 1) {
        inputHandler(choice);
      }
    }
    if (servo_Rotaion) {
      Myservo.write(pos);
      delay(rotation_speed_delay);
    }
    if (pos % display_reading_after == 0) {
      blynk(20);
      if (servo_Rotaion) {
        Serial.print("Angle : " + String(pos) + " -> ");
      }
      update_distance(true);
    }
  }
  if (!ArraysInitialized) {
    ArraysInitialized = true;
    Serial.println("data in array is");

    int ijk = 0;
    Serial.print("D1 : ");
    for (; ijk < 18; ijk++) {
      Serial.print(String(d1[ijk]) + ",");
    }
    Serial.println("");

    ijk = 0;
    Serial.print("D2 : ");
    for (; ijk < 18; ijk++) {
      Serial.print(String(d2[ijk]) + ",");
    }
    Serial.println("");
  }
}
void update_distance(bool check) {
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

  Serial.print("D1 : ");
  Serial.print(distance / 2.54);
  Serial.print(", D2 : ");
  Serial.print(distance2 / 2.54);
  Serial.println(" in");

  if (!ArraysInitialized && check) { // initializing arrays
    // Serial.print("Angle : " + String(pos));
    // Serial.println(", index : " + String(pos / display_reading_after));
    d1[pos / display_reading_after] = (distance / 2.54);
    d2[pos / display_reading_after] = (distance2 / 2.54);
  }
}
void check_critical_distance() {
  update_distance(false);
  if ((distance / 2.54) < critical_zone) {
    Serial.print("Critical distance");
    beep();
  }
}
void check_warning_distance() {
  update_distance(false);
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
    if (BuzzerBeeping) {
      digitalWrite(critical_zone_buzzer, HIGH);
    }
    delay(50);
    digitalWrite(critical_zone_buzzer, LOW);
    delay(50);
  }
  digitalWrite(warning_zone_Led, LOW);
}
void blynk(int defined_delay) {
  digitalWrite(LED_BUILTIN,
               HIGH);             // turn the LED on (HIGH is the voltage level)
  delay(defined_delay);           // wait for a second
  digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
  // delay(1000);
  if (warningLED) {
    digitalWrite(warning_zone_Led, HIGH);
    delay(defined_delay);
    digitalWrite(warning_zone_Led, LOW);
  }
}
String getString() {
  String sdata = "";
  char ch = '0';
  bool condit = true;
  while (condit) {

    // if (Serial.available() > 0)
    // {
    ch = Serial.read(); // get the character
    delay(20);
    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
        (ch >= '0' && ch <= '9') || (ch == '=')) {
      sdata += ch;
    } else if (ch == '!') {
      Serial.println("Bypassed");
      loop();
    } else if (ch == '.') {
      // Serial.print("Sr we got ");
      // Serial.println(sdata);
      condit = false;
      delay(100);
      // FileNameLoop = sdata;
    } else if (ch == ',') {
      Serial.println("Str cleared\n");
      sdata = "";
      // Print(sdata);
      // condit = false;
      // FileNameLoop = sdata;
    }
    // }
  }
  return sdata;
}
void choise_handler(int *p) {
  Serial.print("Enter new value : ");
  int newvalue = getString().toInt();
  Serial.println(newvalue);
  Serial.print("Value Changed : " + String(*p) + "-->");
  *p = newvalue;
  Serial.println(String(*p));
  // Serial.println("new value : " + String(*p));
}
String manage_spacing(String tempstring, int allotmentSize) {
  // int strLength = tempstring.length();
  String managedString = "";
  if ((allotmentSize - tempstring.length()) % 2 == 0) {
    // _ _ ali _ _ , incase if space is even

    // allotmentSize - tempstring.length()
    // Serial.print("")
    int spaces = (allotmentSize - tempstring.length()) / 2;
    for (; spaces > 0; --spaces) {
      // Serial.print(" ");
      managedString += " ";
    }

    // Serial.print(tempstring);
    managedString += tempstring;
    spaces = (allotmentSize - tempstring.length()) / 2;
    for (; spaces > 0; --spaces) {
      // Serial.print(" ");
      managedString += " ";
    }
    // Serial.println("");
  } else if ((allotmentSize - tempstring.length()) % 2 != 0) {
    // incase if spaces are odd - - ali - - -
    int spaces = (allotmentSize - tempstring.length()) / 2;
    for (; spaces > 0; --spaces) {
      // Serial.print(" ");
      managedString += " ";
    }

    // Serial.print(tempstring);
    managedString += tempstring;

    spaces = ((allotmentSize - tempstring.length()) / 2) + 1;
    for (; spaces > 0; --spaces) {
      // Serial.print(" ");
      managedString += " ";
    }
    // Serial.println("");
  } else {
    Serial.println("Undefined error");
  }
  return managedString;
}