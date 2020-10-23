#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para usar strings
#include <stdbool.h>
#include <math.h>

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
Img aspectCorrection(Img *pic);
void writeImage(Img *pic);
void writeImageInPixels(Img *pic);
Img reduceImage(Img *pic, int percentual);

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
        int greyScale = (pic->img[i].r * 0.3) + (pic->img[i].g * 0.59) + (pic->img[i].b * 0.11);

        pic->img[i].r = greyScale;
        pic->img[i].g = greyScale;
        pic->img[i].b = greyScale;
    }
}

Img aspectCorrection(Img *pic1)
{
    //PONTEIRO RESPONSÁVEL POR INTERPRETAR O VETOR COMO MATRIZ
    RGB(*in)
    [pic1->width] = (RGB(*)[pic1->width])pic1->img;

    int totalPixelEmUmBloco = 0;
    int indiceOut = 0;

    RGB *out = malloc((pic1->width * pic1->height) * sizeof *out);
    bool naoPodeMaisPercorrerColuna = false;

    for (int linhaExterna = 0; linhaExterna <= pic1->height; linhaExterna += 5)
    {
        for (int colunaExterna = 0; colunaExterna <= pic1->width; colunaExterna += 4)
        {
            if (linhaExterna + 5 <= pic1->height && colunaExterna + 4 <= pic1->width)
            {
                for (int linhaAtual = linhaExterna; linhaAtual < linhaExterna + 5; linhaAtual++)
                {
                    for (int colunaAtual = colunaExterna; colunaAtual < colunaExterna + 4; colunaAtual++)
                    {
                        totalPixelEmUmBloco += in[linhaAtual][colunaAtual].r;
                    }
                }

                out[indiceOut].r = totalPixelEmUmBloco / 20;
                out[indiceOut].g = totalPixelEmUmBloco / 20;
                out[indiceOut].b = totalPixelEmUmBloco / 20;
                indiceOut++;
                totalPixelEmUmBloco = 0;
            }
        }
    }

    Img newPic;

    newPic.width = pic1->width / 4;
    newPic.height = pic1->height / 5;
    newPic.img = out;

    return newPic;
}

void writeImage(Img *pic)
{
    RGB(*in)
    [pic->width] = (RGB(*)[pic->width])pic->img;

    FILE *arq = fopen("saida.html", "w");

    int size = pic->width * pic->height;

    char pixelsEmASCII[size];
    int indiceOut = 0;

    for (int linha = 0; linha < pic->height; linha++)
    {
        for (int coluna = 0; coluna < pic->width; coluna++)
        {
            if (in[linha][coluna].r <= 32)
                pixelsEmASCII[indiceOut] = '.';
            else if (in[linha][coluna].r <= 64)
                pixelsEmASCII[indiceOut] = ':';
            else if (in[linha][coluna].r <= 96)
                pixelsEmASCII[indiceOut] = 'c';
            else if (in[linha][coluna].r <= 128)
                pixelsEmASCII[indiceOut] = 'o';
            else if (in[linha][coluna].r <= 160)
                pixelsEmASCII[indiceOut] = 'C';
            else if (in[linha][coluna].r <= 192)
                pixelsEmASCII[indiceOut] = '0';
            else if (in[linha][coluna].r <= 224)
                pixelsEmASCII[indiceOut] = '8';
            else
                pixelsEmASCII[indiceOut] = '@';

            indiceOut++;
        }
    }

    fprintf(arq, "<html>\n");
    fprintf(arq, "<head>\n");
    fprintf(arq, "</head>\n");
    fprintf(arq, "<body style='background: black;' leftmargin=0 topmargin=0> \n");
    fprintf(arq, "<style> \n");
    fprintf(arq, "pre  {\n");
    fprintf(arq, "color: white;\n");
    fprintf(arq, "font-family: Courier;\n");
    fprintf(arq, "  font-size: 8px;\n");
    fprintf(arq, "}\n");
    fprintf(arq, "</style>\n");
    fprintf(arq, "<pre>\n");

    int largura = 0;

    for (int i = 0; i < size; i++)
    {
        fprintf(arq, "%c", pixelsEmASCII[i]);
        largura++;

        if (largura == pic->width)
        {
            largura = 0;
            fprintf(arq, "\n");
        }
    }

    fprintf(arq, "</pre>\n");
    fprintf(arq, "</body>\n");
    fprintf(arq, "</html>\n");

    fclose(arq);
}

void writeImageInPixels(Img *pic)
{
    FILE *arq = fopen("saidaEmPixels.html", "w");

    fprintf(arq, "<html>\n");
    fprintf(arq, "<head>\n");
    fprintf(arq, "</head>\n");
    fprintf(arq, "<body style='background: black;' leftmargin=0 topmargin=0> \n");
    fprintf(arq, "<style> \n");
    fprintf(arq, "pre  {\n");
    fprintf(arq, "color: white;\n");
    fprintf(arq, "font-family: Courier;\n");
    fprintf(arq, "  font-size: 8px;\n");
    fprintf(arq, "}\n");
    fprintf(arq, "</style>\n");
    fprintf(arq, "<pre>\n");

    RGB(*in)
    [pic->width] = (RGB(*)[pic->width])pic->img;

    for (int i = 0; i < pic->height; i++)
    {
        fprintf(arq, "linha %d: ", i);

        for (int j = 0; j < pic->width; j++)
            fprintf(arq, "[%d] ", in[i][j].r);

        fprintf(arq, "\n");
    }

    fprintf(arq, "</pre>\n");
    fprintf(arq, "</body>\n");
    fprintf(arq, "</html>\n");

    fclose(arq);
}

Img reduceImage(Img *pic, int percentual)
{
    int percentualLarguraEmPixels = pic->width * percentual / 100;
    int percentualAlturaEmPixels = pic->height * percentual / 100;
    int larguraReduzida = pic->width - percentualLarguraEmPixels;
    int alturaReduzida = pic->height - percentualAlturaEmPixels;
    int intervaloHorizontal = round(pic->width / (double)percentualLarguraEmPixels);
    int intervaloVertical = round(pic->height / (double)percentualAlturaEmPixels);

    RGB(*in)
    [pic->width] = (RGB(*)[pic->width])pic->img;
    RGB *out = malloc((larguraReduzida * alturaReduzida) * sizeof *out);
    int indiceOut = 0;

    for (int linha = 0; linha < pic->height; linha++)
    {
        if ((linha + 1) % intervaloVertical == 0)
            continue;

        for (int coluna = 0; coluna < pic->width; coluna++)
        {
            if ((coluna + 1) % intervaloHorizontal == 0)
                continue;

            out[indiceOut].r = in[linha][coluna].r;
            out[indiceOut].g = in[linha][coluna].g;
            out[indiceOut].b = in[linha][coluna].b;
            indiceOut++;
        }
    }

    Img newPic;
    newPic.width = larguraReduzida;
    newPic.height = alturaReduzida;
    newPic.img = out;

    return newPic;
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

    int tamanhoConvertido = atoi(argv[2]);
    Img newPic = reduceImage(&pic, tamanhoConvertido);
    SOIL_save_image("outReduceImage.bmp", SOIL_SAVE_TYPE_BMP, newPic.width, newPic.height, 3, (const unsigned char *)newPic.img);

    convertToGreyScale(&pic);

    writeImageInPixels(&pic);

    SOIL_save_image("outGreyScale.bmp", SOIL_SAVE_TYPE_BMP, newPic.width, newPic.height, 3, (const unsigned char *)newPic.img);

    printf("Primeiros 10 pixels da imagem em escala de CINZA:\n");
    for (int i = 0; i < 10; i++)
    {
        printf("[%d %d %d] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
    }

    printf("\n");

    newPic = aspectCorrection(&newPic);

    writeImage(&newPic);

    SOIL_save_image("outAspectCorrection.bmp", SOIL_SAVE_TYPE_BMP, newPic.width, newPic.height, 3, (const unsigned char *)newPic.img);

    free(pic.img);
}