#include "helpers.h"
#include <math.h>

typedef struct
{
    float bsum;
    float gsum;
    float rsum;
}
rgbsum;
typedef struct
{
    double bsum;
    double gsum;
    double rsum;
}
rgbsumedge;

//Takes the 9x9 sum
rgbsum _9ave(int height, int width, int h, int w, RGBTRIPLE image[height][width]);
//Takes the edges sum
rgbsum slcave(int height, int width, int h, int w, RGBTRIPLE image[height][width]);
rgbsum srcave(int height, int width, int h, int w, RGBTRIPLE image[height][width]);
rgbsum ilcave(int height, int width, int h, int w, RGBTRIPLE image[height][width]);
rgbsum ircave(int height, int width, int h, int w, RGBTRIPLE image[height][width]);

//Takes the corners sum
rgbsum icave(int height, int width, int h, int w, RGBTRIPLE image[height][width]);
rgbsum lcave(int height, int width, int h, int w, RGBTRIPLE image[height][width]);
rgbsum rcave(int height, int width, int h, int w, RGBTRIPLE image[height][width]);
rgbsum scave(int height, int width, int h, int w, RGBTRIPLE image[height][width]);

//Calculates the gx and the gy
double _gx(BYTE x1, BYTE x3, BYTE x4, BYTE x6, BYTE x7, BYTE x9);
double _gy(BYTE x1, BYTE x2, BYTE x3, BYTE x7, BYTE x8, BYTE x9);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            BYTE *b = &image[h][w].rgbtBlue;
            BYTE *r = &image[h][w].rgbtRed;
            BYTE *g = &image[h][w].rgbtGreen;
            float ave = (*b + *r + *g) / 3.0;
            int average = round(ave);
            if(average < 0)
            {
                *b = 0;
                *r = 0;
                *g = 0;
            }
            else if(average > 255)
            {
                *b = 255;
                *r = 255;
                *g = 255;
            }
            else
            {
                *b = average;
                *r = average;
                *g = average;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < (width / 2); w++)
        {
            int ww = (width - 1) - w;
            RGBTRIPLE *brg1 = &image[h][w];
            RGBTRIPLE *brg2 = &image[h][ww];
            RGBTRIPLE sb = *brg2;
            *brg2 = *brg1;
            *brg1 = sb;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imgCopy[height][width];
    for (int row = 0; row < height; row++)
	{
		for (int w = 0; w < width; w++)
		{
			imgCopy[row][w].rgbtRed = image[row][w].rgbtRed;
			imgCopy[row][w].rgbtGreen = image[row][w].rgbtGreen;
			imgCopy[row][w].rgbtBlue = image[row][w].rgbtBlue;
		}
	}

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //Declares the final triple
            rgbsum sum;
            RGBTRIPLE rgb;

            //In normal 9x9 situation
            if (h > 0 && w > 0 && w < (width - 1) && h < (height - 1))
            {
                sum = _9ave(height, width, h, w, imgCopy);
            }

            //In case of a superior left corner situation
            if (h == 0 && w == 0)
            {
                sum = slcave(height, width, h, w, imgCopy);
            }

            //In case of a superior right corner
            if (h == 0 && w == (width - 1))
            {
                sum = srcave(height, width, h, w, imgCopy);
            }

            //In case of a inferior left corner
            if (h == (height - 1) && w == 0)
            {
                sum = ilcave(height, width, h, w, imgCopy);
            }

            //In case od a inferior right corner
            if (h == (height - 1) && w == (width - 1))
            {
                sum = ircave(height, width, h, w, imgCopy);
            }

            //In case of a inferior corner
            if (h == (height - 1) && w < (width - 1) && w > 0)
            {
                sum = icave(height, width, h, w, imgCopy);
            }

            //In case of a left corner
            if (w == 0 && h < (height - 1) && h > 0)
            {
                sum = lcave(height, width, h, w, imgCopy);
            }

            //In case of a right corner
            if (w == (width - 1) && h < (height - 1) && h > 0)
            {
                sum = rcave(height, width, h, w, imgCopy);
            }

            //In case of a superior corner

            if (h == 0 && w < (width - 1) && w > 0)
            {
                sum = scave(height, width, h, w, imgCopy);
            }

            BYTE baverage = round(sum.bsum);
            BYTE raverage = round(sum.rsum);
            BYTE gaverage = round(sum.gsum);
            BYTE *ggg = &image[h][w].rgbtGreen;
            BYTE *bbb = &image[h][w].rgbtBlue;
            BYTE *rrr = &image[h][w].rgbtRed;
            *ggg = gaverage;
            *bbb = baverage;
            *rrr = raverage;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imgCp[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            imgCp[h][w] = image[h][w];
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            //Takes the values of all the pixels
            RGBTRIPLE x1 = imgCp[h-1][w-1];
            RGBTRIPLE x2 = imgCp[h-1][w];
            RGBTRIPLE x3 = imgCp[h-1][w+1];
            RGBTRIPLE x4 = imgCp[h][w-1];
            RGBTRIPLE x6 = imgCp[h][w+1];
            RGBTRIPLE x7 = imgCp[h+1][w-1];
            RGBTRIPLE x8 = imgCp[h+1][w];
            RGBTRIPLE x9 = imgCp[h+1][w+1];
            double gxr;
            double gyr;
            double gxb;
            double gyb;
            double gxg;
            double gyg;

            //pixels on the middle
            if(w > 0 && w < (width - 1) && h > 0 && h < (height - 1))
            {
                gxr = _gx(x1.rgbtRed, x3.rgbtRed, x4.rgbtRed, x6.rgbtRed, x7.rgbtRed, x9.rgbtRed);
                gyr = _gy(x1.rgbtRed, x2.rgbtRed, x3.rgbtRed, x7.rgbtRed, x8.rgbtRed, x9.rgbtRed);
                gxg = _gx(x1.rgbtGreen, x3.rgbtGreen, x4.rgbtGreen, x6.rgbtGreen, x7.rgbtGreen, x9.rgbtGreen);
                gyg = _gy(x1.rgbtGreen, x2.rgbtGreen, x3.rgbtGreen, x7.rgbtGreen, x8.rgbtGreen, x9.rgbtGreen);
                gxb = _gx(x1.rgbtBlue, x3.rgbtBlue, x4.rgbtBlue, x6.rgbtBlue, x7.rgbtBlue, x9.rgbtBlue);
                gyb = _gy(x1.rgbtBlue, x2.rgbtBlue, x3.rgbtBlue, x7.rgbtBlue, x8.rgbtBlue, x9.rgbtBlue);
            }

            //pixels in the corners
            if(w == 0 && h == 0)
            {
                gxr = _gx(0, 0, 0, x6.rgbtRed, 0, x9.rgbtRed);
                gyr = _gy(0, 0, 0, 0, x8.rgbtRed, x9.rgbtRed);
                gxg = _gx(0, 0, 0, x6.rgbtGreen, 0, x9.rgbtGreen);
                gyg = _gy(0, 0, 0, 0, x8.rgbtGreen, x9.rgbtGreen);
                gxb = _gx(0, 0, 0, x6.rgbtBlue, 0, x9.rgbtBlue);
                gyb = _gy(0, 0, 0, 0, x8.rgbtBlue, x9.rgbtBlue);
            }

            if(w == (width - 1) && h == 0)
            {
                gxr = _gx(0, 0, x4.rgbtRed, 0, x7.rgbtRed, 0);
                gyr = _gy(0, 0, 0, x7.rgbtRed, x8.rgbtRed, 0);
                gxg = _gx(0, 0, x4.rgbtGreen, 0, x7.rgbtGreen, 0);
                gyg = _gy(0, 0, 0, x7.rgbtGreen, x8.rgbtGreen, 0);
                gxb = _gx(0, 0, x4.rgbtBlue, 0, x7.rgbtBlue, 0);
                gyb = _gy(0, 0, 0, x7.rgbtBlue, x8.rgbtBlue, 0);
            }

            if(w == 0 && h == (height - 1))
            {
                gxr = _gx(0, x3.rgbtRed, 0, x6.rgbtRed, 0, 0);
                gyr = _gy(0, x2.rgbtRed, x3.rgbtRed, 0, 0, 0);
                gxg = _gx(0, x3.rgbtGreen, 0, x6.rgbtGreen, 0, 0);
                gyg = _gy(0, x2.rgbtGreen, x3.rgbtGreen, 0, 0, 0);
                gxb = _gx(0, x3.rgbtBlue, 0, x6.rgbtBlue, 0, 0);
                gyb = _gy(0, x2.rgbtBlue, x3.rgbtBlue, 0, 0, 0);
            }

            if(w == (width - 1) && h == (height - 1))
            {
                gxr = _gx(x1.rgbtRed, 0, x4.rgbtRed, 0, 0, 0);
                gyr = _gy(x1.rgbtRed, x2.rgbtRed, 0, 0, 0, 0);
                gxg = _gx(x1.rgbtGreen, 0, x4.rgbtGreen, 0, 0, 0);
                gyg = _gy(x1.rgbtGreen, x2.rgbtGreen, 0, 0, 0, 0);
                gxb = _gx(x1.rgbtBlue, 0, x4.rgbtBlue, 0, 0, 0);
                gyb = _gy(x1.rgbtBlue, x2.rgbtBlue, 0, 0, 0, 0);
            }

            //pixels on the edges
            if(w == 0 && h < (height - 1) && h > 0)
            {
                gxr = _gx(0, x3.rgbtRed, 0, x6.rgbtRed, 0, x9.rgbtRed);
                gyr = _gy(0, x2.rgbtRed, x3.rgbtRed, 0, x8.rgbtRed, x9.rgbtRed);
                gxg = _gx(0, x3.rgbtGreen, 0, x6.rgbtGreen, 0, x9.rgbtGreen);
                gyg = _gy(0, x2.rgbtGreen, x3.rgbtGreen, 0, x8.rgbtGreen, x9.rgbtGreen);
                gxb = _gx(0, x3.rgbtBlue, 0, x6.rgbtBlue, 0, x9.rgbtBlue);
                gyb = _gy(0, x2.rgbtBlue, x3.rgbtBlue, 0, x8.rgbtBlue, x9.rgbtBlue);
            }

            if(w < (width - 1) && w > 0 && h == (height - 1))
            {
                gxr = _gx(x1.rgbtRed, x3.rgbtRed, x4.rgbtRed, x6.rgbtRed, 0, 0);
                gyr = _gy(x1.rgbtRed, x2.rgbtRed, x3.rgbtRed, 0, 0, 0);
                gxg = _gx(x1.rgbtGreen, x3.rgbtGreen, x4.rgbtGreen, x6.rgbtGreen, 0, 0);
                gyg = _gy(x1.rgbtGreen, x2.rgbtGreen, x3.rgbtGreen, 0, 0, 0);
                gxb = _gx(x1.rgbtBlue, x3.rgbtBlue, x4.rgbtBlue, x6.rgbtBlue, 0, 0);
                gyb = _gy(x1.rgbtBlue, x2.rgbtBlue, x3.rgbtBlue, 0, 0, 0);
            }

            if(w == (width - 1) && h < (height - 1) && h > 0)
            {
                gxr = _gx(x1.rgbtRed, 0, x4.rgbtRed, 0, x7.rgbtRed, 0);
                gyr = _gy(x1.rgbtRed, x2.rgbtRed, 0, x7.rgbtRed, x8.rgbtRed, 0);
                gxg = _gx(x1.rgbtGreen, 0, x4.rgbtGreen, 0, x7.rgbtGreen, 0);
                gyg = _gy(x1.rgbtGreen, x2.rgbtGreen, 0, x7.rgbtGreen, x8.rgbtGreen, 0);
                gxb = _gx(x1.rgbtBlue, 0, x4.rgbtBlue, 0, x7.rgbtBlue, 0);
                gyb = _gy(x1.rgbtBlue, x2.rgbtBlue, 0, x7.rgbtBlue, x8.rgbtBlue, 0);
            }

            if(w < (width - 1) && w > 0 && h == 0)
            {
                gxr = _gx(0, 0, x4.rgbtRed, x6.rgbtRed, x7.rgbtRed, x9.rgbtRed);
                gyr = _gy(0, 0, 0, x7.rgbtRed, x8.rgbtRed, x9.rgbtRed);
                gxg = _gx(0, 0, x4.rgbtGreen, x6.rgbtGreen, x7.rgbtGreen, x9.rgbtGreen);
                gyg = _gy(0, 0, 0, x7.rgbtGreen, x8.rgbtGreen, x9.rgbtGreen);
                gxb = _gx(0, 0, x4.rgbtBlue, x6.rgbtBlue, x7.rgbtBlue, x9.rgbtBlue);
                gyb = _gy(0, 0, 0, x7.rgbtBlue, x8.rgbtBlue, x9.rgbtBlue);
            }

            double bgrt = gxb + gyb;
            double rgrt = gxr + gyr;
            double ggrt = gxg + gyg;

            if(bgrt > 65025)
                bgrt = 65025;
            if(rgrt > 65025)
                rgrt = 65025;
            if(ggrt > 65025)
                ggrt = 65025;

            BYTE bluev = round(sqrt(bgrt));
            BYTE greenv = round(sqrt(ggrt));
            BYTE redv = round(sqrt(rgrt));

            BYTE *ggg = &image[h][w].rgbtGreen;
            BYTE *bbb = &image[h][w].rgbtBlue;
            BYTE *rrr = &image[h][w].rgbtRed;

            *bbb = bluev;
            *ggg = greenv;
            *rrr = redv;

        }
    }

    return;
}

//Takes the 9x9 sum
rgbsum _9ave(int height, int width, int h, int w, RGBTRIPLE image[height][width])
{
    rgbsum rgb;
    int bsum = 0;
    int rsum = 0;
    int gsum = 0;

    //Takes the blue, red and green pixels sum separadely
    for (int h1 = (h - 1); h1 <= (h + 1); h1++)
    {
        for (int w1 = (w - 1); w1 <= (w + 1); w1++)
        {
            BYTE *b = &image[h1][w1].rgbtBlue;
            BYTE *r = &image[h1][w1].rgbtRed;
            BYTE *g = &image[h1][w1].rgbtGreen;
            bsum = (bsum + *b);
            rsum = (rsum + *r);
            gsum = (gsum + *g);
        }
    }
    rgb.bsum = (bsum / 9.0);
    rgb.gsum = (gsum / 9.0);
    rgb.rsum = (rsum / 9.0);
    return rgb;
}

//Takes the edges sum
rgbsum slcave(int height, int width, int h, int w, RGBTRIPLE image[height][width])
{
    rgbsum rgb;
    int bsum = 0;
    int rsum = 0;
    int gsum = 0;

    for (int h1 = 0; h1 <= 1; h1++)
    {
        for (int w1 = 0; w1 <= 1; w1++)
        {
            BYTE *b = &image[h1][w1].rgbtBlue;
            BYTE *r = &image[h1][w1].rgbtRed;
            BYTE *g = &image[h1][w1].rgbtGreen;
            bsum = (bsum + *b);
            rsum = (rsum + *r);
            gsum = (gsum + *g);
        }
    }
    rgb.bsum = (bsum / 4.0);
    rgb.gsum = (gsum / 4.0);
    rgb.rsum = (rsum / 4.0);

    return rgb;
}

rgbsum srcave(int height, int width, int h, int w, RGBTRIPLE image[height][width])
{
    rgbsum rgb;
    int bsum = 0;
    int rsum = 0;
    int gsum = 0;

    for (int h1 = h; h1 <= (h + 1); h1++)
    {
        for (int w1 = (w - 1); w1 <= w; w1++)
        {
            BYTE *b = &image[h1][w1].rgbtBlue;
            BYTE *r = &image[h1][w1].rgbtRed;
            BYTE *g = &image[h1][w1].rgbtGreen;
            bsum = (bsum + *b);
            rsum = (rsum + *r);
            gsum = (gsum + *g);
        }
    }
    rgb.bsum = (bsum / 4.0);
    rgb.gsum = (gsum / 4.0);
    rgb.rsum = (rsum / 4.0);

    return rgb;
}

rgbsum ilcave(int height, int width, int h, int w, RGBTRIPLE image[height][width])
{
    rgbsum rgb;
    int bsum = 0;
    int rsum = 0;
    int gsum = 0;

    for (int h1 = (h - 1); h1 < height; h1++)
    {
        for (int w1 = w; w1 <= 1; w1++)
        {
            BYTE *b = &image[h1][w1].rgbtBlue;
            BYTE *r = &image[h1][w1].rgbtRed;
            BYTE *g = &image[h1][w1].rgbtGreen;
            bsum = (bsum + *b);
            rsum = (rsum + *r);
            gsum = (gsum + *g);
        }
    }
    rgb.bsum = (bsum / 4.0);
    rgb.gsum = (gsum / 4.0);
    rgb.rsum = (rsum / 4.0);

    return rgb;
}

rgbsum ircave(int height, int width, int h, int w, RGBTRIPLE image[height][width])
{
    rgbsum rgb;
    int bsum = 0;
    int rsum = 0;
    int gsum = 0;

    for (int h1 = (h - 1); h1 <= h; h1++)
    {
        for (int w1 = (w - 1); w1 <= w; w1++)
        {
            BYTE *b = &image[h1][w1].rgbtBlue;
            BYTE *r = &image[h1][w1].rgbtRed;
            BYTE *g = &image[h1][w1].rgbtGreen;
            bsum = (bsum + *b);
            rsum = (rsum + *r);
            gsum = (gsum + *g);
        }
    }
    rgb.bsum = (bsum / 4.0);
    rgb.gsum = (gsum / 4.0);
    rgb.rsum = (rsum / 4.0);

    return rgb;
}

//Takes the sum of the corners
rgbsum icave(int height, int width, int h, int w, RGBTRIPLE image[height][width])
{
    rgbsum rgb;
    int bsum = 0;
    int rsum = 0;
    int gsum = 0;

    for (int h1 = (h - 1); h1 <= h; h1++)
    {
        for (int w1 = (w - 1); w1 <= (w + 1); w1++)
        {
            BYTE *b = &image[h1][w1].rgbtBlue;
            BYTE *r = &image[h1][w1].rgbtRed;
            BYTE *g = &image[h1][w1].rgbtGreen;
            bsum = (bsum + *b);
            rsum = (rsum + *r);
            gsum = (gsum + *g);
        }
    }
    rgb.bsum = (bsum / 6.0);
    rgb.gsum = (gsum / 6.0);
    rgb.rsum = (rsum / 6.0);

    return rgb;
}

rgbsum lcave(int height, int width, int h, int w, RGBTRIPLE image[height][width])
{
    rgbsum rgb;
    int bsum = 0;
    int rsum = 0;
    int gsum = 0;

    for (int h1 = (h - 1); h1 <= (h + 1); h1++)
    {
        for (int w1 = w; w1 <= (w + 1); w1++)
        {
            BYTE *b = &image[h1][w1].rgbtBlue;
            BYTE *r = &image[h1][w1].rgbtRed;
            BYTE *g = &image[h1][w1].rgbtGreen;
            bsum = (bsum + *b);
            rsum = (rsum + *r);
            gsum = (gsum + *g);
        }
    }
    rgb.bsum = (bsum / 6.0);
    rgb.gsum = (gsum / 6.0);
    rgb.rsum = (rsum / 6.0);

    return rgb;
}

rgbsum rcave(int height, int width, int h, int w, RGBTRIPLE image[height][width])
{
    rgbsum rgb;
    int bsum = 0;
    int rsum = 0;
    int gsum = 0;

    for (int h1 = (h - 1); h1 <= (h + 1); h1++)
    {
        for (int w1 = (w - 1); w1 <= w; w1++)
        {
            BYTE *b = &image[h1][w1].rgbtBlue;
            BYTE *r = &image[h1][w1].rgbtRed;
            BYTE *g = &image[h1][w1].rgbtGreen;
            bsum = (bsum + *b);
            rsum = (rsum + *r);
            gsum = (gsum + *g);
        }
    }
    rgb.bsum = (bsum / 6.0);
    rgb.gsum = (gsum / 6.0);
    rgb.rsum = (rsum / 6.0);

    return rgb;
}

rgbsum scave(int height, int width, int h, int w, RGBTRIPLE image[height][width])
{
    rgbsum rgb;
    int bsum = 0;
    int rsum = 0;
    int gsum = 0;

    for (int h1 = 0; h1 <= 1; h1++)
    {
        for (int w1 = (w - 1); w1 <= (w + 1); w1++)
        {
            BYTE b = image[h1][w1].rgbtBlue;
            BYTE r = image[h1][w1].rgbtRed;
            BYTE g = image[h1][w1].rgbtGreen;
            bsum = (bsum + b);
            rsum = (rsum + r);
            gsum = (gsum + g);
        }
    }
    rgb.bsum = (bsum / 6.0);
    rgb.gsum = (gsum / 6.0);
    rgb.rsum = (rsum / 6.0);

    return rgb;
}

//Calculates the gx and the gy
double _gx(BYTE x1, BYTE x3, BYTE x4, BYTE x6, BYTE x7, BYTE x9)
{
    int gxt = ((-1) * x1) + x3 + ((-2) * x4) + (2 * x6) + ((-1) * x7) + x9;
    double gxxt = gxt * gxt;

    return gxxt;
}

double _gy(BYTE x1, BYTE x2, BYTE x3, BYTE x7, BYTE x8, BYTE x9)
{
    int gyt = ((-1) * x1) + ((-2) * x2) + ((-1) * x3) + x7 + (2 * x8) + x9;
    double gyyt = gyt * gyt;

    return gyyt;
}