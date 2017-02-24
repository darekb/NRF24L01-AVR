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


#define CHANNELS  126
uint8_t channel[CHANNELS];
uint8_t line;
char grey[] = " .:-=+*aRW";

void outputChannels() {
    uint8_t norm = 0;

    // find the maximal count in channel array
    for (uint8_t i = 0; i < CHANNELS; i++)
        if (channel[i] > norm) norm = channel[i];

    // now output the data
    slUART_WriteByte('|');
    for (uint8_t i = 0; i < CHANNELS; i++) {
        int pos;

        // calculate grey value position
        if (norm != 0) pos = (channel[i] * 10) / norm;
        else pos = 0;

        // boost low values
        if (pos == 0 && channel[i] > 0) pos++;

        // clamp large values
        if (pos > 9) pos = 9;

        // print it out

        slUART_WriteByte(grey[pos]);
        channel[i] = 0;
    }
    slUART_WriteByte('|');
    slUART_LogDecNl(norm);
}

void scanChannels(void) {
    disable();//disable rx
    for (uint8_t j = 0; j < 200; j++) {
        for (uint8_t i = 0; i < CHANNELS; i++) {
            // select a new channel
            setRegister(slNRF_RF_CH, (128 * i) / CHANNELS);

            // switch on RX
            setRx();

            // wait enough for RX-things to settle
            _delay_us(40);

            // this is actually the point where the RPD-flag
            // is set, when CE goes low
            disable();

            // read out RPD flag; set to 1 if
            // received power > -64dBm
            if (getRegister(slNRF_RPD, 0) > 0) channel[i]++;
        }
    }
    // for (int i = 0; i < CHANNELS; i++) {
    //     slUART_LogDecNl(channel[i]);
    // }
}

void printChannels(void) {
    // slUART_WriteStringNl("000000000000000011111111111111112222222222222222333333333333333344444444444444445555555555555555666666666666666677777777777777");
    // slUART_WriteStringNl("0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcd");
}

struct MEASURE {
   int16_t temperature;
   uint16_t humidity;
   int16_t pressure;
   uint8_t voltage;
   uint8_t sensorId;
};
int main(void) {
    slUART_SimpleTransmitInit();
    slUART_WriteStringNl("Starting Poor Man's Wireless 2.4GHz Scanner ...");

    // slUART_WriteString("SPCR: ");
    // slUART_LogBinaryNl(SPCR);
    // slUART_WriteString("SPSR: ");
    // slUART_LogBinaryNl(SPSR);
    slNRF_Init();
    slUART_WriteString("SPCR: ");
    slUART_LogBinaryNl(SPCR);
    slUART_WriteString("SPSR: ");
    slUART_LogBinaryNl(SPSR);

    //disable();
    powerUp();
    powerDown();
    powerUp();

    // switch off Shockburst
    slUART_WriteStringNl("\r\nswitch off Shockburst");
    setRegister(slNRF_EN_AA, 0x0);
    getRegister(slNRF_EN_AA, 1);


    // make sure RF-section is set properly 
    // - just write default value... 
    slUART_WriteStringNl("\r\nRF-section write default value");
    setRegister(slNRF_RF_SETUP, 0x0F);
    getRegister(slNRF_RF_SETUP, 1);


    while (1) {
        // do the scan
        //scanChannels();
        // output the result
        //outputChannels();
    }
    return 0;
}
