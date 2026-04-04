#include "ps2_controller.h"

// SPI INIT
void SPI_Init(void)
{

    // MOSI, SCK, SS เป็น Output
    DDRB |= (1 << PS2_CMD_PIN) | (1 << PS2_CLK_PIN) | (1 << PS2_ATT_PIN);

    // MISO เป็น Input
    DDRB &= ~(1 << PS2_DAT_PIN);

    // Pull-up MISO
    PORTB |= (1 << PS2_DAT_PIN);

    // ATT = HIGH
    PORTB |= (1 << PS2_ATT_PIN);

    // SPI Config
    SPCR = (1 << SPE) | (1 << DORD) | (1 << MSTR) |
           (1 << CPOL) | (1 << CPHA) |
           (1 << SPR1) | (1 << SPR0);
}

// SPI TRANSFER
uint8_t SPI_Transfer(uint8_t data)
{

    SPDR = data;

    while (!(SPSR & (1 << SPIF)))
        ;

    return SPDR;
}

// READ PS2 DATA
void PS2_ReadData(uint8_t *ps2_data)
{

    PORTB &= ~(1 << PS2_ATT_PIN);
    _delay_us(10);

    SPI_Transfer(0x01);
    SPI_Transfer(0x42);
    SPI_Transfer(0x00);

    ps2_data[0] = SPI_Transfer(0x00);
    ps2_data[1] = SPI_Transfer(0x00);
    ps2_data[2] = SPI_Transfer(0x00);
    ps2_data[3] = SPI_Transfer(0x00);
    ps2_data[4] = SPI_Transfer(0x00);
    ps2_data[5] = SPI_Transfer(0x00);

    _delay_us(10);
    PORTB |= (1 << PS2_ATT_PIN);

    return 1;
}