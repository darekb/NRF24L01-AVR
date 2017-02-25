//
// Created by dariusz on 25.02.17.
//
#include <avr/io.h>
#include <string.h>
#include "slBME180Measure.h"
#include "slUart.h"

void fillBuferFromMEASURE (const struct MEASURE structure, uint8_t *buffer) {
    memcpy(buffer, &structure, sizeof(struct MEASURE));
}
struct MEASURE returnMEASUREFromBuffer (uint8_t *buffer) {
    struct MEASURE  tmp;
    memcpy(&tmp, buffer, 8*sizeof(uint8_t));
    return tmp;
}

void testBME180Measure(void){
    struct MEASURE  BME180measure = {856,6818,-1235,33,11};
    struct MEASURE  t2 = {0,0,0,0,11};
    uint8_t buffer[8];
    uint8_t buff[8];
    fillBuferFromMEASURE(BME180measure, buffer);
    slUART_WriteString("\r\n***************************\r\n");
    slUART_WriteBuffer(buff, 8);
    slUART_WriteString("\r\n***************************\r\n");
    float temp = -12.45;
    BME180measure.temperature = (int16_t)((float)temp * (int16_t)100);
    BME180measure.humidity = 4567;
    BME180measure.pressure = 245;
    t2 = returnMEASUREFromBuffer(buffer);
    slUART_LogDecNl(t2.temperature);
    slUART_LogDecNl(BME180measure.temperature);
    slUART_LogDecNl(t2.humidity);
    slUART_LogDecNl(BME180measure.humidity);
    slUART_LogDecNl(t2.pressure);
    slUART_LogDecNl(BME180measure.pressure);
}