# include <Servo.h>
#include <bmp3_defs.h>
#include <DFRobot_BMP388.h>
#include <DFRobot_BMP388_I2C.h>
#include <DFRobot_BMP388_SPI.h>

DFRobot_BMP388_I2C bmp388;

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

  bmp388.set_iic_addr(BMP3_I2C_ADDR_SEC);
  while(bmp388.begin()){
    Serial.println("Initialize error!");
    delay(1000);
  }

  while(Serial.available()== 0) {
    //delay(1);
  }
}

// the loop routine runs over and over again forever:
void loop() {
  float pressure = bmp388.readPressure();
  float temperature = bmp388.readTemperature();
  float altitude = bmp388.readAltitude();
  
  int sensorValue1 = analogRead(A0);
  int sensorValue2 = analogRead(A1);
  float avg1 = 0;
  float avg2 = 0;
  float greater = 0;
  for(int i = 0; i < 100; i++) {
    avg1 = (float)((avg1 * i) + sensorValue1) / (i + 1);
    avg2 = (float)((avg2 * i) + sensorValue2) / (i + 1);
    
    delay(1);        // delay in between reads for stability
  }

  int currentAngle = myServo.read();

  if (avg1 > avg2) {
    greater = avg1;
    if (currentAngle < 180 && abs(avg1 - avg2) > 20) { //if angle is within bounds and difference in readings high enough
      myServo.write(currentAngle + 1);
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
    } else { //reached max angle or not high enough difference
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
    }
  } else {
    greater = avg2;
    if (currentAngle > 0 && abs(avg1 - avg2) > 20) {
      myServo.write(currentAngle - 1);
      digitalWrite(led1, HIGH);
      digitalWrite(led2, LOW);
    } else {
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
    }
  }
  if (count >= 20) {
    Serial.println(String(greater) + " " + String(myServo.read()) + " " + String(pressure) + " " + String(temperature) + " " + String(altitude));
    count = 0;
  }
  count++;
}
