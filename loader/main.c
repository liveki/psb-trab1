#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para usar strings

// SOIL é a biblioteca para leitura das imagens
#include "SOIL.h"

// Um pixel RGB
typedef struct
{
    unsigned char r, g, b;
} RGB;

// Uma imagem em RGB
typedef struct
{
    int width, height;
    RGB *img;
} Img;

// Protótipos
void load(char *name, Img *pic);
void convertToGreyScale(Img *pic);

// Carrega uma imagem para a struct Img
void load(char *name, Img *pic)
{
    int chan;
    pic->img = (unsigned char *)SOIL_load_image(name, &pic->width, &pic->height, &chan, SOIL_LOAD_RGB);
    if (!pic->img)
    {
        printf("SOIL loading error: '%s'\n", SOIL_last_result());
        exit(1);
    }
    printf("Load: %d x %d x %d\n", pic->width, pic->height, chan);
}

void convertToGreyScale(Img *pic)
{

    for (int i = 0; i < pic->width * pic->height; i++)
    {
        int r = pic->img[i].r * 0.3;
        int g = pic->img[i].g * 0.59;
        int b = pic->img[i].b * 0.11;

        int greyScale = (r + g + b);

        pic->img[i].r = pic->img[i].g = pic->img->b = greyScale;
    }
}

int main(int argc, char **argv)
{
    Img pic;
    if (argc < 1)
    {
        printf("loader [img]\n");
        exit(1);
    }
    load(argv[1], &pic);

    printf("Primeiros 10 pixels da imagem:\n");
    for (int i = 0; i < 10; i++)
    {
        printf("[%d %d %d] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
    }

    printf("\n");
    convertToGreyScale(&pic);

    SOIL_save_image("out.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, (const unsigned char *)pic.img);

    free(pic.img);
}
