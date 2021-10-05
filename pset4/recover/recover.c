#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cs50.h>

typedef uint8_t BYTE;

// prototypes
bool is_jpeg_signature(BYTE buffer[]);
int recover_image(BYTE buffer[], FILE *input, int img_ct);

int main(int argc, char *argv[])
{
    // verify command line argument
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    // Open file
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // intialize variables
    BYTE buffer[512];
    int img_ct = 0;

    int read_ct = fread(buffer, sizeof(BYTE), 512, input); // read at least once and initialize buffer
    // read file in 512B blocks until end of file is reached
    while (read_ct != 0)
    {
        // check if current buffer contains JPEG signature
        if (is_jpeg_signature(buffer))
        {
            recover_image(buffer, input, img_ct);
            img_ct++;
        }
        else
        {
            // read input file and store 512B into buffer
            read_ct = fread(buffer, sizeof(BYTE), 512, input);
        }
    }

    // Close files
    fclose(input);
}


// checks if given buffer contains the jpeg signature in the first 4 bytes
bool is_jpeg_signature(BYTE buffer[])
{
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff &&
       (buffer[3] == 0xe0 || buffer[3] == 0xe1 || buffer[3] == 0xe2 || buffer[3] == 0xe3 || buffer[3] == 0xe4 || buffer[3] == 0xe5 ||
        buffer[3] == 0xe6 || buffer[3] == 0xe7 || buffer[3] == 0xe8 || buffer[3] == 0xe9 || buffer[3] == 0xea || buffer[3] == 0xeb ||
        buffer[3] == 0xec || buffer[3] == 0xed || buffer[3] == 0xee || buffer[3] == 0xef))
    {
        return true;
    }
    else
    {
        return false;
    }
}

// enters when jpeg signature is found
int recover_image(BYTE buffer[], FILE *input, int img_ct)
{
    int read_ct;
    char filename[8];
    sprintf(filename, "%03i.jpg", img_ct); // format filename with 0 padding
    FILE *output = fopen(filename, "w"); // open new file
    fwrite(buffer, sizeof(BYTE), 512, output); // write buffer w/ jpeg header included into new file

    read_ct = fread(buffer, sizeof(BYTE), 512, input); // read next 512B block

    // keep reading until a block contains a jpeg signature or end of file is reached
    while (!is_jpeg_signature(buffer) && read_ct != 0)
    {
        fwrite(buffer, sizeof(BYTE), 512, output); // write buffer to new file
        read_ct = fread(buffer, sizeof(BYTE), 512, input); // read next 512B block
    }

    // jpeg signature found or end of file
    // close new file
    fclose(output);
    return read_ct;
}