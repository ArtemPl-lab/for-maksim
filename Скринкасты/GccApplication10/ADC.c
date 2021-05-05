#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include "ADC.h"

// Voltage Reference: AVCC pin
#define ADC_VREF_TYPE ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))

//прочитать результат преобразования АЦП из канала номер adc_input
uint16_t adc_read(uint8_t adc_input)
{
    ADMUX=adc_input | ADC_VREF_TYPE;//записываем номер канала
    // пока стабилизируется входное напряжение на АЦП
    _delay_us(10);
    // Запускаем преобразование
    ADCSRA|=(1<<ADSC);
    // Ждем пока преобразование закончится
    while ((ADCSRA & (1<<ADIF))==0);
    ADCSRA|=(1<<ADIF);
    return ADCW;
}

//Инициализация АЦП
void adc_init()
{
    //  Предделитель 64 для 16MHz  - 250 000 Hz
    // АЦП референсный пин: AVCC pin

    ADMUX=ADC_VREF_TYPE;
    ADCSRA = (1<<ADEN) |(0<<ADSC) |(0<<ADIF)|(0<<ADIE)| (1<<ADPS2) |(1<<ADPS1) |(0<<ADPS0);
    //
    adc_read(0); //"прогреваем" АЦП, первое преобразование самое долгое
}
