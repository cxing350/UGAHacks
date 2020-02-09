# include <Servo.h>

int led1 = 13;
int led2 = 12;
int count = 0;
Servo myServo;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  myServo.attach(9);

  while(Serial.available()== 0) {
    //delay(1);
  }
}

// the loop routine runs over and over again forever:
void loop() {
  int sensorValue1 = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  float avg1 = 0;
  float avg2 = 0;
  for(int i = 0; i < 100; i++) {
    avg1 = (float)((avg1 * i) + sensorValue1) / (i + 1);
    avg2 = (float)((avg2 * i) + sensorValue2) / (i + 1);
    
    delay(1);        // delay in between reads for stability
  }

  int currentAngle = myServo.read();

  if (avg1 > avg2) {
    if (currentAngle < 180 && abs(avg1 - avg2) > 20) {
      myServo.write(currentAngle + 1);
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
    } else {
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
    }
    if (count >= 20) {
      Serial.println(String(avg1) + " " + String(myServo.read()));
      count = 0;
    }
  } else {
    digitalWrite(led2, HIGH);
    digitalWrite(led1, LOW);
    if (currentAngle > 0 && abs(avg1 - avg2) > 20) {
      myServo.write(currentAngle - 1);
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
    } else {
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
    }
    if (count >= 20) {
      Serial.println(String(avg2) + " " + String(myServo.read()));
      count = 0;
    }
  }
  count++;
}
