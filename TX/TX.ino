#include <Wire.h>
#include <SPI.h>
#include <OneWire.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Adafruit_BMP280.h>
#include <ADXL345.h>
#include <TroykaGPS.h>
ADXL345 accelerometer;
Adafruit_BMP280 bmp280;
RF24 radio(PE3,PE2);
OneWire ds(15);
#define GPS_SERIAL    Serial1
GPS gps(GPS_SERIAL);
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};
byte counter;
float mass[8] = {0,0,0,0,0,0,0,0};
String tmp2 = "";
#define MAX_SIZE_MASS 16
// массив для хранения текущего времени
char strTime[MAX_SIZE_MASS];
// массив для хранения текущей даты
char strDate[MAX_SIZE_MASS];
// массив для хранения широты в градусах, минутах и секундах
char latitudeBase60[MAX_SIZE_MASS];
// массив для хранения долготы в градусах, минутах и секундах
char longitudeBase60[MAX_SIZE_MASS];
void showRange(void)
{
  
  switch (accelerometer.getRange())
  {
    case ADXL345_RANGE_16G: Serial.println("+/- 16 g"); break;
    case ADXL345_RANGE_8G:  Serial.println("+/- 8 g");  break;
    case ADXL345_RANGE_4G:  Serial.println("+/- 4 g");  break;
    case ADXL345_RANGE_2G:  Serial.println("+/- 2 g");  break;
    default: Serial.println("Bad range"); break;
  }
}
void showDataRate(void)
{
 
  switch (accelerometer.getDataRate())
  {
    case ADXL345_DATARATE_3200HZ: Serial.println("3200 Hz"); break;
    case ADXL345_DATARATE_1600HZ: Serial.println("1600 Hz"); break;
    case ADXL345_DATARATE_800HZ:  Serial.println("800 Hz");  break;
    case ADXL345_DATARATE_400HZ:  Serial.println("400 Hz");  break;
    case ADXL345_DATARATE_200HZ:  Serial.println("200 Hz");  break;
    case ADXL345_DATARATE_100HZ:  Serial.println("100 Hz");  break;
    case ADXL345_DATARATE_50HZ:   Serial.println("50 Hz");   break;
    case ADXL345_DATARATE_25HZ:   Serial.println("25 Hz");   break;
    case ADXL345_DATARATE_12_5HZ: Serial.println("12.5 Hz"); break;
    case ADXL345_DATARATE_6_25HZ: Serial.println("6.25 Hz"); break;
    case ADXL345_DATARATE_3_13HZ: Serial.println("3.13 Hz"); break;
    case ADXL345_DATARATE_1_56HZ: Serial.println("1.56 Hz"); break;
    case ADXL345_DATARATE_0_78HZ: Serial.println("0.78 Hz"); break;
    case ADXL345_DATARATE_0_39HZ: Serial.println("0.39 Hz"); break;
    case ADXL345_DATARATE_0_20HZ: Serial.println("0.20 Hz"); break;
    case ADXL345_DATARATE_0_10HZ: Serial.println("0.10 Hz"); break;
    default: Serial.println("Bad data rate"); break;
  }
}
void setup() {
  Serial.begin(115200);
  radio.begin();
  radio.openWritingPipe(0xAABBCCDD11);
  radio.setChannel(60);
  radio.setPALevel (RF24_PA_MAX); 
  radio.setDataRate (RF24_250KBPS);
  radio.powerUp();
  radio.stopListening();
  while (!bmp280.begin(BMP280_ADDRESS - 1)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    delay(2000);
  }
  
  if (!accelerometer.begin())
  {
    delay(500);
  }
  accelerometer.setRange(ADXL345_RANGE_16G);
  showRange();
  showDataRate();
  GPS_SERIAL.begin(115200);
}
void loop() {
  float temperature = bmp280.readTemperature();
  float pressure = bmp280.readPressure();
  float altitude = bmp280.readAltitude(1013.25);
  Vector raw = accelerometer.readRaw();
  Vector norm = accelerometer.readNormalize();
  byte data[2];
  ds.reset();
  ds.write(0xCC);
  ds.write(0x44);
  delay(1000);
  ds.reset();
  ds.write(0xCC);
  ds.write(0xBE);
  data[0] = ds.read();
  data[1] = ds.read();
  float temperature2 =  ((data[1] << 8) | data[0]) * 0.0625;
  mass[0] = temperature;
  mass[1] = pressure;
  mass[2] = norm.XAxis;
  mass[3] = norm.YAxis;
  mass[4] = norm.ZAxis;
  if (gps.available()) {
    gps.readParsing();
    switch(gps.getState()) {
      case GPS_OK:
        mass[5] = gps.getLatitudeBase10();
        mass[6] = gps.getLongitudeBase10();
        mass[7] = gps.getAltitude();
      case GPS_ERROR_DATA:
        mass[5] = -1;
        mass[6] = -1;
        mass[7] = -1;
        break;
      // нет соединение со спутниками
      case GPS_ERROR_SAT:
        mass[5] = -2;
        mass[6] = -2;
        mass[7] = -2;
        break;
    }
  }
  radio.write(&mass, sizeof(mass));
  counter++;
  for(int i = 0; i < 8; i++){
    tmp2+=String(mass[i]);
    tmp2+=" ";
  }
  Serial.println(tmp2);
  tmp2 = "";
  delay(1000);
}
