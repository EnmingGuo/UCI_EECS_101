#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include<iostream>

#define ROWS		100
#define COLUMNS		100

int main( int argc, char **argv )
{

    int			i, j, k;
    FILE			*fp;
    float			u, var;
    unsigned char	image[ROWS][COLUMNS];
    char			*ifile, ch;
    char			filename[4][50];

    strcpy(filename[0], "image1.raw");
    strcpy(filename[1], "image2.raw");
    strcpy(filename[2], "image3.raw");
    strcpy(filename[3], "image4.raw");



    //Assign each image name in filename to ifile here
    /* example: ifile = filename[k]; k=0,1,2,3; a loop might be needed*/

    /* iterate though all the file names */
    for (k = 0; k < 4; k++)
    {
        ifile = filename[k];

        /* open file with read access */
        if (( fp = fopen( ifile, "rb" )) == NULL )
        {
            fprintf( stderr, "error: couldn't open %s\n", ifile );
            exit( 1 );
        }

        /* read pixel data from file */
        for ( i = 0; i < ROWS ; i++ )
            if ( fread( image[i], 1, COLUMNS, fp ) != COLUMNS )
            {
                fprintf( stderr, "error: couldn't read enough stuff\n" );
                exit( 1 );
            }

        fclose( fp );

        //Calculate Mean for each image here
        unsigned int u_sum = 0;
        /* iterate through all the pixels */
        for (i = 0; i < ROWS; i++)
        {
            for (j = 0; j < COLUMNS; j++)
            {
                u_sum += (unsigned)image[i][j];
            }
        }
        /* calculate the mean */
        u = (float)u_sum / (ROWS*COLUMNS);

        //Calculate Variance for each image here
        float var_sum = 0;
        /* iterate through all the pixels */
        for (i = 0; i < ROWS; i++)
        {
            for (j = 0; j < COLUMNS; j++)
            {
                var_sum += (image[i][j] - u) * (image[i][j] - u);
            }
        }
        /* calculate variance */
        var = var_sum / ((ROWS * COLUMNS) - 1);

        //Print mean and variance for each image
        printf("%s: {u=%f, var=%f}\n", ifile, u, var);
    }
    printf("Press any key to exit: ");
    std::cin>>ch;

    return 0;
}


