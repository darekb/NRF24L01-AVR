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
#include "slNRF24.h"
#include "slBME180Measure.h"

uint8_t pipe1[] = {0xF0, 0xF0, 0xF0, 0xF0, 0xE1};
uint8_t pipe2[] = {0xF0, 0xF0, 0xF0, 0xF0, 0xD2};
uint8_t data[9];
uint8_t i = 0;
uint8_t role = 0;//1 = recivier, 0 = traciever
float t = 0;
struct MEASURE BME180measure = {856, 6818, -1235, 332, 11};

void clearData() {
    for (uint8_t i = 0; i < 9; i++) {
        data[i] = 0;
    };
}
void transmit();
void recivie();

void test(void *test, uint8_t length){
    uint8_t *tmp;
    tmp = test;
    for(uint8_t i = 0; i < length; i++){
        slUART_LogHexNl((uint8_t)*tmp++);
    }
}

int main(void) {
    slUART_SimpleTransmitInit();
    test(&BME180measure, sizeof(BME180measure));

    slNRF_Init();
    if (role == 1) {
        slNRF_OpenWritingPipe(pipe2, 9);
        slNRF_OpenReadingPipe(pipe1, 9);
    }
    if (role == 0) {
        t = 23.45;
        BME180measure.temperature = calculateTemperature(t);
        t = 71.567;
        BME180measure.humidity = calculateHumidity(t);
        t = 102356.456;
        BME180measure.pressure = calculatePressure(t);
        t = 3.23;
        BME180measure.voltage = calculateVoltage(t);
        slNRF_OpenWritingPipe(pipe1, 9);
        slNRF_OpenReadingPipe(pipe2, 9);
    }
    slNRF_SetDataRate(RF24_2MBPS);
    slNRF_SetPALevel(RF24_PA_MAX);
    slNRF_SetChannel(100);
    slNRF_EnableDynamicPayloads();
    slNRF_EnableAckPayload();
    slNRF_SetRetries(0, 15);
    slNRF_AutoAck(1);
    //slNRF_showDebugData();
    slNRF_PowerUp();
    slNRF_StartListening();
    if (role == 1) {
        clearData();
    }
    while (1) {
        if (role == 0) {
            transmit();
            _delay_ms(1000);
        }
        if (role == 1) {
            recivie();
            i++;
        }

    }
    return 0;
}

void transmit(){
    BME180measure.temperature = BME180measure.temperature + 1;
    slNRF_StopListening();
    if (!slNRF_Sent(data, sizeof(data))) {
        slUART_WriteStringNl("Fail...");
    } else {
        fillBuferFromMEASURE(BME180measure, data);
        slUART_LogDecNl(BME180measure.temperature);
        slUART_WriteStringNl("Send ok");
    }
    slNRF_StartListening();
}
void recivie(){
    if (slNRF_Available()) {
        slUART_LogDec(i);
        slUART_WriteStringNl(". recive: ");
        clearData();
        slNRF_Recive(data, sizeof(data));
        BME180measure = returnMEASUREFromBuffer(data);
        slUART_LogDecNl(BME180measure.temperature);
        slUART_LogDecNl(BME180measure.humidity);
        slUART_LogDecNl(BME180measure.pressure);
        slUART_LogDecNl(BME180measure.voltage);
        slUART_LogDecNl(BME180measure.sensorId);
    }
}
