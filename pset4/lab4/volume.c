// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;
typedef int16_t AUDIO_SAMPLE;

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    BYTE buffer; // temporary byte to hold the data that's being copied
    for (int i = 0; i < HEADER_SIZE; i++) // iterate through input file's header
    {
        // write each byte of input header into output file
        fread(&buffer, sizeof(BYTE), 1, input); // read input file and store content into buffer
        fwrite(&buffer, sizeof(BYTE), 1, output); // write contents inside buffer to output file
    }

    // TODO: Read samples from input file and write updated data to output file
    AUDIO_SAMPLE audio_buffer; // temporary 16 bit space to hold the data that's being copied
    while (fread(&audio_buffer, sizeof(AUDIO_SAMPLE), 1, input)) // copy audio sample until end of file
    {
        audio_buffer *= factor;
        fwrite(&audio_buffer, sizeof(AUDIO_SAMPLE), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
