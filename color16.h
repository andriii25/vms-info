//
// Created by andry on 6/25/17.
//

#include <stdint.h>

#pragma once

typedef struct color16 {
    uint16_t color;
} color16;

//Note: On most computers the pointer is actually
//      larger than the struct so we just copy it
uint8_t get_alpha4(color16 col);
uint8_t get_red4(color16 col);
uint8_t get_green4(color16 col);
uint8_t get_blue4(color16 col);

uint8_t get_alpha(color16 col);
uint8_t get_red(color16 col);
uint8_t get_green(color16 col);
uint8_t get_blue(color16 col);




