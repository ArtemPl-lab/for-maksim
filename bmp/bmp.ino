#include <Adafruit_BMP280.h>
 
Adafruit_BMP280 bmp280;
 
void setup() {
  Serial.begin(9600);
  Serial.println(F("BMP280"));
 
  while (!bmp280.begin(BMP280_ADDRESS - 1)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    delay(2000);
  }
}
 
void loop() {
  float temperature = bmp280.readTemperature();
  float pressure = bmp280.readPressure();
  float altitude = bmp280.readAltitude(1013.25);
 
  Serial.print(F("Temperature = "));
  Serial.print(temperature);
  Serial.println(" *C");
 
  Serial.print(F("Pressure = "));
  Serial.print(pressure);
  Serial.println(" Pa");
 
  Serial.print(F("Altitude = "));
  Serial.print(altitude);
  Serial.println(" m");
 
  Serial.println();
  delay(2000);
}
