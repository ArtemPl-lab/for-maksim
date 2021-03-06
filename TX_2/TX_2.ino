
#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"
RF24 radio(PE3,PE2);
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};
byte counter;
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(0xAABBCCDD11);
  radio.setChannel(60);
  radio.setPALevel (RF24_PA_MAX); 
  radio.setDataRate (RF24_250KBPS);
  radio.powerUp();
  radio.stopListening();
}
void loop() {
  Serial.print("Sent: ");
  Serial.println(counter);
  radio.write(&counter, sizeof(counter));
  counter++;
  delay(10);
}
