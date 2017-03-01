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

uint8_t pipe1[] = {0xF0, 0xF0, 0xF0, 0xF0, 0xE1};
uint8_t pipe2[] = {0xF0, 0xF0, 0xF0, 0xF0, 0xD2};
uint8_t data[]  = {0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8};
uint8_t i = 0;
//uint8_t data[]  = {0xF1,0xF2,0xF3,0xF4};
uint8_t role = 1;//1 = recivier, 0 = traciever
void clearData(){
    for(uint8_t i =0; i < 8; i++){
        data[i]=0;
    };
}
int main(void) {
    slUART_SimpleTransmitInit();
    slNRF_Init();
    // CSN_LOW();
    // slSPI_TransferInt( FLUSH_RX );
    // CSN_HIGH();
    // CSN_LOW();
    // slSPI_TransferInt( FLUSH_TX );
    // CSN_HIGH();
    if(role == 1){
        slNRF_OpenWritingPipe(pipe2, 8);
        slNRF_OpenReadingPipe(pipe1, 8);
    }
    if(role == 0){
        slNRF_OpenWritingPipe(pipe1, 8);
        slNRF_OpenReadingPipe(pipe2, 8);
    }
    slNRF_SetDataRate(RF24_2MBPS);
    slNRF_SetPALevel( RF24_PA_MAX ) ;
    slNRF_SetChannel(100);
    slNRF_EnableDynamicPayloads();
    slNRF_EnableAckPayload();
    slNRF_SetRetries(0,15);
    slNRF_AutoAck(1);
    //slNRF_showDebugData();
    slNRF_PowerUp();
    slNRF_StartListening();
    if(role == 1){
        clearData();
    }
    while (1) {
        if(role == 0){
            slNRF_StopListening();
            if(!slNRF_Sent(data, sizeof(data))){
                slUART_WriteStringNl("Fail...");
            } else {
                slUART_WriteStringNl("Send ok");
            }
            slNRF_StartListening();
            _delay_ms(1000);
        }
        if(role == 1){
            if(slNRF_Available()){
                slUART_LogDec(i);
                slUART_WriteStringNl(". recive: ");
                slNRF_Recive( data, sizeof(data) );
                for(uint8_t i =0; i < 8; i++){
                    slUART_LogHexNl(data[i]);
                };
                i++;
                clearData();
            }
        }

    }
    return 0;
}
