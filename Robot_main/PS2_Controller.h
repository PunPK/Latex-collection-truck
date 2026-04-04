#ifndef PS2_CONTROLLER_H
#define PS2_CONTROLLER_H

#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

// Pin Mapping (PORTB)

#define PS2_ATT_PIN PB2 // D10
#define PS2_CMD_PIN PB3 // D11
#define PS2_DAT_PIN PB4 // D12
#define PS2_CLK_PIN PB5 // D13

// Function Prototypes
void SPI_Init(void);
uint8_t SPI_Transfer(uint8_t data);
void PS2_ReadData(uint8_t *ps2_data);

typedef enum {
    STOP = 0,

    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,

    FORWARD_LEFT,
    FORWARD_RIGHT,
    BACKWARD_LEFT,
    BACKWARD_RIGHT

} PS2_Status;

PS2_Status PS2_GetStatus(uint8_t *ps2_data);

#endif