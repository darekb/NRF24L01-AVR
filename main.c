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

#define LED (1 << PB0)
#define LED_TOG PORTB ^= LED


void clearData() {
    for (uint8_t i = 0; i < 9; i++) {
        data[i] = 0;
    };
}

void transmit();

void recivie();

void nrf24_Start();

//server
void sensor11start();

void waitForSensor11();

void sensor11sendViaUart();

//sensor11
void waitForStart();

uint8_t setBME280Mode();

uint8_t getDataFromBME280();

uint8_t sendVianRF24L01();

void test(void *test, uint8_t length) {
    uint8_t *tmp;
    tmp = test;
    for (uint8_t i = 0; i < length; i++) {
        slUART_LogHexNl((uint8_t) *tmp++);
    }
}

uint8_t pipe1[] = {0xF0, 0xF0, 0xF0, 0xF0, 0xE1};
uint8_t pipe2[] = {0xF0, 0xF0, 0xF0, 0xF0, 0xD2};
uint8_t data[9];
uint8_t i = 0;
uint8_t role = 0;
//1 = sensor, 0 = server
float t = 0;
struct MEASURE BME180measure = {856, 6818, -1235, 332, 11};
volatile uint8_t stage;
uint8_t t1[9] = {0x73, 0x74, 0x61, 0x72, 0x74, 0x2d, 0x73, 0x31, 0x31};
const char startString[] = "start-s11";

int main(void) {
    slUART_SimpleTransmitInit();
    //test(&BME180measure, sizeof(BME180measure));
    if (strcmp((char *) t1, startString)) {
        slUART_WriteStringNl("start-s11");
    }
    nrf24_Start();
    stage = 1;
    while (1) {
        switch (stage) {
            case 1:
                sensor11start();
                break;
            case 2:
                waitForSensor11();
                break;
            case 3:
                sensor11sendViaUart();
                break;
            case 11:
                waitForStart();
                break;
            case 12:
                setBME280Mode();
                break;
            case 13:
                getDataFromBME280();
                break;
            case 14:
                sendVianRF24L01();
                break;
        }
    }
    return 0;
}

void nrf24_Start() {
    slNRF_Init();
    if (role == 1) {
        slNRF_OpenWritingPipe(pipe2, 9);
        slNRF_OpenReadingPipe(pipe1, 9, 1);
    }
    if (role == 0) {

        slNRF_OpenWritingPipe(pipe1, 9);
        slNRF_OpenReadingPipe(pipe2, 9, 1);
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
}

//sensor11

//stage 11
void waitForStart() {
    if (slNRF_Available()) {
        clearData();
        slNRF_Recive(data, sizeof(data));
        if (strcmp((char *) data, startString)) {
            stage = 12;
        }
    }

}

//stage 12
uint8_t setBME280Mode() {
    stage = 13;
    return 0;
}

//stage 13
uint8_t getDataFromBME280() {
    t = 23.45;
    BME180measure.temperature = calculateTemperature(t);
    t = 71.567;
    BME180measure.humidity = calculateHumidity(t);
    t = 102356.456;
    BME180measure.pressure = calculatePressure(t);
    t = 3.23;
    BME180measure.voltage = calculateVoltage(t);
    stage = 14;
    return 0;
}

//stage 14
uint8_t sendVianRF24L01() {
    LED_TOG;
    fillBuferFromMEASURE(BME180measure, data);
    slNRF_StopListening();
    if (!slNRF_Sent(data, sizeof(data))) {
        slUART_WriteStringNl("Fail|z");
    } else {
        slUART_WriteStringNl("SendOk|z");
    }
    slNRF_StartListening();
    LED_TOG;
    stage = 0;
    return 0;
}

//server

//stage1
void sensor11start() {
    uint8_t startString[] = "start-s11";
    slNRF_StopListening();
    if (!slNRF_Sent(startString, sizeof(startString))) {
        slUART_WriteStringNl("Fail|z");
    } else {
        slUART_WriteStringNl("SendOk|z");
    }
    slNRF_StartListening();
    stage = 2;
}

//stage 2
void waitForSensor11() {
    if (slNRF_Available()) {
        clearData();
        slNRF_Recive(data, sizeof(BME180measure));
        BME180measure = returnMEASUREFromBuffer(data);
        stage = 3;
    }
}

//stage 3
void sensor11sendViaUart() {
    slUART_LogDecWithSign(BME180measure.temperature);
    slUART_WriteString("|");
    slUART_LogDecWithSign(BME180measure.humidity);
    slUART_WriteString("|");
    slUART_LogDecWithSign(BME180measure.pressure);
    slUART_WriteString("|");
    slUART_LogDecWithSign(BME180measure.voltage);
    slUART_WriteString("|");
    slUART_LogDecWithSign(BME180measure.sensorId);
    slUART_WriteString("|z");
    stage = 0;
}

// void transmit() {
//     BME180measure.temperature = BME180measure.temperature + 1;
//     slNRF_StopListening();
//     if (!slNRF_Sent(data, sizeof(data))) {
//         slUART_WriteStringNl("Fail...");
//     } else {
//         fillBuferFromMEASURE(BME180measure, data);
//         slUART_LogDecNl(BME180measure.temperature);
//         slUART_WriteStringNl("Send ok");
//     }
//     slNRF_StartListening();
// }

// void recivie() {
//     if (slNRF_Available()) {
//         slUART_LogDec(i);
//         slUART_WriteStringNl(". recive: ");
//         clearData();
//         slNRF_Recive(data, sizeof(data));
//         BME180measure = returnMEASUREFromBuffer(data);
//         slUART_LogDecNl(BME180measure.temperature);
//         slUART_LogDecNl(BME180measure.humidity);
//         slUART_LogDecNl(BME180measure.pressure);
//         slUART_LogDecNl(BME180measure.voltage);
//         slUART_LogDecNl(BME180measure.sensorId);
//     }
// }
