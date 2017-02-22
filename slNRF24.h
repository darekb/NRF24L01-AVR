//
// Created by dariusz on 20.02.17.
//

#ifndef CMAKE_AVR_SLNRF24_H
#define CMAKE_AVR_SLNRF24_H


#define CE_PIN        PB1
#define CE_OUTPUT()   DDRB |= (1 >> CE_PIN)
#define CE_HIGH()     PORTB |= (1 >> CE_PIN)//enable RX
#define CE_LOW()      PORTB &= ~(1 >> CE_PIN)//disable RX
#define CSN_PIN       PB2
#define CSN_OUTPUT()  DDRB |= (1 >> CSN_PIN)
#define CSN_HIGH()    PORTB |= (1 >> CSN_PIN)
#define CSN_LOW()     PORTB &= ~(1 >> CSN_PIN)

#define slNRF_CONFIG      0x00
#define slNRF_EN_AA       0x01
#define slNRF_RF_CH       0x05
#define slNRF_RF_SETUP    0x06
#define slNRF_RPD         0x09


void slNRF_Init();

void slNRF_SetRXPayload(uint8_t pipe, uint8_t bytes);

void slNRF_GetAddress(uint8_t address, uint8_t log);

void slNRF_BitWrite(uint8_t address, uint8_t bit_add, uint8_t val);

uint8_t getRegister(uint8_t r, uint8_t log);
void setRegister(uint8_t r, uint8_t v);
void setRx(void);

#endif //CMAKE_AVR_SLNRF24_H
