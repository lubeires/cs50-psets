#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE *red, *green, *blue;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            red = &image[i][j].rgbtRed;
            green = &image[i][j].rgbtGreen;
            blue = &image[i][j].rgbtBlue;
            *red = *green = *blue = round((*red + *green + *blue) / 3.0);
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int red, green, blue, sepiaRed, sepiaGreen, sepiaBlue;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            red = image[i][j].rgbtRed;
            green = image[i][j].rgbtGreen;
            blue = image[i][j].rgbtBlue;

            sepiaRed = round(.393 * red + .769 * green + .189 * blue);
            sepiaGreen = round(.349 * red + .686 * green + .168 * blue);
            sepiaBlue = round(.272 * red + .534 * green + .131 * blue);

            image[i][j].rgbtRed = sepiaRed < 255 ? sepiaRed : 255;
            image[i][j].rgbtGreen = sepiaGreen < 255 ? sepiaGreen : 255;
            image[i][j].rgbtBlue = sepiaBlue < 255 ? sepiaBlue : 255;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_pixel;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            temp_pixel = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp_pixel;
        }
    }
    return;
}

// Blur pixel
void blur_pixel(int start_row, int end_row, int start_col, int end_col, int row_index, int col_index, int height, int width,
                RGBTRIPLE image[height][width], RGBTRIPLE temp_image[height][width])
{
    int red, green, blue;
    float cont;
    red = green = blue = cont = 0;
    for (int i = start_row; i <= end_row; i++)
    {
        for (int j = start_col; j <= end_col; j++)
        {
            red += image[row_index + i][col_index + j].rgbtRed;
            green += image[row_index + i][col_index + j].rgbtGreen;
            blue += image[row_index + i][col_index + j].rgbtBlue;
            cont++;
        }
    }

    temp_image[row_index][col_index].rgbtRed = round(red / cont);
    temp_image[row_index][col_index].rgbtGreen = round(green / cont);
    temp_image[row_index][col_index].rgbtBlue = round(blue / cont);
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp_image[height][width];
    int start_row, end_row, start_col, end_col;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0)
            {
                start_row = 0;
                end_row = 1;
            }
            else if (i == height - 1)
            {
                start_row = -1;
                end_row = 0;
            }
            else
            {
                start_row = -1;
                end_row = 1;
            }

            if (j == 0)
            {
                start_col = 0;
                end_col = 1;
            }
            else if (j == width - 1)
            {
                start_col = -1;
                end_col = 0;
            }
            else
            {
                start_col = -1;
                end_col = 1;
            }

            blur_pixel(start_row, end_row, start_col, end_col, i, j, height, width, image, temp_image);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = temp_image[i][j];
        }
    }
    return;
}