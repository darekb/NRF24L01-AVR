//
// Created by dariusz on 20.02.17.
//

#include <avr/io.h>
#include <util/delay.h>
#include "slNRF24.h"
#include "slSPI.h"
#include "slUart.h"

uint8_t dataIn[5];


uint8_t bitRead(uint8_t dataIn, uint8_t x) {
    if (bit_is_set(dataIn, x)) {
        return 1;
    } else {
        return 0;
    }
}


void returnData(uint8_t address) {
    switch (address) {
        case 0:
            slUART_WriteString(" CONFIG REGISTER =");
            slUART_LogBinaryNl(dataIn[1]);
            slUART_WriteString("PRIM_RX = ");
            if (bitRead(dataIn[1], 0))
                slUART_WriteStringNl("PRX");
            else
                slUART_WriteStringNl("PTX");

            slUART_WriteString("PWR_UP = ");
            if (bitRead(dataIn[1], 1))
                slUART_WriteStringNl("POWER UP");
            else
                slUART_WriteStringNl("POWER DOWN");

            slUART_WriteString("CRCO = ");
            if (bitRead(dataIn[1], 2))
                slUART_WriteStringNl("2Bytes");
            else
                slUART_WriteStringNl("1Byte");

            slUART_WriteString("EN_CRC = ");
            if (bitRead(dataIn[1], 3))
                slUART_WriteStringNl("Enabled");
            else
                slUART_WriteStringNl("Disabled");

            slUART_WriteString("MASK_MAX_RT = ");
            if (bitRead(dataIn[1], 4))
                slUART_WriteStringNl("Interrupt not reflected on the IRQ pin");
            else
                slUART_WriteStringNl("Reflect MAX_RT as active low interrupt on the IRQ pin");

            slUART_WriteString("MASK_TX_DS = ");
            if (bitRead(dataIn[1], 5))
                slUART_WriteStringNl("Interrupt not reflected on the IRQ pin");
            else
                slUART_WriteStringNl("Reflect TX_DS as active low interrupt on the IRQ pin");

            slUART_WriteString("MASK_RX_DR = ");
            if (bitRead(dataIn[1], 6))
                slUART_WriteStringNl("Interrupt not reflected on the IRQ pin");
            else
                slUART_WriteStringNl("Reflect RX_DR as active low interrupt on the IRQ pin");
            break;//0
        case 1:
            slUART_WriteString(" EN_AA REGISTER Enhanced ShockBurst =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//1
        case 2:
            slUART_WriteString(" EN_RXADDR REGISTER Enabled RX Addresses =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//2
        case 3:
            slUART_WriteString(" SETUP_AW REGISTER Setup of Address Widths =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//3
        case 4:
            slUART_WriteString(" SETUP_RETR REGISTER Setup of Automatic Retransmission =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//4
        case 5:
            slUART_WriteString(" RF_CH REGISTER RF Channel =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//5
        case 6:
            slUART_WriteString(" RF_SETUP REGISTER RF Setup Register =");
            slUART_LogBinaryNl(dataIn[1]);
            slUART_WriteString("RF Power = ");
            slUART_LogBinary(bitRead(dataIn[1], 2));
            slUART_LogBinaryNl(bitRead(dataIn[1], 1));
            slUART_WriteString("RF_DR_HIGH = ");
            slUART_LogBinaryNl(bitRead(dataIn[1], 3));
            slUART_WriteString("PLL_LOCK = ");
            slUART_LogBinaryNl(bitRead(dataIn[1], 4));
            slUART_WriteString("RF_DR_LOW = ");
            slUART_LogBinaryNl(bitRead(dataIn[1], 5));
            slUART_WriteString("CONT_WAVE = ");
            slUART_LogBinaryNl(bitRead(dataIn[1], 7));
            break;//6
        case 7:
            slUART_WriteString(" STATUS REGISTER  =");
            slUART_LogBinaryNl(dataIn[1]);
            slUART_WriteString("TX_FULL = ");
            if (bitRead(dataIn[1], 0))
                slUART_WriteStringNl("TX FIFO full");
            else
                slUART_WriteStringNl("TX FIFO Not full");

            slUART_WriteString("RX_P_NO = ");
            if (bitRead(dataIn[1], 1) && bitRead(dataIn[1], 2) && bitRead(dataIn[1], 3))
                slUART_WriteStringNl("RX FIFO Empty");
            else
                slUART_LogBinaryNl(bitRead(dataIn[1], 1) + (bitRead(dataIn[1], 2) << 1) + (bitRead(dataIn[1], 2) << 2));
            slUART_WriteString("MAX_RT Interrupt = ");
            slUART_LogBinaryNl(bitRead(dataIn[1], 4));
            slUART_WriteString("TX_DS Interrupt = ");
            slUART_LogBinaryNl(bitRead(dataIn[1], 5));
            slUART_WriteString("RX_DR Interrupt = ");
            slUART_LogBinaryNl(bitRead(dataIn[1], 6));
            break;//7
        case 8:
            slUART_WriteString(" OBSERVE_TX REGISTER Transmit observe register  =");
            slUART_LogBinaryNl(dataIn[1]);
            slUART_WriteString("ARC_CNT = ");
            slUART_LogBinaryNl(bitRead(dataIn[1], 0) + (bitRead(dataIn[1], 1) << 1) + (bitRead(dataIn[1], 2) << 2) +
                               (bitRead(dataIn[1], 3) << 3));
            slUART_WriteString("PLOS_CNT = ");
            slUART_LogBinaryNl(bitRead(dataIn[1], 4) + (bitRead(dataIn[1], 5) << 1) + (bitRead(dataIn[1], 6) << 2) +
                               (bitRead(dataIn[1], 7) << 3));
            break;//8
        case 9:
            slUART_WriteString(" RPD REGISTER Received Power Detector =");
            slUART_LogBinaryNl(bitRead(dataIn[1], 0));
            break;//9
        case 10:
            slUART_WriteString(" RX_ADDR_P0 LSB =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//10
        case 11:
            slUART_WriteString(" RX_ADDR_P1 LSB =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//11
        case 12:
            slUART_WriteString(" RX_ADDR_P2 LSB =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//12
        case 13:
            slUART_WriteString(" RX_ADDR_P3 LSB =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//13
        case 14:
            slUART_WriteString(" RX_ADDR_P4 LSB =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//14
        case 15:
            slUART_WriteString(" RX_ADDR_P5 LSB =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//15
        case 16:
            slUART_WriteString(" TX_ADDR LSB =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//16
        case 17:
            slUART_WriteString(" RX_PW_P0 RX payload =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//17
        case 18:
            slUART_WriteString(" RX_PW_P1 RX payload =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//18
        case 19:
            slUART_WriteString(" RX_PW_P2 RX payload =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//19
        case 20:
            slUART_WriteString(" RX_PW_P3 RX payload =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//20
        case 21:
            slUART_WriteString(" RX_PW_P4 RX payload =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//21
        case 22:
            slUART_WriteString(" RX_PW_P5 RX payload =");
            slUART_LogBinaryNl(dataIn[1]);
            break;//22

        case 23:
            slUART_WriteString(" FIFO_STATUS Register =");
            slUART_LogBinaryNl(dataIn[1]);
            slUART_WriteString("RX_EMPTY = ");
            if (bitRead(dataIn[1], 0))
                slUART_WriteStringNl("RX FIFO empty");
            else
                slUART_WriteStringNl("Data in RX FIFO");

            slUART_WriteString("RX_EMPTY = ");
            if (bitRead(dataIn[1], 1))
                slUART_WriteStringNl("RX FIFO full");
            else
                slUART_WriteStringNl("Available locations in RX FIFO");

            slUART_WriteString("TX_EMPTY = ");
            if (bitRead(dataIn[1], 4))
                slUART_WriteStringNl("TX FIFO empty");
            else
                slUART_WriteStringNl("Data in TX FIFO");

            slUART_WriteString("TX_FULL = ");
            if (bitRead(dataIn[1], 5))
                slUART_WriteStringNl("TX FIFO full");
            else
                slUART_WriteStringNl("Available locations in TX FIFO");
            slUART_WriteString("TX_REUSE = ");
            slUART_LogBinaryNl(bitRead(dataIn[1], 6));
            break;//23
    }//switch
}

void slNRF_Init() {
    slUART_WriteString("after init SPI\r\n\r\n");
    slSPI_Init();
    CE_OUTPUT();
    CSN_OUTPUT();
    CSN_HIGH();
    CE_HIGH();
    CE_LOW();
}

void slNRF_SetRXPayload(uint8_t pipe, uint8_t bytes) {
    slUART_WriteString("slNRF_SetRXPayload: \r\n");
    uint8_t address = pipe + 32 + 16 + 1;// a register write starts at 32, so add on the 1 and 16 to get you to at R17
    CSN_LOW();
    dataIn[0] = slSPI_TransferInt(address);
    CSN_HIGH();
    CSN_LOW();
    dataIn[1] = slSPI_TransferInt(bytes);
    CSN_HIGH();
    slUART_WriteString("dataIn[0]: ");
    slUART_LogBinaryNl(dataIn[0]);
    slUART_WriteString("dataIn[1]: ");
    slUART_LogBinaryNl(dataIn[1]);
}

void slNRF_GetAddress(uint8_t address, uint8_t log) {
    slUART_WriteString("slNRF_GetAddress: \r\n");
    CSN_LOW();
    dataIn[0] = slSPI_TransferInt(address);
    CSN_HIGH();
    CSN_LOW();
    dataIn[1] = slSPI_TransferInt(0x00);
    CSN_HIGH();
    slUART_WriteString("dataIn[0]: ");
    slUART_LogBinaryNl(dataIn[0]);
    slUART_WriteString("dataIn[1]: ");
    slUART_LogBinaryNl(dataIn[1]);
    if (log == 1) {
        returnData(address);
    }
}


void slNRF_BitWrite(uint8_t address, uint8_t bit_add,
                    uint8_t val) {//   start bit write   start bit write   start bit write
    //This routine writes single bits of a register, without affecting the rest of the register
    //slUART_WriteString("\r\n\r\n\r\n");
    slNRF_GetAddress(address, 0);//first read out the register
    if (val == 1)//if we want to write a one to the bit then set the bit in the register we read
        dataIn[1] |= (1 << bit_add);
    else
        dataIn[1] &= ~(1 << bit_add);
    slUART_LogBinaryNl(dataIn[1]);
    CSN_LOW();
    dataIn[0] = slSPI_TransferInt(32 + address);//a write to a register adds 32
    CSN_HIGH();
    CSN_LOW();
    dataIn[1] = slSPI_TransferInt(dataIn[1]);//write the modified register
    CSN_HIGH();
    slUART_WriteString("dataIn[0]: ");
    slUART_LogBinaryNl(dataIn[0]);
    slUART_WriteString("dataIn[1]: ");
    slUART_LogBinaryNl(dataIn[1]);
}


uint8_t getRegister(uint8_t r, uint8_t log) {
    uint8_t c;
    CSN_LOW();
    slSPI_TransferInt(r & 0x1F);
    c = slSPI_TransferInt(0);
    CSN_HIGH();
    if (log == 1) {
        slUART_WriteString("\r\n************************\r\n");
        slUART_WriteString("getRegister: \r\n");
        slUART_WriteString("c: ");
        slUART_LogBinaryNl(c);
        dataIn[1] = c;
        returnData(r);
    }
    return c;
}

void setRegister(uint8_t r, uint8_t v) {
//    uint8_t c1, c2;
//    slUART_WriteString("setRegister: \r\n");
//    slUART_WriteString("register: ");
//    slUART_LogBinaryNl(r);
//    slUART_WriteString("value: ");
//    slUART_LogBinaryNl(v);
    CSN_LOW();
    slSPI_TransferInt((r & 0x1F) | 0x20);
    slSPI_TransferInt(v);
    CSN_HIGH();
//    slUART_WriteString("c1: ");
//    slUART_LogBinaryNl(c1);
//    slUART_WriteString("c2: ");
//    slUART_LogBinaryNl(c2);
}

void setRx(void) {
    //slUART_WriteStringNl("\r\nsetRx");
    setRegister(slNRF_CONFIG, getRegister(slNRF_CONFIG, 0) | 0x01);
    enable();
    _delay_us(100);
    //getRegister(slNRF_CONFIG, 1);
}

void powerUp(){
    slUART_WriteStringNl("\r\nPowerUP");
    setRegister(slNRF_CONFIG, getRegister(slNRF_CONFIG, 0) | 0x02);
    _delay_us(130);
    getRegister(slNRF_CONFIG, 1);
}


void powerDown(){
    slUART_WriteStringNl("\r\nPowerDown");
    setRegister(slNRF_CONFIG, getRegister(slNRF_CONFIG, 0) &~0x02);
    getRegister(slNRF_CONFIG, 1);
}

void enable(){
    CE_HIGH();
}

void disable(){
    CE_LOW();
}