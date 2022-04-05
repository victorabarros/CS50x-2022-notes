#include <stdio.h>
#include "helpers.h"
#include <math.h>

// make filter && ./filter -g images/yard.bmp out.bmp

double min(double a, double b)
{
    if (a < b)
    {
        return a;
    }
    else
    {
        return b;
    }
}
RGBTRIPLE rgbAverage9(RGBTRIPLE val1, RGBTRIPLE val2, RGBTRIPLE val3, RGBTRIPLE val4, RGBTRIPLE val5, RGBTRIPLE val6,
                      RGBTRIPLE val7, RGBTRIPLE val8, RGBTRIPLE val9)
{
    RGBTRIPLE resp;

    double red = (val1.rgbtRed + val2.rgbtRed + val3.rgbtRed + val4.rgbtRed + val5.rgbtRed + val6.rgbtRed
                  + val7.rgbtRed + val8.rgbtRed + val9.rgbtRed) / 9.;
    resp.rgbtRed = (uint8_t)round(red);

    double green = (val1.rgbtGreen + val2.rgbtGreen + val3.rgbtGreen + val4.rgbtGreen + val5.rgbtGreen + val6.rgbtGreen + val7.rgbtGreen
                    + val8.rgbtGreen + val9.rgbtGreen) / 9.;
    resp.rgbtGreen = (uint8_t)round(green);

    double blue = (val1.rgbtBlue + val2.rgbtBlue + val3.rgbtBlue + val4.rgbtBlue + val5.rgbtBlue + val6.rgbtBlue + val7.rgbtBlue
                   + val8.rgbtBlue + val9.rgbtBlue) / 9.;
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
        resp[ii][0] = rgbAverage6(image[ii - 1][0], image[ii - 1][1],
                                  image[ii][0], image[ii][1],
                                  image[ii + 1][0], image[ii + 1][1]);

        // center
        for (int jj = 1; jj < width - 1; jj++)
        {
            resp[ii][jj] = rgbAverage9(image[ii - 1][jj - 1], image[ii - 1][jj], image[ii - 1][jj + 1],
                                       image[ii][jj - 1], image[ii][jj], image[ii][jj + 1],
                                       image[ii + 1][jj - 1], image[ii + 1][jj], image[ii + 1][jj + 1]);
        }

        // jj = width - 1; aka right column
        resp[ii][width - 1] = rgbAverage6(image[ii - 1][width - 2], image[ii - 1][width - 1],
                                          image[ii][width - 2], image[ii][width - 1],
                                          image[ii + 1][width - 2], image[ii + 1][width - 1]);
    }

    // top line
    for (int jj = 1; jj < width - 1; jj++)
    {
        resp[0][jj] = rgbAverage6(image[0][jj - 1], image[0][jj], image[0][jj + 1],
                                  image[1][jj - 1], image[1][jj], image[1][jj + 1]);
    }

    // bottom line
    for (int jj = 1; jj < width - 1; jj++)
    {
        // printf("%i %i\n", width, jj);
        resp[height - 1][jj] = rgbAverage6(image[height - 2][jj - 1], image[height - 2][jj], image[height - 2][jj + 1],
                                           image[height - 1][jj - 1], image[height - 1][jj], image[width - 1][jj + 1]);
    }

    // corners
    // 0 0
    resp[0][0] = rgbAverage4(image[0][0], image[0][1], image[1][0], image[1][1]);
    // 0 width
    resp[0][width - 1] = rgbAverage4(image[0][width - 2], image[0][width - 1], image[1][width - 2], image[1][width - 1]);
    // height width
    resp[height - 1][width - 1] = rgbAverage4(image[height - 2][width - 2], image[height - 2][width - 1], image[height - 1][width - 2],
                                  image[height - 1][width - 1]);
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

double sobel_operator_x(uint8_t pixel00, uint8_t pixel01, uint8_t pixel02,
                        uint8_t pixel10, uint8_t pixel11, uint8_t pixel12,
                        uint8_t pixel20, uint8_t pixel21, uint8_t pixel22)
{
    return (-1) * pixel00 + (0) * pixel01 + (+1) * pixel02 +
           (-2) * pixel10 + (0) * pixel11 + (+2) * pixel12 +
           (-1) * pixel20 + (0) * pixel21 + (+1) * pixel22;
}

double sobel_operator_y(uint8_t pixel00, uint8_t pixel01, uint8_t pixel02,
                        uint8_t pixel10, uint8_t pixel11, uint8_t pixel12,
                        uint8_t pixel20, uint8_t pixel21, uint8_t pixel22)
{
    return (-1) * pixel00 + (-2) * pixel01 + (-1) * pixel02 +
           (0) * pixel10 + (0) * pixel11 + (0) * pixel12 +
           (+1) * pixel20 + (+2) * pixel21 + (+1) * pixel22;
}

RGBTRIPLE sobel_operator(RGBTRIPLE pixel00, RGBTRIPLE pixel01, RGBTRIPLE pixel02,
                         RGBTRIPLE pixel10, RGBTRIPLE pixel11, RGBTRIPLE pixel12,
                         RGBTRIPLE pixel20, RGBTRIPLE pixel21, RGBTRIPLE pixel22)
{
    RGBTRIPLE resp;

    double sobel_operator_x_red = sobel_operator_x(pixel00.rgbtRed, pixel01.rgbtRed, pixel02.rgbtRed,
                                  pixel10.rgbtRed, pixel11.rgbtRed, pixel12.rgbtRed,
                                  pixel20.rgbtRed, pixel21.rgbtRed, pixel22.rgbtRed);
    double sobel_operator_y_red = sobel_operator_y(pixel00.rgbtRed, pixel01.rgbtRed, pixel02.rgbtRed,
                                  pixel10.rgbtRed, pixel11.rgbtRed, pixel12.rgbtRed,
                                  pixel20.rgbtRed, pixel21.rgbtRed, pixel22.rgbtRed);
    double red = sqrt(pow(sobel_operator_x_red, 2.) + pow(sobel_operator_y_red, 2.));
    resp.rgbtRed = (uint8_t)round(min(red, 255));

    double sobel_operator_x_green = sobel_operator_x(pixel00.rgbtGreen, pixel01.rgbtGreen, pixel02.rgbtGreen,
                                    pixel10.rgbtGreen, pixel11.rgbtGreen, pixel12.rgbtGreen,
                                    pixel20.rgbtGreen, pixel21.rgbtGreen, pixel22.rgbtGreen);
    double sobel_operator_y_green = sobel_operator_y(pixel00.rgbtGreen, pixel01.rgbtGreen, pixel02.rgbtGreen,
                                    pixel10.rgbtGreen, pixel11.rgbtGreen, pixel12.rgbtGreen,
                                    pixel20.rgbtGreen, pixel21.rgbtGreen, pixel22.rgbtGreen);
    double green = sqrt(pow(sobel_operator_x_green, 2.) + pow(sobel_operator_y_green, 2.));
    resp.rgbtGreen = (uint8_t)round(min(green, 255));

    double sobel_operator_x_blue = sobel_operator_x(pixel00.rgbtBlue, pixel01.rgbtBlue, pixel02.rgbtBlue,
                                   pixel10.rgbtBlue, pixel11.rgbtBlue, pixel12.rgbtBlue,
                                   pixel20.rgbtBlue, pixel21.rgbtBlue, pixel22.rgbtBlue);
    double sobel_operator_y_blue = sobel_operator_y(pixel00.rgbtBlue, pixel01.rgbtBlue, pixel02.rgbtBlue,
                                   pixel10.rgbtBlue, pixel11.rgbtBlue, pixel12.rgbtBlue,
                                   pixel20.rgbtBlue, pixel21.rgbtBlue, pixel22.rgbtBlue);
    double blue = sqrt(pow(sobel_operator_x_blue, 2.) + pow(sobel_operator_y_blue, 2.));
    resp.rgbtBlue = (uint8_t)round(min(blue, 255));

    return resp;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE resp[height][width];
    RGBTRIPLE black;
    black.rgbtRed = 0x00;
    black.rgbtGreen = 0x00;
    black.rgbtBlue = 0x00;

    for (int ii = 1; ii < height - 1; ii++)
    {
        // jj = 0; aka left column
        resp[ii][0] = sobel_operator(black, image[ii - 1][0], image[ii - 1][1],
                                     black, image[ii][0], image[ii][1],
                                     black, image[ii + 1][0], image[ii + 1][1]);

        // center
        for (int jj = 1; jj < width - 1; jj++)
        {
            resp[ii][jj] = sobel_operator(image[ii - 1][jj - 1], image[ii - 1][jj], image[ii - 1][jj + 1],
                                          image[ii][jj - 1], image[ii][jj], image[ii][jj + 1],
                                          image[ii + 1][jj - 1], image[ii + 1][jj], image[ii + 1][jj + 1]);
        }

        // jj = width - 1; aka right column
        resp[ii][width - 1] = sobel_operator(image[ii - 1][width - 2], image[ii - 1][width - 1], black,
                                             image[ii][width - 2], image[ii][width - 1], black,
                                             image[ii + 1][width - 2], image[ii + 1][width - 1], black);
    }

    // top line
    for (int jj = 1; jj < width - 1; jj++)
    {
        resp[0][jj] = sobel_operator(black, black, black,
                                     image[0][jj - 1], image[0][jj], image[0][jj + 1],
                                     image[1][jj - 1], image[1][jj], image[1][jj + 1]);
    }

    // bottom line
    for (int jj = 1; jj < width - 1; jj++)
    {
        // printf("%i %i\n", width, jj);
        resp[height - 1][jj] = sobel_operator(image[height - 2][jj - 1], image[height - 2][jj], image[height - 2][jj + 1],
                                              image[height - 1][jj - 1], image[height - 1][jj], image[width - 1][jj + 1],
                                              black, black, black);
    }

    // corners
    // 0 0
    resp[0][0] = sobel_operator(black, black, black,
                                black, image[0][0], image[0][1],
                                black, image[1][0], image[1][1]);
    // 0 width
    resp[0][width - 1] = sobel_operator(black, black, black,
                                        image[0][width - 2], image[0][width - 1], black,
                                        image[1][width - 2], image[1][width - 1], black);
    // height width
    resp[height - 1][width - 1] = sobel_operator(image[height - 2][width - 2], image[height - 2][width - 1], black,
                                  image[height - 1][width - 2], image[height - 1][width - 1], black,
                                  black, black, black);
    // height 0
    resp[height - 1][0] = sobel_operator(black, image[height - 2][0], image[height - 2][1],
                                         black, image[height - 1][0], image[height - 1][1],
                                         black, black, black);

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
