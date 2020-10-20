#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Para usar strings
#include <stdbool.h>

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

    //ARREDONDANDO MEDIDAS DA IMAGEM PARA REALIZAR A CORREÇÃO DE ASPECTO
    int larguraArredondada = pic1->width - (pic1->width % 4);
    int alturaArredondada = pic1->height - (pic1->height % 5);

    //PONTEIRO RESPONSÁVEL POR INTERPRETAR O VETOR COMO MATRIZ
    RGB(*in)
    [pic1->width] = (RGB(*)[pic1->width])pic1->img;

    //NOVAS MEDIDAS DA IMAGEM DE SAÍDA
    int novaLargura = larguraArredondada / 4;
    int novaAltura = alturaArredondada / 5;

    RGB out[novaLargura * novaAltura];
    int indiceOut = 0;

    int totalPixelEmUmBloco = 0;

    int indiceColunaInicial = 0;
    int indiceColunaMaxima = 4;
    bool chegouFinalVetor = false;

    int contador = 1;

    for (int linhas = 0; linhas < alturaArredondada; linhas++)
    {
        for (int colunas = indiceColunaInicial; colunas < indiceColunaMaxima; colunas++)
        {
            int pixelAtual = in[linhas][colunas].r;
            totalPixelEmUmBloco += pixelAtual;

            if ((linhas + 1) == alturaArredondada && (colunas + 1) == larguraArredondada)
                chegouFinalVetor = true;
        }

        if (contador == 5)
        {
            int mediaPonderada = totalPixelEmUmBloco / 20;

            out[indiceOut].r = mediaPonderada;
            out[indiceOut].g = mediaPonderada;
            out[indiceOut].b = mediaPonderada;
            indiceOut++;
            totalPixelEmUmBloco = 0;
            contador = 0;
        }

        if ((linhas + 1) == alturaArredondada)
        {
            indiceColunaInicial = indiceColunaMaxima;
            indiceColunaMaxima += 4;
            linhas = -1;
        }

        if (chegouFinalVetor)
            break;
        else
            contador++;
    }

    Img newPic;
    newPic.height = novaAltura;
    newPic.width = novaLargura;
    newPic.img = out;

    return newPic;
}

void writeImage(Img *pic)
{
    int size = pic->width * pic->height;

    RGB(*in)
    [pic->width] = (RGB(*)[pic->width])pic->img;

    FILE *arq = fopen("saida.html", "w");

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

    for (int i = 0; i < pic->height; i++)
    {
        for (int j = 0; j < pic->width; j++)
        {
            if (in[i][j].r <= 32)
                fprintf(arq, "%c", '.');
            else if (in[i][j].r <= 64)
                fprintf(arq, "%c", ':');
            else if (in[i][j].r <= 96)
                fprintf(arq, "%c", 'c');
            else if (in[i][j].r <= 128)
                fprintf(arq, "%c", 'o');
            else if (in[i][j].r <= 160)
                fprintf(arq, "%c", 'C');
            else if (in[i][j].r <= 192)
                fprintf(arq, "%c", '0');
            else if (in[i][j].r <= 224)
                fprintf(arq, "%c", '8');
            else
                fprintf(arq, "%c", '@');
        }
        fprintf(arq, "\n");
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

    writeImageInPixels(&pic);

    SOIL_save_image("outGreyScale.bmp", SOIL_SAVE_TYPE_BMP, pic.width, pic.height, 3, (const unsigned char *)pic.img);

    printf("Primeiros 10 pixels da imagem em escala de CINZA:\n");
    for (int i = 0; i < 10; i++)
    {
        printf("[%d %d %d] ", pic.img[i].r, pic.img[i].g, pic.img[i].b);
    }

    printf("\n");

    Img newPic = aspectCorrection(&pic);

    writeImage(&newPic);

    SOIL_save_image("outAspectCorrection.bmp", SOIL_SAVE_TYPE_BMP, newPic.width, newPic.height, 3, (const unsigned char *)newPic.img);

    free(pic.img);
}