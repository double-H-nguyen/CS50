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

// Reflect image horizontally
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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Define Gx and Gy kernels
    const int UL_GX = -1;
    const int UM_GX = 0;
    const int UR_GX = 1;
    const int ML_GX = -2;
    const int MM_GX = 0;
    const int MR_GX = 2;
    const int BL_GX = -1;
    const int BM_GX = 0;
    const int BR_GX = 1;
    const int UL_GY = -1;
    const int UM_GY = -2;
    const int UR_GY = -1;
    const int ML_GY = 0;
    const int MM_GY = 0;
    const int MR_GY = 0;
    const int BL_GY = 1;
    const int BM_GY = 2;
    const int BR_GY = 1;

    RGBTRIPLE image_buffer[height][width];
    int upper_h, lower_h, left_w, right_w;
    int gx_blue_sum, gx_green_sum, gx_red_sum;
    int gy_blue_sum, gy_green_sum, gy_red_sum;
    int combined_blue, combined_green, combined_red;

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // reset values
            gx_blue_sum = gx_green_sum = gx_red_sum = 0;
            gy_blue_sum = gy_green_sum = gy_red_sum = 0;
            combined_blue = combined_green = combined_red = 0;

            // neightboring pixels' indices
            upper_h = h - 1;
            lower_h = h + 1;
            left_w = w - 1;
            right_w = w + 1;

            // for each target and its neighboring pixels, get product of original channel value and kernal

            // upper left pixel
            if (upper_h >= 0 && left_w >= 0)
            {
                // calculate Gx per channel
                gx_blue_sum += image[upper_h][left_w].rgbtBlue * UL_GX;
                gx_green_sum += image[upper_h][left_w].rgbtGreen * UL_GX;
                gx_red_sum += image[upper_h][left_w].rgbtRed * UL_GX;

                // calculate Gy per channel
                gy_blue_sum += image[upper_h][left_w].rgbtBlue * UL_GY;
                gy_green_sum += image[upper_h][left_w].rgbtGreen * UL_GY;
                gy_red_sum += image[upper_h][left_w].rgbtRed * UL_GY;
            }

            // upper mid pixel
            if (upper_h >= 0)
            {
                gx_blue_sum += image[upper_h][w].rgbtBlue * UM_GX;
                gx_green_sum += image[upper_h][w].rgbtGreen * UM_GX;
                gx_red_sum += image[upper_h][w].rgbtRed * UM_GX;

                gy_blue_sum += image[upper_h][w].rgbtBlue * UM_GY;
                gy_green_sum += image[upper_h][w].rgbtGreen * UM_GY;
                gy_red_sum += image[upper_h][w].rgbtRed * UM_GY;
            }

            // upper right pixel
            if (upper_h >= 0 && right_w < width)
            {
                gx_blue_sum += image[upper_h][right_w].rgbtBlue * UR_GX;
                gx_green_sum += image[upper_h][right_w].rgbtGreen * UR_GX;
                gx_red_sum += image[upper_h][right_w].rgbtRed * UR_GX;

                gy_blue_sum += image[upper_h][right_w].rgbtBlue * UR_GY;
                gy_green_sum += image[upper_h][right_w].rgbtGreen * UR_GY;
                gy_red_sum += image[upper_h][right_w].rgbtRed * UR_GY;
            }

            // mid left pixel
            if (left_w >= 0)
            {
                gx_blue_sum += image[h][left_w].rgbtBlue * ML_GX;
                gx_green_sum += image[h][left_w].rgbtGreen * ML_GX;
                gx_red_sum += image[h][left_w].rgbtRed * ML_GX;

                gy_blue_sum += image[h][left_w].rgbtBlue * ML_GY;
                gy_green_sum += image[h][left_w].rgbtGreen * ML_GY;
                gy_red_sum += image[h][left_w].rgbtRed * ML_GY;
            }

            // target pixel
            gx_blue_sum += image[h][w].rgbtBlue * MM_GX;
            gx_green_sum += image[h][w].rgbtGreen * MM_GX;
            gx_red_sum += image[h][w].rgbtRed * MM_GX;

            gy_blue_sum += image[h][w].rgbtBlue * MM_GY;
            gy_green_sum += image[h][w].rgbtGreen * MM_GY;
            gy_red_sum += image[h][w].rgbtRed * MM_GY;

            // mid right pixel
            if (right_w < width)
            {
                gx_blue_sum += image[h][right_w].rgbtBlue * MR_GX;
                gx_green_sum += image[h][right_w].rgbtGreen * MR_GX;
                gx_red_sum += image[h][right_w].rgbtRed * MR_GX;

                gy_blue_sum += image[h][right_w].rgbtBlue * MR_GY;
                gy_green_sum += image[h][right_w].rgbtGreen * MR_GY;
                gy_red_sum += image[h][right_w].rgbtRed * MR_GY;
            }

            // bottom left pixel
            if (lower_h < height && left_w >= 0)
            {
                gx_blue_sum += image[lower_h][left_w].rgbtBlue * BL_GX;
                gx_green_sum += image[lower_h][left_w].rgbtGreen * BL_GX;
                gx_red_sum += image[lower_h][left_w].rgbtRed * BL_GX;

                gy_blue_sum += image[lower_h][left_w].rgbtBlue * BL_GY;
                gy_green_sum += image[lower_h][left_w].rgbtGreen * BL_GY;
                gy_red_sum += image[lower_h][left_w].rgbtRed * BL_GY;
            }

            // bottom mid pixel
            if (lower_h < height)
            {
                gx_blue_sum += image[lower_h][w].rgbtBlue * BM_GX;
                gx_green_sum += image[lower_h][w].rgbtGreen * BM_GX;
                gx_red_sum += image[lower_h][w].rgbtRed * BM_GX;

                gy_blue_sum += image[lower_h][w].rgbtBlue * BM_GY;
                gy_green_sum += image[lower_h][w].rgbtGreen * BM_GY;
                gy_red_sum += image[lower_h][w].rgbtRed * BM_GY;
            }

            // bottom right pixel
            if (lower_h < height && right_w < width)
            {
                gx_blue_sum += image[lower_h][right_w].rgbtBlue * BR_GX;
                gx_green_sum += image[lower_h][right_w].rgbtGreen * BR_GX;
                gx_red_sum += image[lower_h][right_w].rgbtRed * BR_GX;

                gy_blue_sum += image[lower_h][right_w].rgbtBlue * BR_GY;
                gy_green_sum += image[lower_h][right_w].rgbtGreen * BR_GY;
                gy_red_sum += image[lower_h][right_w].rgbtRed * BR_GY;
            }

            // combine Gx and Gy into one value
            combined_blue = round(sqrt(pow(gx_blue_sum, 2) + pow(gy_blue_sum, 2)));
            combined_green = round(sqrt(pow(gx_green_sum, 2) + pow(gy_green_sum, 2)));
            combined_red = round(sqrt(pow(gx_red_sum, 2) + pow(gy_red_sum, 2)));

            // cap value at 255
            if (combined_blue > 255)
            {
                combined_blue = 255;
            }
            if (combined_green > 255)
            {
                combined_green = 255;
            }
            if (combined_red > 255)
            {
                combined_red = 255;
            }

            // update image buffer
            image_buffer[h][w].rgbtBlue = combined_blue;
            image_buffer[h][w].rgbtGreen = combined_green;
            image_buffer[h][w].rgbtRed = combined_red;
        }
    }

    // Once edge image has been made from the original colors,
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
