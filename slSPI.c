//
// Created by dariusz on 20.02.17.
//
#include <avr/io.h>
#include "slSPI.h"
//#include "slUart.h"

void slSPI_Init() {
    //Set MOSI and SCK output, all others input
    DDR_SPI |= (1 << DD_MOSI) | (1 << DD_SCK) | (1 << DD_SS);

    //set AVR as master
    slSPI_SetMaster();

    //enable spi interface
    slSPI_Enable();
//
//    //Mode 0 Rising edge of data, keep clock low
//    slSPI_SetMode0();
//
//    //Run the data in at 16MHz/2 - 8MHz
//    slSPI_SetClockDiv2();
//
//    //SPI Most Significant Bit First
//    slSPI_SetMsb();
}

uint8_t slSPI_TransferByte(char data) {
    /* Start transmission */
//    slUART_WriteString("slSPI_TransferByte data: ");
//    slUART_LogBinary(data);
    SPDR = data;
    asm volatile("nop");
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}

void slSPI_WriteString(const char myString[], char *buffOut) {
    uint8_t i = 0;
    while (myString[i]) {
        buffOut[i] = slSPI_TransferByte(myString[i]);
        i++;
    }
}