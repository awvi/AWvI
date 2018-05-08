#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <wiringPi.h>
#include "nextion.h"

#define MAX_TIMINGS 85
#define DHT_PIN 7
int data[5] = { 0, 0, 0, 0, 0 };

void read_dht_data (void)
{
    uint8_t laststate = HIGH;
    uint8_t counter = 0;
    uint8_t j = 0;
    uint8_t i;
    data[0] = data[1] = data[2] = data[3] = data[4] = 0;
    char hum[6];
    int tempd;
    int man;
    int temp;

    /* pull pin down for 18 milliseconds */
    pinMode( DHT_PIN, OUTPUT );
    digitalWrite( DHT_PIN, LOW );
    delay( 18 );

    /* prepare to read the pin */
    pinMode( DHT_PIN, INPUT );
    /* prepare to read the pin 
    pinMode( DHT_PIN, INPUT );*/

    /* detect change and read data */
    for ( i = 0; i < MAX_TIMINGS; i++ )
    {
        counter = 0;
        while ( digitalRead( DHT_PIN ) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = digitalRead( DHT_PIN );

        if ( counter == 255 )
            break;

        /* ignore first 3 transitions */
        if ( (i >= 4) && (i % 2 == 0) )
        {
            /* shove each bit into the storage bytes */
            data[j / 8] <<= 1;
            if ( counter > 16 )
                data[j / 8] |= 1;
            j++;
        }
    }
    /*
     * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
     * print it out if data is good*/

     if ( (j >= 40) &&
          (data[4] == ( (data[0] + data[1] + data[2] + data[3]) & 0xFF) ) )
    {

        man = (data[0] << 8) + data[1];  
        temp = man/10;  
        if ( man > 1000 )
        {
           temp = data[0]; /*for DHT11*/
           man = 0;
        }
        tempd = man % 10;
        sprintf(hum, "%d.%d", temp, tempd);
        printf("Humidity = %s\n",hum);
        NexSendTxt("hum", hum);

        man = ((data[2] & 0x7F) << 8) + data[3];
        temp = man/10; 
        if ( man > 1250 )
        {
            temp = data[2]; /* for DHT11*/
            man = 0;
        }
        tempd = man%10;
        if ( data[2] & 0x80 )
        {
            sprintf(hum, "-%d.%d", temp, tempd);
        }
        else
        {
            sprintf(hum, "%d.%d", temp, tempd);
        }
        printf("Temp = %s\n",hum);
        NexSendTxt("temp", hum);
    }
    else 
    {
        printf( "Data not good, skip\n" );
    }
}
