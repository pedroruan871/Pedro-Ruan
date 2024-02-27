// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t BYTE;
typedef int16_t SAMPLE;

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

    uint8_t header[HEADER_SIZE];
    // TODO: Copy header from input file to output file
    fread(header, sizeof(BYTE), HEADER_SIZE, input);
    fwrite(header, sizeof(BYTE), HEADER_SIZE, output);

    //It will no longer rewrite the output file, but will append data on it

    // TODO: Read samples from input file and write updated data to output file
    SAMPLE sample;
    int sample_couple = 0;
    while(fread(&sample, sizeof(SAMPLE), 1, input))
    {
        sample_couple++;
        sample = sample * factor;
        fwrite(&sample, sizeof(BYTE), 2, output);

    }
    // Close files
    fclose(input);
    fclose(output);
    return 0;
}
