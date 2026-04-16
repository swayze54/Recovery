#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

//Memory card is read using VAT protocol
#define BLOCK = 512

int main(int argc, char *argv[])
{
//Opens input and output files and checks if valid.
    if(argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "r");
    if(input==NULL)
    {
        printf("Error 2: Could not open file\n");
        return 2;
    }

    int j = 0;
    char picture[13];
    uint8_t buffer[BLOCK];
    int first = 0;
    FILE *output = NULL;

    while(fread(&buffer, 1, BLOCK, input) == BLOCK)
    {
        if((buffer[0]==0xff)&&(buffer[1]==0xd8)&&(buffer[2]==0xff)&&((buffer[3]&0xe0)==0xe0))
        {
            if(first==0)
            {
                sprintf(picture, "%i.jpeg", j);
                output = fopen(picture, "w");
                fwrite(&buffer, 1, BLOCK, output);
                first = 1;
            }
            else
            {
                fclose(output);
                j++;
                sprintf(picture, "%i.jpeg", j);
                output = fopen(picture, "w");
                fwrite(&buffer, 1, BLOCK, output);
            }
        }
        else if(first == 1)
            fwrite(&buffer, 1, BLOCK, output);
        }
// close files
    fclose(input);
    fclose(output);
}
