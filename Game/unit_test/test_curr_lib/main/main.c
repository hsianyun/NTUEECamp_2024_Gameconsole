#include <stdio.h>
#include "st7789.h"

TFT_t Screen;

void app_main(void)
{
    spi_master_init(&Screen, 23, 18, 5, 2, 4, 27);
}
