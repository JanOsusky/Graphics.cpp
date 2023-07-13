/**
 * @file        student.cpp
 * @author      Ladislav Mosner, VUT FIT Brno, imosner@fit.vutbr.cz
 * @author      Petr Kleparnik, VUT FIT Brno, ikleparnik@fit.vutbr.cz
 * @author      Kamil Behun, VUT FIT Brno, ibehun@fit.vutbr.cz
 * @date        11.03.2018
 *
 * @brief       Deklarace funkci studentu. DOPLNUJI STUDENTI
 *
 */

#include "base.h"
#include "student.h"
#include "globals.h"
#include <math.h>

/**
 * @brief Vraci barvu pixelu z pozice [x, y]
 * @param[in] x X souradnice pixelu
 * @param[in] y Y souradnice pixelu
 * @return Barva pixelu na pozici [x, y] ve formatu RGBA
 */
RGBA getPixel(int x, int y)
{
    if (x >= width || y >= height || x < 0 || y < 0)
    {
        IZG_ERROR("Pristup do framebufferu mimo hranice okna\n");
    }
    return framebuffer[y * width + x];
}

/**
 * @brief Nastavi barvu pixelu na pozici [x, y]
 * @param[in] x X souradnice pixelu
 * @param[in] y Y souradnice pixelu
 * @param[in] color Barva pixelu ve formatu RGBA
 */
void putPixel(int x, int y, RGBA color)
{
    if (x >= width || y >= height || x < 0 || y < 0)
    {
        IZG_ERROR("Pristup do framebufferu mimo hranice okna\n");
    }
    framebuffer[y * width + x] = color;
}

/**
 * @brief Vykresli usecku se souradnicemi [x1, y1] a [x2, y2]
 * @param[in] x1 X souradnice 1. bodu usecky
 * @param[in] y1 Y souradnice 1. bodu usecky
 * @param[in] x2 X souradnice 2. bodu usecky
 * @param[in] y2 Y souradnice 2. bodu usecky
 * @param[in] color Barva pixelu usecky ve formatu RGBA
 * @param[in] arrow Priznak pro vykresleni sipky (orientace hrany)
 */
void drawLine(int x1, int y1, int x2, int y2, RGBA color, bool arrow = false)
{

    if (arrow)
    {
        // Sipka na konci hrany
        double vx1 = x2 - x1;
        double vy1 = y2 - y1;
        double length = sqrt(vx1 * vx1 + vy1 * vy1);
        double vx1N = vx1 / length;
        double vy1N = vy1 / length;
        double vx1NN = -vy1N;
        double vy1NN = vx1N;
        int w = 3;
        int h = 10;
        int xT = (int)(x2 + w * vx1NN - h * vx1N);
        int yT = (int)(y2 + w * vy1NN - h * vy1N);
        int xB = (int)(x2 - w * vx1NN - h * vx1N);
        int yB = (int)(y2 - w * vy1NN - h * vy1N);
        pinedaTriangle(Point(x2, y2), Point(xT, yT), Point(xB, yB), color, color, false);
    }

    bool steep = abs(y2 - y1) > abs(x2 - x1);

    if (steep)
    {
        SWAP(x1, y1);
        SWAP(x2, y2);
    }

    if (x1 > x2)
    {
        SWAP(x1, x2);
        SWAP(y1, y2);
    }

    const int dx = x2 - x1, dy = abs(y2 - y1);
    const int P1 = 2 * dy, P2 = P1 - 2 * dx;
    int P = 2 * dy - dx;
    int y = y1;
    int ystep = 1;
    if (y1 > y2)
        ystep = -1;

    for (int x = x1; x <= x2; x++)
    {
        if (steep)
        {
            if (y >= 0 && y < width && x >= 0 && x < height)
            {
                putPixel(y, x, color);
            }
        }
        else
        {
            if (x >= 0 && x < width && y >= 0 && y < height)
            {
                putPixel(x, y, color);
            }
        }

        if (P >= 0)
        {
            P += P2;
            y += ystep;
        }
        else
        {
            P += P1;
        }
    }
}

/**
 * @brief Vyplni a vykresli trojuhelnik
 * @param[in] v1 Prvni bod trojuhelniku
 * @param[in] v2 Druhy bod trojuhelniku
 * @param[in] v3 Treti bod trojuhelniku
 * @param[in] color1 Barva vyplne trojuhelniku
 * @param[in] color2 Barva hranice trojuhelniku
 * @param[in] arrow Priznak pro vykresleni sipky (orientace hrany)
 *
 * SPOLECNY UKOL. Doplnuji studenti se cvicicim.
 */
void pinedaTriangle(const Point &v1, const Point &v2, const Point &v3, const RGBA &color1, const RGBA &color2, bool arrow)
{
    // Nalezeni obalky (minX, maxX), (minY, maxY) trojuhleniku.
    //////// DOPLNTE KOD /////////
    int min_y = MIN(MIN(v1.y, v2.y), v3.y);
    int min_x = MIN(MIN(v1.x, v2.x), v3.x);
    int max_y = MAX(MAX(v1.y, v2.y), v3.y);
    int max_x = MAX(MAX(v1.x, v2.x), v3.x);

    // Oriznuti obalky (minX, maxX, minY, maxY) trojuhleniku podle rozmeru okna.
    min_y = MAX(0, min_y);
    min_x = MAX(0, min_x);
    max_y = MIN(height, max_y);
    max_x = MIN(width, max_x);

    //////// DOPLNTE KOD /////////
    int h1_y = v2.y - v1.y;
    int h1_x = v2.x - v1.x;
    int h2_y = v3.y - v2.y;
    int h2_x = v3.x - v2.x;
    int h3_y = v1.y - v3.y;
    int h3_x = v1.x - v3.x;
    //^ Yellow vectors h (from slides), in the comments for Polygon are referred to as deltas

    int b1_y = min_y - v1.y;
    int b1_x = min_x - v1.x;
    int b2_y = min_y - v2.y;
    int b2_x = min_x - v2.x;
    int b3_y = min_y - v3.y;
    int b3_x = min_x - v3.x;
    //^ Blue vectors b (from slides)

    int e1 = b1_y * h1_x - b1_x * h1_y;
    int e2 = b2_y * h2_x - b2_x * h2_y;
    int e3 = b3_y * h3_x - b3_x * h3_y;
    //^ Red vector obtained from cross product of b and h

    // Vyplnovani: Cyklus pres vsechny body (x, y) v obdelniku (minX, minY), (maxX, maxY).
    // Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) nebo P (x, y +/- 1)
    // vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y).
    //////// DOPLNTE KOD /////////
    for (int y = min_y; y < max_y; ++y)
    {
        int t1 = e1;
        int t2 = e2;
        int t3 = e3;
        for (int x = min_x; x < max_x; ++x)
        {
            if (t1 > 0 && t2 > 0 && t3 > 0)
                putPixel(x, y, color1);
            t1 -= h1_y;
            t2 -= h2_y;
            t3 -= h3_y;
        }
        e1 += h1_x;
        e2 += h2_x;
        e3 += h3_x;
    }

    // Prekresleni hranic trojuhelniku barvou color2.
    drawLine(v1.x, v1.y, v2.x, v2.y, color2, arrow);
    drawLine(v2.x, v2.y, v3.x, v3.y, color2, arrow);
    drawLine(v3.x, v3.y, v1.x, v1.y, color2, arrow);
}

/**
 * @brief Vyplni a vykresli polygon
 * @param[in] points Pole bodu polygonu
 * @param[in] size Pocet bodu polygonu (velikost pole "points")
 * @param[in] color1 Barva vyplne polygonu
 * @param[in] color2 Barva hranice polygonu
 *
 * SAMOSTATNY BODOVANY UKOL. Doplnuji pouze studenti.
 */
void pinedaPolygon(const Point *points, const int size, const RGBA &color1, const RGBA &color2)
{
    // Pri praci muzete vyuzit pro vas predpripravene datove typy z base.h., napriklad:
    //
    //      Pro ukladani parametru hranovych funkci muzete vyuzit prichystany vektor parametru hranovych funkci "EdgeParams":
    //
    //          EdgeParams edgeParams(size)                         // Vytvorite vektor (pole) "edgeParams" parametru hranovych funkci o velikosti "size".
    //          edgeParams[i].deltaX, edgeParams[i].deltaY          // Pristup k parametrum (deltaX, deltaY) hranove funkce v poli "edgeParams" na indexu "i".
    //
    //      Pro ukladani hodnot hranovych funkci muzete vyuzit prichystany vektor hodnot hranovych funkci "EdgeFncValues":
    //
    //          EdgeFncValues edgeFncValues(size)                   // Vytvorite vektor (pole) "edgeFncValues" hodnot hranovych funkci o velikosti "size".
    //          edgeFncValues[i]                                    // Pristup k hodnote hranove funkce v poli "edgeFncValues" na indexu "i".
    //

    bool flag = false;
    int yellow_v1 = 0;
    int blue_v1 = 0;
    int yellow_v2 = 0;
    int blue_v2 = 0;
    EdgeFncValues tmp(size);
    EdgeParams yellowVector(size);
    EdgeFncValues fnc(size);

    // Nalezeni obalky (minX, maxX), (minY, maxY) polygonu.

    //////// DOPLNTE KOD /////////

    int min_x = points[0].x;
    int max_x = points[0].x;
    int min_y = points[0].y;
    int max_y = points[0].y;

    for (int i = 0; i < size; i++)
    {
        min_x = MIN(points[i].x, min_x);
        min_y = MIN(points[i].y, min_y);
        max_x = MAX(points[i].x, max_x);
        max_y = MAX(points[i].y, max_y);
    }

    // Oriznuti obalky (minX, maxX), (minY, maxY) polygonu podle rozmeru okna

    //////// DOPLNTE KOD /////////
    min_x = MAX(0, min_x);
    min_y = MAX(0, min_y);
    max_x = MIN(width, max_x);
    max_y = MIN(height, max_y);

    // Spocitani parametru (deltaX, deltaY) hranove funkce pro kazdou hranu.
    // Hodnoty deltaX, deltaY jsou souradnicemi vektoru, ktery ma pocatek
    // v prvnim vrcholu hrany, konec v druhem vrcholu.
    // Vypocet prvnotnich hodnot hranovych funkci pro jednotlive hrany.

    //////// DOPLNTE KOD /////////

    for (int i = 0; i < size; i++)
    {
        if (i + 1 == size)
        {
            yellowVector[i].deltaX = points[0].x - points[i].x;
            yellowVector[i].deltaY = points[0].y - points[i].y;
        }
        else
        {
            yellowVector[i].deltaX = points[i + 1].x - points[i].x;
            yellowVector[i].deltaY = points[i + 1].y - points[i].y;
        }
    }

    for (int i = 0; i < size; i++)
    {
        blue_v1 = min_y - points[i].y;
        blue_v2 = min_x - points[i].x;
        fnc[i] = (blue_v1 * yellowVector[i].deltaX) - (blue_v2 * yellowVector[i].deltaY);
    }

    // Test konvexnosti polygonu //
    //////// DOPLNTE KOD /////////

    // Vyplnovani: Cyklus pres vsechny body (x, y) v obdelniku (minX, minY), (maxX, maxY).
    // Pro aktualizaci hodnot hranove funkce v bode P (x +/- 1, y) nebo P (x, y +/- 1)
    // vyuzijte hodnoty hranove funkce E (x, y) z bodu P (x, y) */

    //////// DOPLNTE KOD /////////
    // scanline algortimus
    for (int y = min_y; y < max_y; y++)
    {

        for (int i = 0; i < size; i++)
        {
            tmp[i] = fnc[i];
        }

        for (int x = min_x; x < max_x; x++)
        {
            for (int i = 0; i < size; i++)
            {
                if (tmp[i] < 0)
                {
                    flag = false;
                }
            }
            if (flag)
            {
                putPixel(x, y, color1);
            }
            for (int i = 0; i < size; i++)
            {
                tmp[i] -= yellowVector[i].deltaY; // new row
            }
            flag = true;
        }
        for (int i = 0; i < size; i++)
        {
            fnc[i] += yellowVector[i].deltaX; // new column
        }
    }

    // Prekresleni hranic polygonu barvou color2.
    for (int i = 0; i < size; i++)
    {
        drawLine(points[i].x, points[i].y, points[(i + 1) % size].x, points[(i + 1) % size].y, color2 /*, true*/);
    }
}
