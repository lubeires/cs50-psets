#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;
typedef int bool;
const int BLOCK_SIZE = 512;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    FILE *raw_file = fopen(argv[1], "r");
    if (!raw_file)
    {
        printf("Could not open file.\n");
        return 1;
    }

    BYTE buffer[BLOCK_SIZE];
    char file_name[8];
    int cont = 0;
    FILE *image;
    while (fread(buffer, BLOCK_SIZE, 1, raw_file))
    {
        bool is_new_jpeg = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;
        if (is_new_jpeg)
        {
            if (cont)
            {
                fclose(image);
            }
            sprintf(file_name, "%03i.jpg", cont++);
            image = fopen(file_name, "w");
            fwrite(buffer, BLOCK_SIZE, 1, image);
        }
        else
        {
            if (cont)
            {
                fwrite(buffer, BLOCK_SIZE, 1, image);
            }
        }
    }
    fclose(raw_file);
    fclose(image);
}