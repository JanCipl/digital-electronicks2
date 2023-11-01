// https://github.com/Sylaina/oled-display

#include <oled.h>
#include <twi.h>      
#include "timer.h"  

volatile uint8_t new_sensor_data = 0;
struct DHT_values_structure {
   uint8_t hum_int;
   uint8_t hum_dec;
   uint8_t temp_int;
   uint8_t temp_dec;
   uint8_t checksum;
} dht12;


// Slave and internal addresses of temperature/humidity sensor DHT12
#define SENSOR_ADR 0x5c
#define SENSOR_HUM_MEM 0
#define SENSOR_TEMP_MEM 2
#define SENSOR_CHECKSUM 4

int main(void)
{

  char string[2];  // String for converting numbers by itoa()

    oled_init(OLED_DISP_ON);
    oled_clrscr();

    oled_charMode(DOUBLESIZE);
    oled_puts("OLED disp.");

    oled_charMode(NORMALSIZE);

    // oled_gotoxy(x, y)


    // oled_drawLine(x1, y1, x2, y2, color)

    oled_drawLine(0, 25, 120, 25, WHITE);


     TIM1_OVF_1SEC
    TIM1_OVF_ENABLE

    
    // Copy buffer to display RAM
    oled_display();


    while (1) {
           if (new_sensor_data == 1) {
            itoa(dht12.temp_int, string, 10);
            oled_init(OLED_DISP_ON);
            oled_clrscr();
            oled_charMode(NORMALSIZE);
            oled_gotoxy(0, 2);
            oled_puts(string);
            oled_puts(".");
            itoa(dht12.temp_dec, string, 10);
            oled_puts(string);
            oled_puts(" °C\r\n");

            itoa(dht12.hum_int, string, 10);
            oled_charMode(NORMALSIZE);
            oled_gotoxy(0, 4);
            oled_puts(string);
            oled_puts(".");
            itoa(dht12.hum_dec, string, 10);
            oled_puts(string);
            oled_puts(" °%\r\n");

            // Do not print it again and wait for the new data
            new_sensor_data = 0;
        }
        }
    return 0;
}

ISR(TIMER1_OVF_vect)
{
    // Test ACK from sensor
    twi_start();
    if (twi_write((SENSOR_ADR<<1) | TWI_WRITE) == 0) {
        // Set internal memory location
        twi_write(SENSOR_TEMP_MEM);
        twi_stop();
        // Read data from internal memory
        twi_start();
        twi_write((SENSOR_ADR<<1) | TWI_READ);
        dht12.temp_int = twi_read(TWI_ACK);
        dht12.temp_dec = twi_read(TWI_ACK);

        dht12.hum_int = twi_read(TWI_ACK);
        dht12.hum_dec = twi_read(TWI_NACK);

        new_sensor_data = 1;
    }
    twi_stop();
}