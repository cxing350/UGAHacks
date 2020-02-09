# include <Servo.h>

int led1 = 13;
int led2 = 12;
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
  //int sensor1[] = new int[500];
  //int sensor2[] = new int[500];
  int sensorValue1 = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  float avg1 = 0;
  float avg2 = 0;
  for(int i = 0; i < 100; i++) {
    //sensor1[i] = sensorValue1;
    //sensor2[i] = sensorValue2;

    avg1 = (float)((avg1 * i) + sensorValue1) / (i + 1);
    avg2 = (float)((avg2 * i) + sensorValue2) / (i + 1);
    
    delay(1);        // delay in between reads for stability
  }

  int currentAngle = myServo.read();

  if (avg1 > avg2) {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    if (currentAngle < 180 && abs(avg1 - avg2) > 10) {
      myServo.write(currentAngle + 1);
    }
    Serial.println(String(avg1) + " " + String(myServo.read()));
  } else {
    digitalWrite(led2, HIGH);
    digitalWrite(led1, LOW);
    if (currentAngle > 0 && abs(avg1 - avg2) > 10) {
      myServo.write(currentAngle - 1);
    }
    Serial.println(String(avg2) + " " + String(myServo.read()));
  }

  //send bigger avg and angle
}
