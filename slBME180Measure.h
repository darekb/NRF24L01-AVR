//
// Created by dariusz on 25.02.17.
//

#ifndef CMAKE_AVR_SLBME180MEASURE_H
#define CMAKE_AVR_SLBME180MEASURE_H


struct MEASURE {
   int16_t temperature;
   uint16_t humidity;
   int16_t pressure;
   uint8_t voltage;
   uint8_t sensorId;
};

void fillBuferFromMEASURE (const struct MEASURE structure, uint8_t *buffer);
struct MEASURE returnMEASUREFromBuffer (uint8_t *buffer);
void testBME180Measure(void);

#endif //CMAKE_AVR_SLBME180MEASURE_H
