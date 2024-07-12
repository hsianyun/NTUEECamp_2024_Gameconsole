#pragma once
#include <stdint.h>
#include "esp_err.h"

typedef struct __attribute__((__packed__)) {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}pixel_s;


/**
 * @brief Decode the jpeg ``image.jpg`` embedded into the program file into pixel data.
 *
 * @param pixels A pointer to a pointer for an array of rows, which themselves are an array of pixels.
 *        Effectively, you can get the pixel data by doing ``decode_image(&myPixels); pixelval=myPixels[ypos][xpos];``
 * @return - ESP_ERR_NOT_SUPPORTED if image is malformed or a progressive jpeg file
 *         - ESP_ERR_NO_MEM if out of memory
 *         - ESP_OK on succesful decode
 */

esp_err_t decode_image(pixel_s ***pixels, uint8_t img_start[], uint16_t width, uint16_t height);