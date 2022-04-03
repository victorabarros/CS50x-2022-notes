#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

int BLOCK_SIZE = 512;
int IMAGE_INDEX = -1;

char *FILE_NAME;
FILE *IMG;

bool isStartingJpeg(uint8_t buffer[]);
char *getFilename(int ii);

// $ make recover  && ./recover card.raw  && rm -f *.jpg
int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }

    // input file
    FILE *raw_file = fopen(argv[1], "r");
    uint8_t buffer[BLOCK_SIZE];

    bool jpegFounded = false;
    while (fread(buffer, 1, BLOCK_SIZE, raw_file) == BLOCK_SIZE)
    {
        if (isStartingJpeg(buffer))
        {
            jpegFounded = true;
            IMAGE_INDEX++;

            if (IMAGE_INDEX != 0)
            {
                fclose(IMG);
                free(FILE_NAME);
            }

            FILE_NAME = getFilename(IMAGE_INDEX);
            IMG = fopen(FILE_NAME, "w");

            fwrite(buffer, 1, BLOCK_SIZE, IMG);
        }
        else
        {
            if (jpegFounded)
            {
                fwrite(buffer, 1, BLOCK_SIZE, IMG);
            }
        }
    }

    fclose(raw_file);
}

// check jpg header
bool isStartingJpeg(uint8_t buffer[])
{
    if (buffer[0] == 0xff &&
        buffer[1] == 0xd8 &&
        buffer[2] == 0xff &&
        (buffer[3] & 0xf0) == 0xe0)
    {
        return true;
    }
    return false;
}

// check jpg header
char *getFilename(int ii)
{
    char *name = malloc(7);
    sprintf(name, "%03i.jpg", ii);
    return name;
}
