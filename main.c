#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <avr/wdt.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define showDebugDataMain 0

#include "main.h"
#include "slUart.h"
#include "slSPI.h"
#include "slNRF24.h"
#include "slBME180Measure.h"

uint8_t pipe1[] = {0xF1, 0xF0, 0xF0, 0xF0, 0xE1};
uint8_t pipe2[] = {0xFe, 0xF0, 0xF0, 0xF0, 0xD2};
uint8_t data[] = {1,2,3,4,5,6,7,8};

int main(void) {
    slUART_SimpleTransmitInit();
    slNRF_Init();
    slNRF_OpenWritingPipe(pipe1,8);
    slNRF_OpenReadingPipe(pipe2, 8);
    slNRF_SetDataRate(RF24_2MBPS);
    slNRF_SetPALevel( RF24_PA_MAX ) ;
    slNRF_SetChannel(0x1);
    slNRF_EnableDynamicPayloads();
    slNRF_EnableAckPayload();
    slNRF_SetRetries(0,15);
    slNRF_AutoAck(1);
    slNRF_showDebugData();
    slNRF_PowerUp();
    slNRF_StartListening();

    while (1) {
        slNRF_StopListening();
        if(!slNRF_Sent(&data, sizeof(data))){
            slUART_WriteStringNl("Fail...");
        } else {
            slUART_WriteStringNl("Send ok");
        }
        slNRF_StartListening();
        _delay_ms(1000);
    }
    return 0;
}
