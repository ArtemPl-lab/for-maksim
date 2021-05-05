#include <SPI.h>                                               
#include <nRF24L01.h>                                          
#include <RF24.h>                                              
RF24     radio(8,9);                                         
byte myData;                                           
uint8_t  pipe;
void setup(){
    Serial.begin(9600);
    radio.begin();                                             
    radio.openReadingPipe (1, 0xAABBCCDD11);  
    radio.setChannel      (60);
    radio.setPALevel      (RF24_PA_MAX);
    radio.setDataRate     (RF24_250KBPS);
    radio.powerUp();
    radio.startListening  ();
}                                                            
                                                               
void loop(){                                                   
    if(radio.available(&pipe)){                               
        radio.read( &myData, sizeof(myData) );                
        if(pipe==1){ Serial.println(myData);} 
         Serial.println(myData);
    } 
}
