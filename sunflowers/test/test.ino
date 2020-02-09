#include <bmp3_defs.h>
#include <DFRobot_BMP388.h>
#include <DFRobot_BMP388_I2C.h>
#include <DFRobot_BMP388_SPI.h>

DFRobot_BMP388_I2C bmp388;


void setup() {
  Serial.begin(9600);
  bmp388.set_iic_addr(BMP3_I2C_ADDR_SEC);

  while(bmp388.begin()){
    Serial.println("Initialize error!");
    delay(1000);
  }
}

void loop() {
  float pressure = bmp388.readPressure();
  Serial.println(pressure);
  delay(1000);

}
