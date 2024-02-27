#include "helpers.h"
#include <math.h>

typedef struct
{
    float bsum;
    float gsum;
    float rsum;
}
rgbsum;

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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int oRED = image[h][w].rgbtRed;
            int oGREEN = image[h][w].rgbtGreen;
            int oBLUE = image[h][w].rgbtBlue;

            float sRED = (0.393 * oRED) + (0.769 * oGREEN) + (0.189 * oBLUE);
            float sGREEN = (0.349 * oRED) + (0.686 * oGREEN) + (0.168 * oBLUE);
            float sBLUE = (0.272 * oRED) + (0.534 * oGREEN) + (0.131 * oBLUE);

            if (sRED > 255.49)
                sRED = 255;
            if (sGREEN > 255.49)
                sGREEN = 255;
            if (sBLUE > 255.49)
                sBLUE = 255;
            BYTE *rrr = &image[h][w].rgbtRed;
            BYTE *ggg = &image[h][w].rgbtGreen;
            BYTE *bbb = &image[h][w].rgbtBlue;

            *rrr = round(sRED);
            *ggg = round(sGREEN);
            *bbb = round(sBLUE);
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