/**
 * IZG - Zaklady pocitacove grafiky - FIT VUT
 * Lab 01 - Redukce barevneho prostoru
 *
 * Copyright (c) 2022 by Michal Vlnas, ivlnas@fit.vutbr.cz
 *
 * Tato sablona slouzi vyhradne pro studijni ucely, sireni kodu je bez vedomi autora zakazano.
 */

#include "base.h"

const int M[] = {
    0, 204, 51, 255,
    68, 136, 187, 119,
    34, 238, 17, 221,
    170, 102, 153, 85
};

void ImageTransform::grayscale()
{
    uint32_t i = 0;
    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            auto p = getPixel(x, y);
            
            i = round(0.299 * p.r + 0.587 * p.g + 0.114 * p.b);

            p.r = i;
            p.g = i;
            p.b = i;
            
            setPixel(x, y, i);
        }
    }
}

void ImageTransform::threshold()
{
    grayscale();

   
    for (int y = 0; y < cfg->height; ++y)
        for (int x = 0; x < cfg->width; ++x)
        {
            
            RGB color = getPixel(x, y);

            
            if (color.r > 124)
                setPixel(x, y, COLOR_WHITE);
            else
                setPixel(x, y, COLOR_BLACK);
        }
}

void ImageTransform::randomDithering()
{
    grayscale();
    srand((unsigned int)time(NULL));

    for (uint32_t y = 0; y < cfg->height; y++)
    {
        for (uint32_t x = 0; x < cfg->width; x++)
        {
            RGB color = getPixel(x, y);

            if (color.r > rand() % 255)
            {
                setPixel(x, y, COLOR_WHITE);
            }
            else
                setPixel(x, y, COLOR_BLACK);
        }
    }
}

void ImageTransform::orderedDithering()
{
    RGB color;
    grayscale();
    int i, j, prah;

    for (int y = 0; y < cfg->height; y++) {
        for (int x = 0; x < cfg->width; x++) {
            
            i = x % m_side;
            j = y % m_side;

            color = getPixel(x, y);
            prah = M[m_side * j + i];
            if (color.r > prah) {
                setPixel(x, y, COLOR_WHITE);
            }
            else {
                setPixel(x, y, COLOR_BLACK);
            }
        }

    }
}

void ImageTransform::updatePixelWithError(uint32_t x, uint32_t y, float err)
{
    RGB color;
    int scale;
    color = getPixel(x, y);
    scale = round(color.g + err);
    if (scale > 255) {
        scale = 255;
    }
    else if (scale < 0) {
        scale = 0;
    }
    color.b = scale;
    color.g = scale;
    color.r = scale;
    setPixel(x, y, color);
}

void ImageTransform::errorDistribution()
{
    RGB color;
    int scale, error = 0;
    grayscale();

    for (int x = 0; x < cfg->width; x++) {
        for (int y = 0; y < cfg->height; y++) {
            color = getPixel(x, y);
            scale = color.g;
            if (scale <= 127) {
                color = COLOR_BLACK;
            }
            else {
                color = COLOR_WHITE;
            }
            error = scale - color.g;
            setPixel(x, y, color);
            updatePixelWithError(x + 1, y, 3.0 / 8.0 * error);
            updatePixelWithError(x, y + 1, 3.0 / 8.0 * error);
            updatePixelWithError(x + 1, y + 1, 2.0 / 8.0 * error);
        }
    }

}

