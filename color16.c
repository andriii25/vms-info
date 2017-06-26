//
// Created by andry on 6/25/17.
//

#include <stdint.h>
#include "color16.h"

static uint8_t upscale_color(uint8_t color4)
{
    return (uint8_t) color4 * 16 + 15;
}

uint8_t get_alpha4(color16 col)
{
    return (uint8_t)((col.color & 0xF000) >> 12);
}

uint8_t get_red4(color16 col)
{
    return (uint8_t)((col.color & 0x0F00) >> 8);
}

uint8_t get_green4(color16 col)
{
    return (uint8_t)((col.color & 0x00F0) >> 4);
}

uint8_t get_blue4(color16 col)
{
    return (uint8_t)(col.color & 0x000F);
}


uint8_t get_alpha(color16 col)
{
    return upscale_color(get_alpha4(col));
}

uint8_t get_red(color16 col)
{
    return upscale_color(get_red4(col));
}

uint8_t get_green(color16 col)
{
    return upscale_color(get_green4(col));
}

uint8_t get_blue(color16 col)
{
    return upscale_color(get_blue4(col));
}

