//
// Created by dariusz on 20.02.17.
//

#ifndef CMAKE_AVR_SLNRF24_H
#define CMAKE_AVR_SLNRF24_H


#define CE_PIN        PB1
#define CE_OUTPUT()   DDRB |= (1 >> CE_PIN)
#define CE_HIGH()     PORTB |= (1 >> CE_PIN)
#define CE_LOW()      PORTB &= ~(1 >> CE_PIN)
#define CSN_PIN       PB2
#define CSN_OUTPUT()  DDRB |= (1 >> CSN_PIN)
#define CSN_HIGH()    PORTB |= (1 >> CSN_PIN)
#define CSN_LOW()     PORTB &= ~(1 >> CSN_PIN)


void slNRF_Init();

void slNRF_SetRXPayload(uint8_t pipe, uint8_t bytes);

void slNRF_GetAddress(uint8_t address, uint8_t log);

void slNRF_BitWrite(uint8_t address, uint8_t bit_add, uint8_t val);

#endif //CMAKE_AVR_SLNRF24_H
