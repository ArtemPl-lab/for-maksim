// библиотека для работы с GPS устройством
#include <TroykaGPS.h>

// Serial1-порт к которому подключён GPS-модуль
// создаём объект класса GPS и передаём в него объект Serial11 
GPS gps(Serial);

// задаём размер массива для времени, даты, широты и долготы
#define MAX_SIZE_MASS 16
// массив для хранения текущего времени
char strTime[MAX_SIZE_MASS];
// массив для хранения текущей даты
char strDate[MAX_SIZE_MASS];
// массив для хранения широты в градусах, минутах и секундах
char latitudeBase60[MAX_SIZE_MASS];
// массив для хранения долготы в градусах, минутах и секундах
char longitudeBase60[MAX_SIZE_MASS];

void setup()
{ 
  // открываем последовательный порт для мониторинга действий в программе
  // и передаём скорость 115200 бод
  Serial1.begin(115200);
  // ждём, пока не откроется монитор последовательного порта
  // для того, чтобы отследить все события в программе
  Serial1.print("Serial1 init OK\r\n");
  // открываем Serial1-соединение с GPS-модулем
  Serial.begin(115200);
}
 
void loop()
{
  // если пришли данные с GPS-модуля
  if (gps.available()) {
    Serial1.println("GPS avalible!");
    gps.readParsing();
    // проверяем состояние GPS-модуля
    switch(gps.getState()) {
      // всё OK
      case GPS_OK:
        Serial1.println("GPS is OK");
        // выводим координаты широты и долготы
        // 1. в градусах, минутах и секундах
        // 2. градусах в виде десятичной дроби
        Serial1.println("GPS Coordinates: ");
        gps.getLatitudeBase60(latitudeBase60, MAX_SIZE_MASS);
        gps.getLongitudeBase60(longitudeBase60, MAX_SIZE_MASS);
        Serial1.print("Latitude\t");
        Serial1.print(latitudeBase60);
        Serial1.print("\t\t");
        Serial1.println(gps.getLatitudeBase10(), 6);
        Serial1.print("Longitude\t");
        Serial1.print(longitudeBase60);
        Serial1.print("\t\t");
        Serial1.println(gps.getLongitudeBase10(), 6);
        // выводим количество видимых спутников
        Serial1.print("Sat: ");
        Serial1.println(gps.getSat());
        // выводим текущую скорость
        Serial1.print("Speed: ");
        Serial1.println(gps.getSpeedKm());
        // выводим высоту над уровнем моря
        Serial1.print("Altitude: ");
        Serial1.println(gps.getAltitude());
        // выводим текущее время
        Serial1.print("Time: ");
        gps.getTime(strTime, MAX_SIZE_MASS);
        gps.getDate(strDate, MAX_SIZE_MASS);
        Serial1.write(strTime);
        Serial1.println();
        // выводим текущую дату
        Serial1.print("Date: ");
        Serial1.write(strDate);
        Serial1.println("\r\n");
        // каждую переменную дату и времени можно выводить отдельно
  /*    Serial1.print(gps.getHour());
        Serial1.print(gps.getMinute());
        Serial1.print(gps.getSecond());
        Serial1.print(gps.getDay());
        Serial1.print(gps.getMonth());
        Serial1.print(gps.getYear());
  */   
        break;
      // ошибка данных
      case GPS_ERROR_DATA:
        Serial1.println("GPS error data");
        break;
      // нет соединение со спутниками
      case GPS_ERROR_SAT:
        Serial1.println("GPS no connect to satellites!!!");
        break;
    }
  }
  Serial1.println("Err");
}
