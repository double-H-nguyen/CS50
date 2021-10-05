#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate through each row
    for (int h = 0; h < height; h++)
    {
        // for each row, iterate through each pixel
        for (int w = 0; w < width; w++)
        {
            // store copy of pixel
            RGBTRIPLE pixel = image[h][w];

            // take an average of BGR values of pixel
            float average = (float)(pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed) / 3.0;

            // round average to nearest integer
            int new_value = round(average);

            // set BGR to the same value
            image[h][w].rgbtBlue = new_value;
            image[h][w].rgbtGreen = new_value;
            image[h][w].rgbtRed = new_value;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate through each row
    for (int h = 0; h < height; h++)
    {
        // for each row, iterate through each pixel
        for (int w = 0; w < width; w++)
        {
            // store copy of pixel
            int originalRed = image[h][w].rgbtRed;
            int originalGreen = image[h][w].rgbtGreen;
            int originalBlue = image[h][w].rgbtBlue;

            // create sepiaRed
            int sepiaRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            // if value is greater than 255, make it 255
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }

            // create sepiaGreen
            int sepiaGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }

            // create sepiaBlue
            int sepiaBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            // update pixel
            image[h][w].rgbtRed = sepiaRed;
            image[h][w].rgbtGreen = sepiaGreen;
            image[h][w].rgbtBlue = sepiaBlue;
        }
    }

    return;
}

// Reflect image horizontally--
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // iterate through each row
    for (int h = 0; h < height; h++)
    {
        // for each row, inverse the pixel
        RGBTRIPLE pixel_buffer;
        RGBTRIPLE *left_pixel, *right_pixel;
        int left_pixel_index = 0;
        int right_pixel_index = width - 1;
        double midpoint = floor((float) width / 2.0);
        for (int i = 0; i < (int) midpoint; i++) // when midpoint is reached, inverse is completed
        {
            // pass pixels' addresses to temporary pointer variables
            left_pixel = &image[h][left_pixel_index];
            right_pixel = &image[h][right_pixel_index];

            // swap
            pixel_buffer = *left_pixel; // saving contents of left_pixel into buffer
            *left_pixel = *right_pixel; // saving contents of right_pixel into the address that left_pixel is pointing to
            *right_pixel = pixel_buffer; // saving contents of buffer into the address that right_pixel is pointing to

            // update indexes
            left_pixel_index++;
            right_pixel_index--;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_buffer[height][width];
    int upper_h, lower_h, left_w, right_w;
    int blue_avg, green_avg, red_avg;
    float blue_sum, green_sum, red_sum;
    float ct;

    // create blur image and save to image_buffer
    // top-left pixel is 1, 1
    // bottom-right pixel is height, width
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // reset values
            blue_avg = green_avg = red_avg = 0;
            blue_sum = green_sum = red_sum = 0.0;
            ct = 0.0;

            // add target to sum, and increment ct
            blue_sum += image[h][w].rgbtBlue;
            green_sum += image[h][w].rgbtGreen;
            red_sum += image[h][w].rgbtRed;
            ct++;

            // neightboring pixels' indices
            upper_h = h - 1;
            lower_h = h + 1;
            left_w = w - 1;
            right_w = w + 1;

            // upper left pixel
            if (upper_h >= 0 && left_w >= 0)
            {
                blue_sum += image[upper_h][left_w].rgbtBlue;
                green_sum += image[upper_h][left_w].rgbtGreen;
                red_sum += image[upper_h][left_w].rgbtRed;
                ct++;
            }

            // upper mid pixel
            if (upper_h >= 0)
            {
                blue_sum += image[upper_h][w].rgbtBlue;
                green_sum += image[upper_h][w].rgbtGreen;
                red_sum += image[upper_h][w].rgbtRed;
                ct++;
            }

            // upper right pixel
            if (upper_h >= 0 && right_w < width)
            {
                blue_sum += image[upper_h][right_w].rgbtBlue;
                green_sum += image[upper_h][right_w].rgbtGreen;
                red_sum += image[upper_h][right_w].rgbtRed;
                ct++;
            }

            // mid left pixel
            if (left_w >= 0)
            {
                blue_sum += image[h][left_w].rgbtBlue;
                green_sum += image[h][left_w].rgbtGreen;
                red_sum += image[h][left_w].rgbtRed;
                ct++;
            }

            // mid right pixel
            if (right_w < width)
            {
                blue_sum += image[h][right_w].rgbtBlue;
                green_sum += image[h][right_w].rgbtGreen;
                red_sum += image[h][right_w].rgbtRed;
                ct++;
            }

            // bottom left pixel
            if (lower_h < height && left_w >= 0)
            {
                blue_sum += image[lower_h][left_w].rgbtBlue;
                green_sum += image[lower_h][left_w].rgbtGreen;
                red_sum += image[lower_h][left_w].rgbtRed;
                ct++;
            }

            // bottom mid pixel
            if (lower_h < height)
            {
                blue_sum += image[lower_h][w].rgbtBlue;
                green_sum += image[lower_h][w].rgbtGreen;
                red_sum += image[lower_h][w].rgbtRed;
                ct++;
            }

            // bottom right pixel
            if (lower_h < height && right_w < width)
            {
                blue_sum += image[lower_h][right_w].rgbtBlue;
                green_sum += image[lower_h][right_w].rgbtGreen;
                red_sum += image[lower_h][right_w].rgbtRed;
                ct++;
            }

            // average and round
            blue_avg = round(blue_sum / ct);
            green_avg = round(green_sum / ct);
            red_avg = round(red_sum / ct);

            // add new blurred pixel to image_buffer
            image_buffer[h][w].rgbtBlue = blue_avg;
            image_buffer[h][w].rgbtGreen = green_avg;
            image_buffer[h][w].rgbtRed = red_avg;
        }
    }

    // Once blur image has been made from the original colors,
    // save contents of image buffer into the address that original image is pointing to
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // save image_buffer's pixel to original image location
            image[h][w] = image_buffer[h][w];
        }
    }

    return;
}
