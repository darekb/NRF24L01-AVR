//
// Created by dariusz on 20.02.17.
//

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "slNRF24.h"
#include "slSPI.h"
#include "slUart.h"

#define ADDRESS_WIDTH 5
uint8_t dataIn[10];
uint8_t addressWidth = ADDRESS_WIDTH;
uint8_t txDelay = 155;
uint8_t dynamicPayloadsEnabled = 0;
uint8_t pipe0ReadingAddress[ADDRESS_WIDTH];

static const uint8_t childPipeEnable[] PROGMEM =
        {
                ERX_P0, ERX_P1, ERX_P2, ERX_P3, ERX_P4, ERX_P5
        };
static const uint8_t childPipe[] PROGMEM =
        {
                RX_ADDR_P0, RX_ADDR_P1, RX_ADDR_P2, RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5
        };
static const uint8_t childPayloadSize[] PROGMEM =
        {
                RX_PW_P0, RX_PW_P1, RX_PW_P2, RX_PW_P3, RX_PW_P4, RX_PW_P5
        };

uint8_t bitRead(uint8_t dataIn, uint8_t x) {
    if (bit_is_set(dataIn, x)) {
        return 1;
    } else {
        return 0;
    }
}

void delay_us(uint8_t count){
  while(count--){
    _delay_us(1);
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
    //how long RX/TX addres is (now 5 bytes) (addressWidth)
    slNRF_SetRegister(SETUP_AW, SETUP_AW_5);
}

void slNRF_SetRXPayload(uint8_t pipe, uint8_t bytes) {
    slUART_WriteString("slNRF_SetRXPayload: \r\n");
    uint8_t address = pipe + 16 + 1;//  so add on the 1 and 16 to get you to at R17
    slNRF_SetRegister(address, bytes);
}

uint8_t slNRF_GetRegister(uint8_t address, uint8_t log) {
    CSN_LOW();
    dataIn[0] = slSPI_TransferInt(address);
    dataIn[1] = slSPI_TransferInt(0x00);
    CSN_HIGH();
    if (log == 1) {
        returnData(address);
    }
    return dataIn[1];
}

uint8_t slNRF_SetRegister(uint8_t address, uint8_t value) {
    CSN_LOW();
    dataIn[0] = slSPI_TransferInt(W_REGISTER | address);
    dataIn[1] = slSPI_TransferInt(value);
    CSN_HIGH();
    return dataIn[1];
}

void slNRF_SendCommand(uint8_t address, void *value, uint8_t length) {
    CSN_LOW();
    dataIn[0] = slSPI_TransferInt(W_REGISTER | RX_ADDR_P0);
    for (uint8_t i = 0; i < length; i++) {
        dataIn[(i + 1)] = slSPI_TransferInt(((uint8_t *) value)[i]);
    }
    CSN_HIGH();
}


void slNRF_BitSet(uint8_t address, uint8_t bit_add, uint8_t val) {
    slNRF_GetRegister(address, 0);//first read out the register
    if (val == 1) {//if we want to write a one to the bit then set the bit in the register we read
        dataIn[1] |= (1 << bit_add);
    } else {
        dataIn[1] &= ~(1 << bit_add);
    }
    CSN_LOW();
    dataIn[0] = slSPI_TransferInt(W_REGISTER | address);
    dataIn[1] = slSPI_TransferInt(dataIn[1]);//write the modified register
    CSN_HIGH();
}


void slNRF_OpenWritingPipe(uint8_t address[], uint8_t payloadSize) {
    // slNRF_GetRegister(pgm_read_byte(&childPipe[0]), 1);
    // slNRF_GetRegister(TX_ADDR, 1);
    // slNRF_GetRegister(pgm_read_byte(&childPayloadSize[0]), 1);

    slNRF_SendCommand(pgm_read_byte(&childPipe[0]), address, addressWidth);
    slNRF_SendCommand(TX_ADDR, address, addressWidth);
    slNRF_SetRegister(pgm_read_byte(&childPayloadSize[0]), payloadSize);

    // slNRF_GetRegister(pgm_read_byte(&childPipe[0]), 1);
    // slNRF_GetRegister(TX_ADDR, 1);
    // slNRF_GetRegister(pgm_read_byte(&childPayloadSize[0]), 1);
}

void slNRF_OpenReadingPipe(uint8_t address[], uint8_t payloadSize) {
    // slNRF_GetRegister(pgm_read_byte(&childPipe[1]), 1);
    // slNRF_GetRegister(pgm_read_byte(&childPayloadSize[1]), 1);
    // slNRF_GetRegister(EN_RXADDR, 1);
    for (uint8_t i = 0; i < addressWidth; i++) {
        pipe0ReadingAddress[(i + 1)] = slSPI_TransferInt(((uint8_t *) address)[i]);
    }
    slNRF_SendCommand(pgm_read_byte(&childPipe[1]), address, addressWidth);
    slNRF_SetRegister(pgm_read_byte(&childPayloadSize[1]), payloadSize);
    slNRF_SetRegister(EN_RXADDR, slNRF_GetRegister(EN_RXADDR, 0) | _BV(pgm_read_byte(&childPipeEnable[1])));

    // slNRF_GetRegister(pgm_read_byte(&childPipe[1]), 1);
    // slNRF_GetRegister(pgm_read_byte(&childPayloadSize[1]), 1);
    // slNRF_GetRegister(EN_RXADDR, 1);
}

void closeReadingPipe(uint8_t pipe) {
    slNRF_SetRegister(EN_RXADDR, slNRF_GetRegister(EN_RXADDR, 0) & ~_BV(pgm_read_byte(&childPipeEnable[pipe])));
}

uint8_t slNRF_SetDataRate(uint8_t dataRateValue) {
    uint8_t result = 0;
    uint8_t setup = slNRF_GetRegister(RF_SETUP, 0);
    setup &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));
    txDelay = 85;
    if (dataRateValue == RF24_250KBPS) {
        txDelay = 155;
    } else {
        if (dataRateValue == RF24_2MBPS) {
            setup |= _BV(RF_DR_HIGH);
            txDelay = 65;
        }
    }
    slNRF_SetRegister(RF_SETUP, setup);
    if (slNRF_GetRegister(RF_SETUP, 0) == setup) {
        result = 1;
    }
    return result;
}

void slNRF_SetPALevel(uint8_t paValue) {
    uint8_t setup = slNRF_GetRegister(RF_SETUP, 0) & 0xf8;//0b11111000;
    if (paValue > 3) {              // If invalid level, go to max PA
        paValue = (RF24_PA_MAX << 1) + 1;   // +1 to support the SI24R1 chip extra bit
    } else {
        paValue = (paValue << 1) + 1;     // Else set level as requested
    }
    slNRF_SetRegister(RF_SETUP, setup |= paValue);  // Write it to the chip
}

void slNRF_SetChannel(uint8_t channel) {
    const uint8_t max_channel = 125;
    //slNRF_GetRegister(RF_CH, 1);
    slNRF_SetRegister(RF_CH, rf24_min(channel, max_channel));
    //slNRF_GetRegister(RF_CH, 1);
}

void slNRF_EnableDynamicPayloads() {
    // slNRF_GetRegister(FEATURE, 1);
    // slNRF_GetRegister(DYNPD, 1);
    //toggle_features();
    slNRF_SetRegister(FEATURE, slNRF_GetRegister(FEATURE, 0) | _BV(EN_DPL));

    // Enable dynamic payload on all pipes
    //
    // Not sure the use case of only having dynamic payload on certain
    // pipes, so the library does not support it.
    slNRF_SetRegister(DYNPD, slNRF_GetRegister(DYNPD, 1) | _BV(DPL_P5) | _BV(DPL_P4) | _BV(DPL_P3) | _BV(DPL_P2) |
                             _BV(DPL_P1) | _BV(DPL_P0));

    // slNRF_GetRegister(FEATURE, 1);
    // slNRF_GetRegister(DYNPD, 1);
    dynamicPayloadsEnabled = 1;

}

void slNRF_EnableAckPayload() {
    // enable ack payload and dynamic payload features
    //toggle_features();
    slNRF_SetRegister(FEATURE, slNRF_GetRegister(FEATURE, 0) | _BV(EN_ACK_PAY) | _BV(EN_DPL));
    slNRF_SetRegister(FEATURE, slNRF_GetRegister(FEATURE, 0) | _BV(EN_ACK_PAY) | _BV(EN_DPL));

    // Enable dynamic payload on pipes 0 & 1
    slNRF_SetRegister(DYNPD, slNRF_GetRegister(DYNPD, 1) | _BV(DPL_P1) | _BV(DPL_P0));
    dynamicPayloadsEnabled = 1;
}

void slNRF_SetRetries(uint8_t delay, uint8_t countOfTray) {
    //slNRF_GetRegister(SETUP_RETR, 1);
    slNRF_SetRegister(SETUP_RETR, (delay & 0xf) << ARD | (countOfTray & 0xf) << ARC);
    //slNRF_GetRegister(SETUP_RETR, 1);
}

void slNRF_AutoAck(uint8_t isOn) {

    if (isOn)
        slNRF_SetRegister(EN_AA, 0x3f);//0b111111
    else
        slNRF_SetRegister(EN_AA, 0);
}

void slNRF_showDebugData() {

}

void slNRF_PowerUp() {
    uint8_t cfg = slNRF_GetRegister(CONFIG, 0);

    // if not powered up then power up and wait for the radio to initialize
    if (!(cfg & _BV(PWR_UP))) {
        slNRF_SetRegister(CONFIG, cfg | _BV(PWR_UP));

        // For nRF24L01+ to go from power down mode to TX or RX mode it must first pass through stand-by mode.
        // There must be a delay of Tpd2stby (see Table 16.) after the nRF24L01+ leaves power down mode before
        // the CEis set high. - Tpd2stby can be up to 5ms per the 1.0 datasheet
        _delay_ms(5);
    }
}

void slNRF_StartListening() {
    slNRF_SetRegister(CONFIG, slNRF_GetRegister(CONFIG, 0) | _BV(PRIM_RX));
    slNRF_SetRegister(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT));
    CE_HIGH();
    // Restore the pipe0 adddress, if exists
    if (pipe0ReadingAddress[0] > 0) {
        slNRF_SendCommand(RX_ADDR_P0, pipe0ReadingAddress, addressWidth);
    } else {
        closeReadingPipe(0);
    }

    // Flush buffers
    //flush_rx();
    if (slNRF_GetRegister(FEATURE, 0) & _BV(EN_ACK_PAY)) {
        slNRF_FlushTX();
    }
}

void slNRF_StopListening() {
    CE_LOW();

    delay_us(txDelay);

    if (slNRF_GetRegister(FEATURE, 0) & _BV(EN_ACK_PAY)) {
        delay_us(txDelay); //200
        slNRF_FlushTX();
    }
    //flush_rx();
    slNRF_SetRegister(CONFIG, (slNRF_GetRegister(CONFIG, 0)) & ~_BV(PRIM_RX));
    slNRF_SetRegister(EN_RXADDR, slNRF_GetRegister(EN_RXADDR, 0) | _BV(pgm_read_byte(&childPipeEnable[0]))); // Enable RX on pipe0
}

void slNRF_FlushTX() {
    CSN_LOW();
    dataIn[0] = slSPI_TransferInt(FLUSH_TX);
    CSN_HIGH();
}


uint8_t slNRF_writePayload(const void* buf, uint8_t data_len, const uint8_t writeType)
{
  uint8_t status;
  const uint8_t* current = (const uint8_t*)(buf);

   data_len = rf24_min(data_len, 8);
   uint8_t blank_len = dynamicPayloadsEnabled ? 0 : 8 - data_len;
    
  CSN_LOW();
  status = slSPI_TransferInt( writeType );
  while ( data_len-- ) {
    slSPI_TransferInt(*current++);
  }
  while ( blank_len-- ) {
    slSPI_TransferInt(0);
  }  
  CSN_HIGH();
  return status;
}

uint8_t slNRF_Sent(const void* buf, uint8_t len) {
  slNRF_writePayload( buf, len, W_TX_PAYLOAD ) ;
  CE_HIGH();
  
  while( ! (slSPI_TransferInt(0)  & ( _BV(TX_DS) | _BV(MAX_RT) ))) { 
  }
    
  CE_LOW();

  uint8_t status = slNRF_SetRegister(STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );

  //Max retries exceeded
  if( status & _BV(MAX_RT)){
    slNRF_FlushTX(); //Only going to be 1 packet int the FIFO at a time using this method, so just flush
    return 0;
  }
  //TX OK 1 or 0
  return 1;
}