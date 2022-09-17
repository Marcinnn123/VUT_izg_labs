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
    if (x >= width || y >= height || x < 0 || y < 0) {
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
    if (x >= width || y >= height || x < 0 || y < 0) {
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

    if (arrow) {
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
        int xT = (int) (x2 + w * vx1NN - h * vx1N);
        int yT = (int) (y2 + w * vy1NN - h * vy1N);
        int xB = (int) (x2 - w * vx1NN - h * vx1N);
        int yB = (int) (y2 - w * vy1NN - h * vy1N);
        pinedaTriangle(Point(x2, y2), Point(xT, yT), Point(xB, yB), color, color, false);
    }

    bool steep = abs(y2 - y1) > abs(x2 - x1);

    if (steep) {
        SWAP(x1, y1);
        SWAP(x2, y2);
    }

    if (x1 > x2) {
        SWAP(x1, x2);
        SWAP(y1, y2);
    }

    const int dx = x2 - x1, dy = abs(y2 - y1);
    const int P1 = 2 * dy, P2 = P1 - 2 * dx;
    int P = 2 * dy - dx;
    int y = y1;
    int ystep = 1;
    if (y1 > y2) ystep = -1;

    for (int x = x1; x <= x2; x++) {
        if (steep) {
            if (y >= 0 && y < width && x >= 0 && x < height) {
                putPixel(y, x, color);
            }
        } else {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                putPixel(x, y, color);
            }
        }

        if (P >= 0) {
            P += P2;
            y += ystep;
        } else {
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
    int min_y = MIN(MIN(v1.y, v2.y), v3.y);
    int min_x = MIN(MIN(v1.x, v2.x), v3.x);

    int max_y = MAX(MAX(v1.y, v2.y), v3.y);
    int max_x = MAX(MAX(v1.x, v2.x), v3.x);


    min_y = MAX(0, min_y);
    min_x = MAX(0, min_x);

    max_x = MIN(width - 1, max_x);
    max_y = MIN(height - 1, max_y);


    int delta_y1 = v2.y - v1.y;
    int delta_x1 = v2.x - v1.x;
    int delta_x2 = v3.x - v2.x;
    int delta_y2 = v3.y - v2.y;
    int delta_x3 = v1.x - v3.x;
    int delta_y3 = v1.y - v3.y;


    int e1 = (min_y - v1.y) * delta_x1 - (min_x - v1.x) * delta_y1;
    int e2 = (min_y - v2.y) * delta_x2 - (min_x - v2.x) * delta_y2;
    int e3 = (min_y - v3.y) * delta_x3 - (min_x - v3.x) * delta_y3;

    for (int y = min_y; y <= max_y; ++y)
    {
        int t1 = e1;
        int t2 = e2;
        int t3 = e3;
        for (int x = min_x; x <= max_x; ++x)
        {
            if (t1 >= 0 && t2 >= 0 && t3 >= 0)
            {
                putPixel(x, y, color1);
            }
            t1 -= delta_y1;
            t2 -= delta_y2;
            t3 -= delta_y3;
        }
        e1 += delta_x1;
        e2 += delta_x2;
        e3 += delta_x3;
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
    EdgeFncValues edge_val(size);
    EdgeParams edge(size);
    int min_y = points[0].y;
    int min_x = points[0].x;
    int max_y = points[0].y;
    int max_x = points[0].x;

    for (int i = 1; i < size; i++)
    {
        min_y = MIN(min_y, points[i].y);
        min_x = MIN(min_x, points[i].x);
        max_y = MAX(max_y, points[i].y);
        max_x = MAX(max_x, points[i].x);

    }

    min_y = MAX(0, min_y);
    min_x = MAX(0, min_x);

    max_x = MIN(width - 1, max_x);
    max_y = MIN(height - 1, max_y);

    int next;

    for (int i = 0; i < size; next = (i + 1) % size; )
    {
        edge[i].deltaX = points[next].x - points[i].x;
        edge[i].deltaY = points[next].y - points[i].y;
        i++;
    }

    
    for (int i = 0; i < size; i++) 
    {
        edge_val[i] = (min_y - points[i].y) * edge[i].deltaX - (min_x - points[i].x) * edge[i].deltaY;
    }
    
    for (int y = min_y; y <= max_y; y++)
    {
        for (int x = min_x; x <= max_x; x++)
        {
            bool check = false;
            for (int i = 0; i < size; i++)
            {
                if (edge_val[i] < 0) 
                {
                    check = true;
                    break;
                }
            }
            if (!check) {
                putPixel(x, y, color2);
            }
          
        }
        
    }
    // Prekresleni hranic polygonu barvou color2.
    for (int i = 0; i < size; i++) {
        drawLine(points[i].x, points[i].y, points[(i + 1) % size].x, points[(i + 1) % size].y, color2/*, true*/);
    }
}
