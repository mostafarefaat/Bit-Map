#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma pack(1)
                                     /*-------------Important Formulas-------------*/
                                /* imag size = width * height * number of bytes of each pixel*/
                                        /*----In Case of Rectangle----I*/
                                /* index of point(x,y) is (y * width + x)  */
                                /* Position of an index is ( x = index % width , y = index/ width)*/


#define Max_Width 1920
#define Max_Height 1276

struct  Header
{
    char signature[2];
    int file_size;
    int reserved;
    int offset;
};

struct  DipHeader
{
    int dipheadersize;
    int width;
    int heigth;
    short int nr_planes;
    short int bitcount;
    int compression;
    int imgsize;
    int x_pm_m;
    int y_pm_m;
    int use_colors;
    int important_colors;
};

struct Pixel
{
    char b;
    char g;
    char r;
};

struct BMP_File
{
    struct  Header header;
    struct  DipHeader dipheader;
    struct  Pixel pixels[Max_Width * Max_Height];
};

enum Color{
    WHITE,
    BLACK,
    RED,
    GREEN,
    BLUE,
};


struct BMP_File bmp;

void draw_point(int x , int y, enum Color color){

    int index = (y * bmp.dipheader.width + x);
    switch (color)
    {
    case WHITE: bmp.pixels[index].r = bmp.pixels[index].b = bmp.pixels[index].g = 255;
        break;
    case BLACK: bmp.pixels[index].r = bmp.pixels[index].b = bmp.pixels[index].g = 0;
        break;
    case RED:   bmp.pixels[index].r = 255;
                bmp.pixels[index].b = bmp.pixels[index].g = 0;
        break;
    case GREEN: bmp.pixels[index].g = 255;
                bmp.pixels[index].b = bmp.pixels[index].r = 0;
        break;
    case BLUE:  bmp.pixels[index].b = 255;
                bmp.pixels[index].r = bmp.pixels[index].g = 0;
        break;
    
    default:
        break;
    }
}

void set_contrast(float percntage){
    for(int i = 0; i < (bmp.dipheader.width * bmp.dipheader.heigth); i++)
    {  
        bmp.pixels[i].b = (unsigned char)bmp.pixels[i].b * percntage;
        bmp.pixels[i].g = (unsigned char)bmp.pixels[i].g * percntage;
        bmp.pixels[i].r = (unsigned char)bmp.pixels[i].r * percntage;
    }
}

int main(){

    int width = 0;
    int height = 0;
    
    struct Header * header_ptr = &bmp.header;
    struct DipHeader * dipheader_ptr = &bmp.dipheader;
    struct Pixel * pixel_ptr = bmp.pixels;


    strcpy(header_ptr->signature,"BM");
    header_ptr->offset = 54;

    dipheader_ptr->width = 20;
    dipheader_ptr->heigth = 20;
    width = dipheader_ptr->width ;
    height = dipheader_ptr->heigth ;
    dipheader_ptr->bitcount = 24;
    dipheader_ptr->imgsize = (dipheader_ptr->width ) * (dipheader_ptr->heigth) * 3;
    dipheader_ptr->dipheadersize = 40;

    header_ptr->file_size = (sizeof(struct Header)) + (sizeof(struct DipHeader)) + (dipheader_ptr->imgsize);

    for(int i = 0; i<(width * (height/2));i++){
        pixel_ptr[i].r = pixel_ptr[i].g = pixel_ptr[i].b = 255;
    }

    for(int i = (width * (height/2)); i<(width * height);i++){
        pixel_ptr[i].r = pixel_ptr[i].g = pixel_ptr[i].b = 0;
    }

    draw_point(10,0,RED);
    set_contrast(0.5);
    FILE * fileptr = fopen("test.bmp","w");
    fwrite(&bmp, header_ptr->file_size , 1, fileptr);
    fclose(fileptr);
    return 0;
}