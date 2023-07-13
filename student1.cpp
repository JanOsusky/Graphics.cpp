/**
 * IZG - Zaklady pocitacove grafiky - FIT VUT
 * Lab 01 - Redukce barevneho prostoru
 *
 * Copyright (c) 2022-23 by Michal Vlnas, ivlnas@fit.vutbr.cz
 *
 * Tato sablona slouzi vyhradne pro studijni ucely, sireni kodu je bez vedomi autora zakazano.
 */

#include "base.h"


/**
 * Converts image into grayscale.
 * Use empiric equation presented in lecture.
 */
void ImageTransform::grayscale()
{
    // TODO: write code here

   //std::cout << "Velikost obrazku s: " << cfg->w << " v: " << cfg->h << "\n";
   for (uint32_t i = 0; i < cfg->w; i++) //projeti vsech sloupcu
    {
        for (uint32_t j = 0; j < cfg->h; j++) // projeti kazdeho radku v jednom sloupci
        {
           //std::cout << "\nSloupec: " << i << ",   Radek: " << j << "\n";
            RGB frame = getPixel(i, j); // I = 0.299 · R + 0.587 · G + 0.114 · B
           //std::cout << "R: " << int(frame.r) << " G: " << int(frame.g) << " B: " << int(frame.b) << "\n";
            uint32_t gray = 0.229 * frame.r + 0.587 * frame.g + 0.114 * frame.b;
            std::round(gray);
            //std::cout << "hodnota gray: " << int(gray);
            frame.r = gray;
            frame.g = gray;
            frame.b = gray;
            //std::cout << "\n melo by byt stejne jako gray " << int(frame.b) << "\n\n\n\n";

            setPixel(i, j, frame);
        }
    }
}

/**
 * Evaluate threshold value for further usage in thresholding,
 * has to be called after grayscale().
 *
 * Note: evaluation method will DIFFER for each group!
 *
 * @return threshold
 */
uint8_t ImageTransform::evaluateThreshold()
{
    uint8_t th = 128;

    // TODO: write code here or in evaluateThreshold(x, y)


    IZG_INFO("Evaluated threshold: " + std::to_string(th));
    return th;
}

/**
 * Evaluate threshold value for further usage in thresholding,
 * has to be called after grayscale().
 *
 * Note: evaluation method will DIFFER for each group!
 *
 * @param x Input pixel coordinate
 * @param y Input pixel coordinate
 * @return  threshold
 */
uint8_t ImageTransform::evaluateThreshold(int32_t x, int32_t y)
{
    (void)x;
    (void)y;

    uint8_t th = 128;

    // TODO: write code here or in evaluateThreshold()
    
    uint8_t tileWidth = std::round(cfg->w / 8);
    uint8_t tileHight = std::round(cfg->h / 8);

    int colorTotalNumber = 0;
    int N = tileWidth * tileHight;
    for (uint32_t i = x; i < x + tileWidth; i++) //projeti vsech sloupcu
    {
        for (uint32_t j = y; j < y + tileHight; j++) // projeti kazdeho radku v jednom sloupci
        {
            RGB frame = getPixel(i, j);
            colorTotalNumber += int(frame.r);
        }
    }

    return th = colorTotalNumber/N;
}

void ImageTransform::threshold()
{
    // TODO: write code here
    grayscale();

    uint8_t tileWidth = std::round(cfg->w / 8);
    uint8_t tileHight = std::round(cfg->h / 8);
    int collumCounter = 0;
    int rowCounter = 0;
    while (true) //pocita dlazdice
    {
        int hight = tileHight * collumCounter;  //aktualni pozice y
        int width = tileWidth * rowCounter;     // aktualni pozice x 
        uint8_t threshold = evaluateThreshold(width, hight);

        //std::cout << "sloupec: " << collumCounter << " radek: " << rowCounter << "\n";
        //std::cout << "vyska: " << hight << " sirka: " << width << "\n";
        //std::cout << "pocet radku: " << 8 << " pocet sloupcu: " << 8 << "\n";
        //std::cout << "vyska obrazku: " << cfg->h << " sirka obrazku:" << cfg->w << "\n\n\n";

        for (int i = width; i < width + tileWidth; i++)
        {
            for (int j = hight; j < hight + tileHight; j++)
            {
                RGB frame = getPixel(i, j);
                if (int(frame.b) > threshold)
                {
                    frame = COLOR_WHITE;
                    setPixel(i, j, frame);
                }
                else
                {
                    frame = COLOR_BLACK;
                    setPixel(i, j, frame);
                }
            }
        }

        collumCounter++;
        if (collumCounter > 7)
        {
            collumCounter = 0;
            rowCounter++;
        }
        if (rowCounter > 7)
            break;
    }
}

void ImageTransform::randomDithering()
{
    grayscale();

    for (uint32_t y = 0; y < cfg->h; y++)
    {
        for (uint32_t x = 0; x < cfg->w; x++)
        {
            auto p = getPixel(x, y);
            uint8_t value = p.r > getRandom() ? 255 : 0;

            setPixel(x, y, RGB(value));
        }
    }
}

void ImageTransform::orderedDithering()
{
    static uint8_t matrix[m_side][m_side] =
    {
        { 0,   254, 51,  255 },
        { 68,  136, 187, 119 },
        { 34,  238, 17,  221 },
        { 170, 102, 153, 85  }
    };

    // TODO: write code here

    grayscale();

    for (uint32_t i = 0; i < cfg->w; i++) //projeti vsech sloupcu
    {
        for (uint32_t j = 0; j < cfg->h; j++) // projeti kazdeho radku v jednom sloupci
        {
            int x = i % m_side;
            int y = j % m_side;
            RGB frame = getPixel(i, j);
           // std::cout << "x: " << x << "y: " << y << "\n\n\n";
           // std::cout << "matrix[x][y]: " << int(matrix[x][y]) << "\n\n\n";
            if (matrix[x][y] < frame.r)
            {
                frame = COLOR_WHITE;
                setPixel(i, j, frame);
            }
            else
            {
                frame = COLOR_BLACK;
                setPixel(i, j, frame);
            }
        }
   }
   /* int collumCounter = 0;
    int rowCounter = 0;

    while (true) //pocita dlazdice
    {
        int hight = m_side * collumCounter;  //aktualni pozice y
        int width = m_side * rowCounter;     // aktualni pozice x 
        std::cout << "sloupec: " << collumCounter << " radek: " << rowCounter << "\n";
        std::cout << "vyska: " << hight << " sirka: " << width << "\n";
        std::cout << "pocet radku: " << cfg->h / m_side << " pocet sloupcu: " << cfg->w / m_side << "\n";
        std::cout << "vyska obrazku: " << cfg->h << " sirka obrazku:" << cfg->w << "\n\n\n";
    
        for (int i = width; i < width + m_side; i++)
        {
            for (int j = hight; j < hight + m_side; j++)
            {
                int x = i % m_side;
                int y = j % m_side;
                RGB frame = getPixel(i, j);
                if (matrix[x][y] > 128)
                {
                    frame = COLOR_WHITE;
                    setPixel(i, j, frame);
                }
                else
                {
                    frame = COLOR_BLACK;
                    setPixel(i, j, frame);
                }
            }
        }

        collumCounter++;
        if (collumCounter > cfg->h/m_side)
        {
            collumCounter = 0;
            rowCounter++;
        }
        if (rowCounter > cfg->w/m_side)
            break;
        
    }
    */
    /*int n = 0;
    uint8_t tileWidth =  m_side;
    uint8_t tileHight = m_side;
    int collumCounter = 0;
    int rowCounter = 0;
    while (true) //pocita dlazdice
    {
        int hight = tileHight * collumCounter;  //aktualni pozice y
        int width = tileWidth * rowCounter;     // aktualni pozice x 

        std::cout << "sloupec: " << collumCounter << " radek: " << rowCounter << "\n";
        std::cout << "vyska: " << hight << " sirka: " << width << "\n";
        std::cout << "pocet radku: " << cfg->h / m_side << " pocet sloupcu: " << cfg->w / m_side << "\n";
        std::cout << "vyska obrazku: " << cfg->h << " sirka obrazku:" << cfg->w << "\n\n\n";

        for (int i = width; i < width + tileWidth; i++)
        {
            for (int j = hight; j < hight + tileHight; j++)
            {
                std::cout << "sirka i :" << i << " vyska j: " << j << "\n\n";
                int x = i % m_side;
                int y = j % m_side;
                RGB frame = getPixel(i, j);
                if (matrix[x][y] > 128)
                {
                    frame = COLOR_WHITE;
                    setPixel(i, j, frame);
                }
                else
                {
                    frame = COLOR_BLACK;
                    setPixel(i, j, frame);
                }
            }
        }
        n++;
        if (n > 1000)
            break;
        collumCounter++;
        if (collumCounter > cfg->h/m_side)
        {
            collumCounter = 0;
            rowCounter++;
        }
        if (rowCounter > cfg->w / m_side)
            break;
    }*/

}

void ImageTransform::updatePixelWithError(uint32_t x, uint32_t y, float err, double weight)
{
    (void)x;
    (void)y;
    (void)err;
    (void)weight;

    // TODO: write code here

    if (x >= cfg->w || y >= cfg->h) {
        return;
    }
    RGB frame = getPixel(x, y);

    frame.r = std::max(0, std::min(255, (int)round(frame.r + err * weight)));
    frame.g = std::max(0, std::min(255, (int)round(frame.g + err * weight)));
    frame.b = std::max(0, std::min(255, (int)round(frame.b + err * weight)));
    setPixel(x, y, frame);
}

void ImageTransform::errorDistribution()
{
    // TODO: write code here
    for (int y = 0; y < cfg->h; y++) {
        for (int x = 0; x < cfg->w; x++) {
            RGB frame = getPixel(x, y);
            RGB newColor = (int(frame.r) < 128) ? COLOR_BLACK : COLOR_WHITE;
            setPixel(x, y, newColor);
            // vypocet chybz
            int32_t error = frame.r - ((newColor.r == 0) ? 0 : 255);
            updatePixelWithError(x + 1, y, error, 7.0 / 16.0);
            updatePixelWithError(x - 1, y + 1, error, 3.0 / 16.0);
            updatePixelWithError(x, y + 1, error, 5.0 / 16.0);
            updatePixelWithError(x + 1, y + 1, error, 1.0 / 16.0);
        }
    }
}

