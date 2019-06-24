#ifndef __BIT_OPERATION_H
#define __BIT_OPERATION_H

#include "..\KeilMDK.h"

uint8_t byte_rotate_bit(uint8_t source);
uint8_t byte_setbit(uint8_t source,uint8_t bitnumber);
uint8_t byte_clearbit(uint8_t source,uint8_t bitnumber);
uint8_t byte_getbit(uint8_t source,uint8_t bitnumber);
uint8_t byte_bitoverturn(uint8_t source,uint8_t bitnumber);

uint16_t twobyte_setbit(uint16_t source,uint8_t bitnumber);
uint16_t twobyte_clearbit(uint16_t source,uint8_t bitnumber);
uint16_t twobyte_getbit(uint16_t source,uint8_t bitnumber);
uint16_t twobyte_bitoverturn(uint16_t source,uint8_t bitnumber);

#endif
