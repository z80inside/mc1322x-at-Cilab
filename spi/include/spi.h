
#ifndef SPI
#define SPI

#include <stdint.h>

#define TX_DATA (0)
#define RX_DATA (0x04)
#define CLK_CTRL (0x08)
/* SPI_CLK_CTRL bits */
#define SPI_START 7
#define SPI_SCK_COUNT 8
#define SPI_DATA_LENGTH 0

#define SETUP (0x0c)
/* SPI_SETUP bits */
#define SPI_SS_SETUP 0
#define SPI_SS_DELAY 2
#define SPI_SDO_INACTIVE_ST 4
#define SPI_SCK_POL 8
#define SPI_SCK_PHASE 9
#define SPI_MISO_PHASE 10
#define SPI_SCK_FREQ 12
#define SPI_MODE 16
#define SPI_3WIRE 17

#define STATUS (0x10)
/* SPI_STATUS bits */
#define SPI_INT 0
#define SPI_OVERFLOW 4
#define SPI_FIRST_DATA 8

#define SPI_BASE (0x80002000)

#define SPI_TX_DATA	((volatile uint32_t *) (SPI_BASE + TX_DATA))
#define SPI_RX_DATA	((volatile uint32_t *) (SPI_BASE + RX_DATA))
#define SPI_CLK_CTRL	((volatile uint32_t *) (SPI_BASE + CLK_CTRL))
#define SPI_SETUP	((volatile uint32_t *) (SPI_BASE + SETUP))
#define SPI_STATUS	((volatile uint32_t *) (SPI_BASE + STATUS))

#endif
