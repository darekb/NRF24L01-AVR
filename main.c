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

struct MEASURE {
    int16_t   temperature;
    uint16_t  humidity;
    uint16_t  pressure;
    uint8_t   voltage;
    uint8_t   sensorId;
} BME180measure;

char dataIn[] = "Start";
char dataOut[] = "";
uint8_t test = 0;

int main(void) {
    slUART_Init();
    slUART_WriteString("start\n");
    // slUART_WriteString("Size of BME180measure: ");
    // slUART_LogDec(sizeof(BME180measure));
    // slUART_WriteStringNl(" bytes");
    // slUART_WriteString("Size of string; -13.34|68.35|101549|11|z: ");
    // slUART_LogDec(sizeof("-13.34|68.35|101549|11|z"));
    // slUART_WriteStringNl(" bytes");

    // test = 0xff;
    // test &= ~(1<<SPR1) & ~(1<<SPR0);
    // slUART_WriteString("&= ~(1<<SPR1) & ~(1<<SPR0): ");
    // slUART_LogBinary(test);
    // test = 0xf0;
    // test &= ~(1<<SPR1) & ~(1<<SPR0);
    // slUART_WriteString("&= ~(1<<SPR1) & ~(1<<SPR0): ");
    // slUART_LogBinary(test);

    // test = 0xff;
    // test |= (1<<SPR0); test &= ~(1<<SPR1);
    // slUART_WriteString("|= (1<<SPR0); test &= ~(1<<SPR1); ");
    // slUART_LogBinary(test);
    // test = 0xf0;
    // test |= (1<<SPR0); test &= ~(1<<SPR1);
    // slUART_WriteString("|= (1<<SPR0); test &= ~(1<<SPR1); ");
    // slUART_LogBinary(test);

    // test = 0xff;
    // test |= (1<<SPR1); test &= ~(1<<SPR0);
    // slUART_WriteString("|= (1<<SPR1); test &= ~(1<<SPR0); ");
    // slUART_LogBinary(test);
    // test = 0xf0;
    // test |= (1<<SPR1); test &= ~(1<<SPR0);
    // slUART_WriteString("|= (1<<SPR1); test &= ~(1<<SPR0); ");
    // slUART_LogBinary(test);


    // test = 0xff;
    // test |= (1<<SPR1) | (1<<SPR0);
    // slUART_WriteString("|= (1<<SPR1) | (1<<SPR0): ");
    // slUART_LogBinary(test);
    // test = 0xf0;
    // test |= (1<<SPR1) | (1<<SPR0);
    // slUART_WriteString("|= (1<<SPR1) | (1<<SPR0): ");
    // slUART_LogBinary(test);
    slUART_WriteString("SPCR: ");
    slUART_LogBinaryNl(SPCR);
    slUART_WriteString("SPSR: ");
    slUART_LogBinaryNl(SPSR);
    slNRF_Init();
    slUART_WriteString("SPCR: ");
    slUART_LogBinaryNl(SPCR);
    slUART_WriteString("SPSR: ");
    slUART_LogBinaryNl(SPSR);
    slNRF_SetRXPayload(0, 3);
    slNRF_GetAddress(7, 1);
    slNRF_BitWrite(0, 0, 1);//register#, bit#, and value 0 or 1, ::  0,0,1 RX Mode
    slNRF_BitWrite(0, 1, 1);//register, bit, and value 0,1,1 PowerUP
    slNRF_BitWrite(0, 4, 1);//RT Mask turns off the RT interrupt
    slNRF_BitWrite(0, 5, 1);//TX Mask turns off the TX interrupt
    CSN_LOW();
    dataIn[0] = slSPI_TransferByte(0xE2);//flush RX
    slUART_WriteString("flush RX  dataIn[0]: ");
    slUART_LogBinaryNl(dataIn[0]);
    CSN_HIGH();
    CSN_LOW();
    dataIn[0] = slSPI_TransferByte(0xE1);//flush TX
    slUART_WriteString("flush TX  dataIn[0]: ");
    slUART_LogBinaryNl(dataIn[0]);
    CSN_HIGH();
    slNRF_GetAddress(0, 1);
    slNRF_GetAddress(7, 1);
    while(1){

    }
    return 0;
}
