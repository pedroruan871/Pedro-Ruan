#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");

    if(f == NULL)
    {
        printf("Invalid image\n");
        return 1;
    }

    BYTE buffer[512];
    //the jpeg count will help me on opening new files
    int JPEG_COUNT = 0;

    FILE *filename = NULL;// output file

    char *str = malloc(8 * sizeof(char));// filename

    while(fread(&buffer, sizeof(char), 512, f))
    {

        //if it is the beginning of a new jpeg
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //if it is the first jpeg i've found
            sprintf(str, "%03i.jpg", JPEG_COUNT);

            //Open the file in write mode
            filename = fopen(str, "w");

            JPEG_COUNT++;
        }

        //if it isn't the beginning of a new jpeg and i am already writing one
        if(filename != NULL)
        {
            //keep writing this block inside of the jpeg
            fwrite(&buffer, sizeof(char), 512, filename);
        }

    }
    fclose(filename);
    free(str);
    fclose(f);
    return 0;
}