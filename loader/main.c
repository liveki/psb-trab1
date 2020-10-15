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
    //NOVAS MEDIDAS DA IMAGEM DE SAÍDA
    int novaLargura = pic1->width / 4;
    int novaAltura = pic1->height / 5;

    //PONTEIRO RESPONSÁVEL POR INTERPRETAR O VETOR COMO MATRIZ
    RGB(*in)
    [pic1->width] = (RGB(*)[pic1->width])pic1->img;

    //ARREDONDANDO MEDIDAS DA IMAGEM PARA REALIZAR A CORREÇÃO DE ASPECTO
    int larguraArredondada = pic1->width - (pic1->width % 4);
    int alturaArredondada = pic1->height - (pic1->height % 5);

    RGB out[novaLargura * novaAltura];
    int indiceOut = 0;

    int totalPixelEmUmBloco = 0;

    int colunaMaxima = 4;
    int colunaInicial = 0;
    bool chegouFinalVetor = false;

    for (int linhas = 0; linhas < alturaArredondada; linhas++)
        {
            
        for(int colunas = colunaInicial; colunas < colunaMaxima; colunas++){
            totalPixelEmUmBloco += in[linhas][colunas].r;

            if((linhas +1) == alturaArredondada && (colunas +1) == larguraArredondada){
                chegouFinalVetor= true;
            }
          
        }

        if((linhas +1) %5 == 0 ){
            out[indiceOut].r = totalPixelEmUmBloco / 20;
            indiceOut++;
            totalPixelEmUmBloco = 0;

            if((linhas +1) == alturaArredondada){
                linhas = 0;
                colunaInicial += 4;
                colunaMaxima += 4;

            }                        
        }
        if(chegouFinalVetor){
            break;
        }
    }

    Img newPic;
    newPic.height = novaAltura;
    newPic.width = novaLargura;
    newPic.img = out;

    printf("%s", "estou saindo da funcao");

    return newPic;
}

void writeImage(Img *pic)
{
    int size = pic->width * pic->height;

    char pixelsinASCII[size];

    for (int i = 0; i < size; i++)
    {
        int greyScale = pic->img[i].r;

        if (greyScale <= 32)
            pixelsinASCII[i] = '.';
        else if (greyScale <= 64)
            pixelsinASCII[i] = ':';
        else if (greyScale <= 96)
            pixelsinASCII[i] = 'c';
        else if (greyScale <= 128)
            pixelsinASCII[i] = 'o';
        else if (greyScale <= 160)
            pixelsinASCII[i] = 'C';
        else if (greyScale <= 192)
            pixelsinASCII[i] = '0';
        else if (greyScale <= 224)
            pixelsinASCII[i] = '8';
        else
            pixelsinASCII[i] = '@';
    }

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

    int largura = 0;

    for (int i = 0; i < pic->width * pic->height; i++)
    {
        fprintf(arq, "%c", pixelsinASCII[i]);

        if (largura == 300)
        {
            fprintf(arq, "\n");
            largura = 0;
        }
        else
            largura++;
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