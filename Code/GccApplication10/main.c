#define F_CPU 8000000// Clock Speed
//#define BAUD 9600L   //UART SPEED
//#define MYUBRR F_CPU/16/BAUD-1

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>
#include "uart.h"
#include "ADC.h"
#include "onewire.h"
#include "ds18x20.h"

#define MAXSENSORS 5


uint8_t gSensorIDs[MAXSENSORS][OW_ROMCODE_SIZE];

static uint8_t search_sensors(void)
{
	uint8_t i;
	uint8_t id[OW_ROMCODE_SIZE];
	uint8_t diff, nSensors;

	printf("Scanning Bus for DS18X20\n");

	ow_reset();

	nSensors = 0;

	diff = OW_SEARCH_FIRST;
	while ( diff != OW_LAST_DEVICE && nSensors < MAXSENSORS ) {
		DS18X20_find_sensor( &diff, &id[0] );

		if( diff == OW_PRESENCE_ERR ) {
			printf( "No Sensor found\n");
			break;
		}

		if( diff == OW_DATA_ERR ) {
			printf( "Bus Error\n");
			break;
		}

		for ( i=0; i < OW_ROMCODE_SIZE; i++ )
		gSensorIDs[nSensors][i] = id[i];

		nSensors++;
	}

	return nSensors;
}


void print_address(unsigned char* address) {
	printf("%.2X %.2X %.2X %.2X %.2X %.2X %.2X %.2X\n", address[0],address[1],address[2],address[3],address[4],address[5],address[6],address[7]);
}

static int USART_PutChar(char c, FILE *stream)
{
	if (c == '\n')
	USART_PutChar('\r', stream);
	USART_SendByte(c);
	return 0;
}
static FILE mystdout = FDEV_SETUP_STREAM(USART_PutChar, NULL, _FDEV_SETUP_WRITE);
int main( void )
{
	//uint16_t a0;
	uint8_t nSensors, i;
	int16_t decicelsius;
	int32_t mxRes;
	uint8_t error;
	//float v;
	USART_Init ( 9600 );
	stdout = &mystdout; // указываем, куда будет выводить printf
	//adc_init();//инициализация АЦП

	printf("\nDS18X20 1-Wire-Reader Demo\n");
	printf("-------------------------------------------\n");
	nSensors = search_sensors();
	printf( "%i DS18X20 Sensor(s) available:\n", nSensors);

	while(1)
	{
		for ( i = nSensors; i > 0; i-- ) {
			printf("Sensor# %i \n",i);
			print_address(gSensorIDs[i-1]);
			DS18X20_set_resolution(&gSensorIDs[i-1][0], DS18B20_12_BIT);

			if ( DS18X20_start_meas( DS18X20_POWER_EXTERN,&gSensorIDs[i-1][0]) == DS18X20_OK ) {
				_delay_ms( DS18B20_TCONV_12BIT );
				if (DS18X20_read_decicelsius(&gSensorIDs[i-1][0], &decicelsius)	== DS18X20_OK ) {
					printf("temp = %i\n", decicelsius );
					} else {
					printf( "CRC Error (lost connection?)" );
					error++;
				}
				if (DS18X20_read_maxres(&gSensorIDs[i-1][0], &mxRes)	== DS18X20_OK ) {
					//printf("temp_maxREs = %.4f\n", mxRes*0.0001 );
					} else {
					printf( "CRC Error (lost connection?)" );
					error++;
				}

			}
			else {
				printf( "Start meas. failed (short circuit?)\n" );
				error++;
			}
		}
		//a0 = adc_read(0);
		//v = a0*5.0/1023;
		_delay_ms(500);
		//printf("ADC = %i, Voltage= %.4f\n",a0,v);
		//_delay_ms(500);

	}
}
