#include "SPI_Interface.h"

spiErr_t (*SPI_Open) (void) = (void *) 0x0000ca49;
spiErr_t (*SPI_Close) (void) = (void *) 0x0000ca7d;
spiErr_t (*SPI_SetConfig) (spiConfig_t *pConfig) = (void *) 0x0000caa1;
spiErr_t (*SPI_WriteSync) (uint32_t Data) = (void *) 0x0000cb81;
