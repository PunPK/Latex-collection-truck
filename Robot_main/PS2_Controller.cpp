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

PS2_Status PS2_GetStatus(uint8_t *ps2_data) {
    uint8_t data_b1 = ps2_data[1];  // ข้อมูลปุ่มกดทิศทาง (ใช้เช็คหน้า-หลัง)
    uint8_t analog_x = ps2_data[5]; // ข้อมูลแกน X (0x00=ซ้าย, 0x80=กลาง, 0xFF=ขวา)

    // ===== Deadzone =====
    // ค่ากลางของแกน X คือ 0x80 (128) และสถานะปล่อยปุ่มของ b1 คือ 0xFF (255)
    if (data_b1 == 0xFF && analog_x > 100 && analog_x < 155) {
        return STOP;
    }

    // ===== แปลงเป็น direction =====
    int x = 0;
    int y = 0;

    // แกน X (ซ้าย-ขวา เช็คจาก analog_x)
    if (analog_x < 100) {
        x = -1; // LEFT (ค่าในตารางคือ 0x00)
    } 
    else if (analog_x > 155) {
        x = 1;  // RIGHT (ค่าในตารางคือ 0xFF)
    }

    // แกน Y (หน้า-หลัง)
    // จากตาราง ไม่มีแกน Y แบบ Analog ส่งมา เราจึงต้องดักค่าจากปุ่ม (data_b1) โดยตรง
    // สังเกตว่าค่าที่มีการกด "ไปหน้า" จะเป็น FE, FC, E6
    if (data_b1 == 0xFE || data_b1 == 0xFC || data_b1 == 0xE6) {
        y = 1;  // FORWARD
    }
    // สังเกตว่าค่าที่มีการกด "ถอยหลัง" จะเป็น FB, F9, E3
    else if (data_b1 == 0xFB || data_b1 == 0xF9 || data_b1 == 0xE3) {
        y = -1; // BACKWARD
    }

    // ===== Mapping 8 ทิศ =====
    if (x == 0 && y == 1) return FORWARD;
    if (x == 0 && y == -1) return BACKWARD;
    if (x == -1 && y == 0) return LEFT;
    if (x == 1 && y == 0) return RIGHT;

    if (x == -1 && y == 1) return FORWARD_LEFT;
    if (x == 1 && y == 1) return FORWARD_RIGHT;
    if (x == -1 && y == -1) return BACKWARD_LEFT;
    if (x == 1 && y == -1) return BACKWARD_RIGHT;

    return STOP;
}