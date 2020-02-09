# include <Servo.h>
#include <bmp3_defs.h>
#include <DFRobot_BMP388.h>
#include <DFRobot_BMP388_I2C.h>
#include <DFRobot_BMP388_SPI.h>
#include <Wire.h> // Include the Arduino SPI library
#include <rgb_lcd.h>
#include "rgb_lcd.h"

rgb_lcd lcd;
const int red = 255;
const int green = 0;
const int blue = 255;


DFRobot_BMP388_I2C bmp388;

int led1 = 13;
int led2 = 12;
int count = 0;
Servo myServo;

const byte s7sAddress = 0x71;

byte heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000
};

byte smiley[8] = {
    0b00000,
    0b00000,
    0b01010,
    0b00000,
    0b00000,
    0b10001,
    0b01110,
    0b00000
};

byte mongo[8] = {
    0b00100,
    0b01010,
    0b00100,
    0b10101,
    0b01110,
    0b00100,
    0b00100,
    0b01010
};

// the setup routine runs once when you press reset:
void setup() {
  //lcd setup
  Wire.begin();
  

  //rgb
  lcd.begin(16, 2);
  lcd.setRGB(red, green, blue);
  lcd.print("Thank You Mongo!");
  lcd.createChar(0, heart);
  lcd.createChar(1, smiley);
  lcd.createChar(2, mongo);
  lcd.setCursor(4, 1);
  lcd.write((unsigned char) 0);
  lcd.setCursor(8, 1);
  lcd.write((unsigned char) 1);
  lcd.setCursor(12, 1);
  lcd.write(2);
   
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
  String send = String(currentAngle);
  Wire.beginTransmission(s7sAddress);
  if (send.length() > 2) {
    Wire.write("0");
    for (int i = 0; i < 3; i++) {
      Wire.write(send[i]);
    }
  } else if (send.length() == 2) {
    Wire.write("00");
    for (int i = 0; i < 2; i++) {
      Wire.write(send[i]);
    }
  } else if (send.length() == 1) {
    Wire.write("000");
    Wire.write(send[0]);
  }
  Wire.endTransmission();

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
