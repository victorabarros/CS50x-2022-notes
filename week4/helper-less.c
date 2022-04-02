#include <stdio.h>
#include "helpers.h"
#include <math.h>

int min(int a, int b)
{
    if (a < b)
        return a;
    else
        return b;
}
RGBTRIPLE rgbAverage9(RGBTRIPLE val1, RGBTRIPLE val2, RGBTRIPLE val3, RGBTRIPLE val4, RGBTRIPLE val5, RGBTRIPLE val6, RGBTRIPLE val7, RGBTRIPLE val8, RGBTRIPLE val9)
{
    RGBTRIPLE resp;

    double red = (val1.rgbtRed + val2.rgbtRed + val3.rgbtRed + val4.rgbtRed + val5.rgbtRed + val6.rgbtRed + val7.rgbtRed + val8.rgbtRed + val9.rgbtRed) / 9.;
    resp.rgbtRed = (uint8_t)round(red);

    double green = (val1.rgbtGreen + val2.rgbtGreen + val3.rgbtGreen + val4.rgbtGreen + val5.rgbtGreen + val6.rgbtGreen + val7.rgbtGreen + val8.rgbtGreen + val9.rgbtGreen) / 9.;
    resp.rgbtGreen = (uint8_t)round(green);

    double blue = (val1.rgbtBlue + val2.rgbtBlue + val3.rgbtBlue + val4.rgbtBlue + val5.rgbtBlue + val6.rgbtBlue + val7.rgbtBlue + val8.rgbtBlue + val9.rgbtBlue) / 9.;
    resp.rgbtBlue = (uint8_t)round(blue);

    return resp;
}
RGBTRIPLE rgbAverage6(RGBTRIPLE val1, RGBTRIPLE val2, RGBTRIPLE val3, RGBTRIPLE val4, RGBTRIPLE val5, RGBTRIPLE val6)
{
    RGBTRIPLE resp;

    double red = (val1.rgbtRed + val2.rgbtRed + val3.rgbtRed + val4.rgbtRed + val5.rgbtRed + val6.rgbtRed) / 6.;
    resp.rgbtRed = (uint8_t)round(red);

    double green = (val1.rgbtGreen + val2.rgbtGreen + val3.rgbtGreen + val4.rgbtGreen + val5.rgbtGreen + val6.rgbtGreen) / 6.;
    resp.rgbtGreen = (uint8_t)round(green);

    double blue = (val1.rgbtBlue + val2.rgbtBlue + val3.rgbtBlue + val4.rgbtBlue + val5.rgbtBlue + val6.rgbtBlue) / 6.;
    resp.rgbtBlue = (uint8_t)round(blue);

    return resp;
}
RGBTRIPLE rgbAverage4(RGBTRIPLE val1, RGBTRIPLE val2, RGBTRIPLE val3, RGBTRIPLE val4)
{
    RGBTRIPLE resp;

    double red = (val1.rgbtRed + val2.rgbtRed + val3.rgbtRed + val4.rgbtRed) / 4.;
    resp.rgbtRed = (uint8_t)round(red);

    double green = (val1.rgbtGreen + val2.rgbtGreen + val3.rgbtGreen + val4.rgbtGreen) / 4.;
    resp.rgbtGreen = (uint8_t)round(green);

    double blue = (val1.rgbtBlue + val2.rgbtBlue + val3.rgbtBlue + val4.rgbtBlue) / 4.;
    resp.rgbtBlue = (uint8_t)round(blue);

    return resp;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int ii = 0; ii < height; ii++)
    {
        for (int jj = 0; jj < width; jj++)
        {
            uint8_t red = image[ii][jj].rgbtRed;
            uint8_t green = image[ii][jj].rgbtGreen;
            uint8_t blue = image[ii][jj].rgbtBlue;

            double average = (red + green + blue) / 3.;

            image[ii][jj].rgbtRed = (uint8_t)round(average);
            image[ii][jj].rgbtGreen = (uint8_t)round(average);
            image[ii][jj].rgbtBlue = (uint8_t)round(average);
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int ii = 0; ii < height; ii++)
    {
        for (int jj = 0; jj < width; jj++)
        {
            double sepiaRed = .393 * image[ii][jj].rgbtRed +
                              .769 * image[ii][jj].rgbtGreen +
                              .189 * image[ii][jj].rgbtBlue;
            double sepiaGreen = .349 * image[ii][jj].rgbtRed +
                                .686 * image[ii][jj].rgbtGreen +
                                .168 * image[ii][jj].rgbtBlue;
            double sepiaBlue = .272 * image[ii][jj].rgbtRed +
                               .534 * image[ii][jj].rgbtGreen +
                               .131 * image[ii][jj].rgbtBlue;

            int newRed = round(sepiaRed);
            int newGreen = round(sepiaGreen);
            int newBlue = round(sepiaBlue);

            newRed = min(newRed, 255);
            newGreen = min(newGreen, 255);
            newBlue = min(newBlue, 255);

            image[ii][jj].rgbtRed = newRed;
            image[ii][jj].rgbtGreen = newGreen;
            image[ii][jj].rgbtBlue = newBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int ii = 0; ii < height; ii++)
    {
        RGBTRIPLE row[width];
        for (int jj = 0; jj < width; jj++)
        {
            row[width - jj - 1] = image[ii][jj];
        }

        for (int jj = 0; jj < width; jj++)
        {
            image[ii][jj] = row[jj];
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE resp[height][width];

    for (int ii = 1; ii < height - 1; ii++)
    {
        // jj = 0; aka left column
        resp[ii][0] = rgbAverage6(
            image[ii - 1][0], image[ii - 1][1],
            image[ii][0], image[ii][1],
            image[ii + 1][0], image[ii + 1][1]);

        // center
        for (int jj = 1; jj < width - 1; jj++)
        {
            resp[ii][jj] = rgbAverage9(
                image[ii - 1][jj - 1], image[ii - 1][jj], image[ii - 1][jj + 1],
                image[ii][jj - 1], image[ii][jj], image[ii][jj + 1],
                image[ii + 1][jj - 1], image[ii + 1][jj], image[ii + 1][jj + 1]);
        }

        // jj = width - 1; aka right column
        resp[ii][width - 1] = rgbAverage6(
            image[ii - 1][width - 2], image[ii - 1][width - 1],
            image[ii][width - 2], image[ii][width - 1],
            image[ii + 1][width - 2], image[ii + 1][width - 1]);
    }

    // top line
    for (int jj = 1; jj < width - 1; jj++)
    {
        resp[0][jj] = rgbAverage6(
            image[0][jj - 1], image[0][jj], image[0][jj + 1],
            image[1][jj - 1], image[1][jj], image[1][jj + 1]);
    }

    // bottom line
    for (int jj = 1; jj < width - 1; jj++)
    {
        // printf("%i %i\n", width, jj);
        resp[height - 1][jj] = rgbAverage6(
            image[height - 2][jj - 1], image[height - 2][jj], image[height - 2][jj + 1],
            image[height - 1][jj - 1], image[height - 1][jj], image[width - 1][jj + 1]);
    }

    // corners
    // 0 0
    resp[0][0] = rgbAverage4(image[0][0], image[0][1], image[1][0], image[1][1]);
    // 0 width
    resp[0][width - 1] = rgbAverage4(image[0][width - 2], image[0][width - 1], image[1][width - 2], image[1][width - 1]);
    // height width
    resp[height - 1][width - 1] = rgbAverage4(image[height - 2][width - 2], image[height - 2][width - 1], image[height - 1][width - 2], image[height - 1][width - 1]);
    // height 0
    resp[height - 1][0] = rgbAverage4(image[height - 2][0], image[height - 2][1], image[height - 1][0], image[height - 1][1]);

    // cp
    for (int ii = 0; ii < height; ii++)
    {
        for (int jj = 0; jj < width; jj++)
        {
            image[ii][jj] = resp[ii][jj];
        }
    }

    return;
}
