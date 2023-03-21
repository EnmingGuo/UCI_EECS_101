//
// Created by Guo Emmett on 1/12/23.
//
#include<stdio.h>
#include <stdlib.h>
#include <math.h>

#define ROWS 128
#define COLS 128
#define LOGICAL_X_MIN -4.0
#define LOGICAL_X_MAX 4.0
#define LOGICAL_Y_MIN -4.0
#define LOGICAL_Y_MAX 4.0
//function statement
void clear(unsigned char image[][COLS]);
int plot_logical_point(float x, float y, unsigned char image[][COLS]);
int plot_physical_point(int x, int y, unsigned char image[][COLS]);
int gen_plot_ortho(unsigned char image[][COLS],float x0, float a, float y0, float b, float z0, float c, float fprime);
int gen_plot_pers(unsigned char image[][COLS], float x0, float a, float y0, float b, float z0, float c, float fprime);
int in_range(int x, int y);
void header(int row, int col, unsigned char head[32]);
void write_image(unsigned char image[][COLS], char filename[]);
unsigned char images[ROWS][COLS];


int main(){

    //Ques1
    clear(images);
    gen_plot_ortho(images,0.5,0,-1,1,0,-1,1);
    write_image(images,"./images/Ques1_1.ras");
    clear(images);
    gen_plot_pers(images,0.5,0,-1,1,0,-1,1);
    write_image(images,"./images/Ques1_2.ras");

    //Ques2
    clear(images);
    gen_plot_ortho(images,0.5,1,-1,1,-1,0,1);
    gen_plot_ortho(images,-0.5,1,-1,1,-1,0,1);
    write_image(images,"./images/Ques2_1.ras");
    clear(images);
    gen_plot_pers(images,0.5,1,-1,1,-1,0,1);
    gen_plot_pers(images,-0.5,1,-1,1,-1,0,1);
    write_image(images,"./images/Ques2_2.ras");
    clear(images);
    gen_plot_ortho(images,0.5,1,-1,1,-2,0,1);
    gen_plot_ortho(images,-0.5,1,-1,1,-2,0,1);
    write_image(images,"./images/Ques2_3.ras");
    clear(images);
    gen_plot_pers(images,0.5,1,-1,1,-2,0,1);
    gen_plot_pers(images,-0.5,1,-1,1,-2,0,1);
    write_image(images,"./images/Ques2_4.ras");
    clear(images);
    gen_plot_ortho(images,0.5,1,-1,1,-3,0,1);
    gen_plot_ortho(images,-0.5,1,-1,1,-3,0,1);
    write_image(images,"./images/Ques2_5.ras");
    clear(images);
    gen_plot_pers(images,0.5,1,-1,1,-3,0,1);
    gen_plot_pers(images,-0.5,1,-1,1,-3,0,1);
    write_image(images,"./images/Ques2_6.ras");

    clear(images);
    gen_plot_ortho(images,0.5,1,-1,1,1,0,1);
    gen_plot_ortho(images,-0.5,1,-1,1,1,0,1);
    write_image(images,"./images/Ques2_7.ras");
    clear(images);
    gen_plot_pers(images,0.5,1,-1,1,1,0,1);
    gen_plot_pers(images,-0.5,1,-1,1,1,0,1);
    write_image(images,"./images/Ques2_8.ras");


    //Ques3
    clear(images);
    gen_plot_ortho(images,-1,0,-1,0,0,1,1);
    gen_plot_ortho(images,1,0,-1,0,0,1,1);
    write_image(images,"./images/Ques3_o_0_1.ras");
    clear(images);
    gen_plot_pers(images,-1,0,-1,0,0,1,1);
    gen_plot_pers(images,1,0,-1,0,0,1,1);
    write_image(images,"./images/Ques_3_p_0_1.ras");

    clear(images);
    gen_plot_ortho(images,-1,0,-1,0,0,-1,1);
    gen_plot_ortho(images,1,0,-1,0,0,-1,1);
    write_image(images,"./images/Ques3_o_0_-1.ras");
    clear(images);
    gen_plot_pers(images,-1,0,-1,0,0,-1,1);
    gen_plot_pers(images,1,0,-1,0,0,-1,1);
    write_image(images,"./images/Ques_3_p_0_-1.ras");

    clear(images);
    gen_plot_ortho(images,-1,0,-1,1,0,1,1);
    gen_plot_ortho(images,1,0,-1,1,0,1,1);
    write_image(images,"./images/Ques3_o_1_1.ras");
    clear(images);
    gen_plot_pers(images,-1,0,-1,1,0,1,1);
    gen_plot_pers(images,1,0,-1,1,0,1,1);
    write_image(images,"./images/Ques_3_p_1_1.ras");

    clear(images);
    gen_plot_ortho(images,-1,0,-1,1,0,-1,1);
    gen_plot_ortho(images,1,0,-1,1,0,-1,1);
    write_image(images,"./images/Ques3_o_1_-1.ras");
    clear(images);
    gen_plot_pers(images,-1,0,-1,1,0,-1,1);
    gen_plot_pers(images,1,0,-1,1,0,-1,1);
    write_image(images,"./images/Ques_3_p_1_-1.ras");

    clear(images);
    gen_plot_ortho(images,-1,0,-1,-1,0,1,1);
    gen_plot_ortho(images,1,0,-1,-1,0,1,1);
    write_image(images,"./images/Ques3_o_-1_1.ras");
    clear(images);
    gen_plot_pers(images,-1,0,-1,-1,0,1,1);
    gen_plot_pers(images,1,0,-1,-1,0,1,1);
    write_image(images,"./images/Ques_3_p_-1_1.ras");

    clear(images);
    gen_plot_ortho(images,-1,0,-1,-1,0,-1,1);
    gen_plot_ortho(images,1,0,-1,-1,0,-1,1);
    write_image(images,"./images/Ques3_o_-1_-1.ras");
    clear(images);
    gen_plot_pers(images,-1,0,-1,-1,0,-1,1);
    gen_plot_pers(images,1,0,-1,-1,0,-1,1);
    write_image(images,"./images/Ques_3_p_-1_-1.ras");
    return 0;
}
void header(int row, int col, unsigned char head[32])
{
    int* p = (int*)head;
    char* ch;
    int num = row * col;

    /* Choose little-endian or big-endian header depending on the machine. Don't modify the contents */

    /* Little-endian for PC */
    *p = 0x956aa659;
    *(p + 3) = 0x08000000;
    *(p + 5) = 0x01000000;
    *(p + 6) = 0x0;
    *(p + 7) = 0xf8000000;

    ch = (char*)&col;
    head[7] = *ch;
    ch++;
    head[6] = *ch;
    ch++;
    head[5] = *ch;
    ch++;
    head[4] = *ch;

    ch = (char*)&row;
    head[11] = *ch;
    ch++;
    head[10] = *ch;
    ch++;
    head[9] = *ch;
    ch++;
    head[8] = *ch;

    ch = (char*)&num;
    head[19] = *ch;
    ch++;
    head[18] = *ch;
    ch++;
    head[17] = *ch;
    ch++;
    head[16] = *ch;
    /*
    // Big-endian for unix
    *p = 0x59a66a95;
    *(p + 1) = col;
    *(p + 2) = row;
    *(p + 3) = 0x8;
    *(p + 4) = num;
    *(p + 5) = 0x1;
    *(p + 6) = 0x0;
    *(p + 7) = 0xf8;
    */
}
int in_range(int x, int y)
{
    return x >= 0 && x < COLS&& y >= 0 && y < ROWS;
}
void clear(unsigned char image[][COLS])
{
    int	i, j;
    for (i = 0; i < ROWS; i++)
        for (j = 0; j < COLS; j++) image[i][j] = 0;
}
int plot_logical_point(float x, float y, unsigned char image[][COLS])
{
    int	nx, ny;
    float	xc, yc;
    xc = COLS / ((float)LOGICAL_X_MAX - LOGICAL_X_MIN);
    yc = ROWS / ((float)LOGICAL_Y_MAX - LOGICAL_Y_MIN);
    nx = (x - LOGICAL_X_MIN) * xc;
    ny = (y - LOGICAL_Y_MIN) * yc;
    return plot_physical_point(nx, ny, image);
}
int plot_physical_point(int x, int y, unsigned char image[][COLS])
{
    if (in_range(x, y)) //return 0;
        return image[y][x] = 255;

}
int gen_plot_ortho(unsigned char image[][COLS],float x0, float a, float y0, float b, float z0, float c, float fprime)
{
    int				i;
    float			t, x, y, z, xprime, yprime;
    for (t = 0.01; t <= 10000; t += .01)  //change the t range
    {
        /*Modify the coefficiences according to questions*/
        x = x0 + t * a;
        y = y0 + t * b;
        z = z0 + t * c;
        /* Modify these according to the projection */
        xprime = x;			/* this is an example of orth */
        yprime = y;
        if ((xprime<LOGICAL_X_MAX && xprime>LOGICAL_X_MIN) && (yprime > LOGICAL_Y_MIN && yprime < LOGICAL_Y_MAX))
            plot_logical_point(xprime, yprime, image);
    }
}
void write_image(unsigned char image[][COLS], char filename[])
{
    int			i;
    FILE			*fp;
    char			 *ofile;
    unsigned char head[32];
    header(ROWS, COLS, head);
    ofile = filename; // this is the name of your output file, modify the name according to the requirements

    if (!(fp = fopen(ofile, "wb")))
        fprintf(stderr, "error: could not open %s\n", ofile), exit(1);

    /* Write the header */
    fwrite(head, 4, 8, fp);
    for (i = 0; i < ROWS; i++) fwrite(image[i], 1, COLS, fp);
    fclose(fp);
}
int gen_plot_pers(unsigned char image[][COLS],float x0, float a, float y0, float b, float z0, float c, float fprime)
{
    int				i;
    float			t, x, y, z, xprime, yprime;
    FILE* fp;
    for (t = 0.01; t <= 10000; t += .01)  //change the t range
    {
        /*Modify the coefficiences according to questions*/
        x = x0 + t * a;
        y = y0 + t * b;
        z = z0 + t * c;
        fprime = 1;
        /* Modify these according to the projection */
        xprime = fprime * (x / z);			/* this is an example of perspective */
        yprime = fprime * (y / z);
        if ((xprime<LOGICAL_X_MAX && xprime>LOGICAL_X_MIN) && (yprime > LOGICAL_Y_MIN && yprime < LOGICAL_Y_MAX))
            plot_logical_point(xprime, yprime, image);
    }
    return 0;
}
