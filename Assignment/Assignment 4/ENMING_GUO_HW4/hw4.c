#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS		(int)480
#define COLUMNS		(int)640

void sobel(unsigned char input[ROWS][COLUMNS], unsigned char output[ROWS][COLUMNS],int reserved[ROWS][COLUMNS], int operator[3][3], int *max);
void sgm(int ximage[ROWS][COLUMNS], int yimage[ROWS][COLUMNS], unsigned char output[ROWS][COLUMNS], int *max);
void binary(unsigned char sgmimage[ROWS][COLUMNS], unsigned char output[ROWS][COLUMNS], int threshold);
void clear( unsigned char image[][COLUMNS] );
void header( int row, int col, unsigned char head[32] );

int main( int argc, char **argv )
{

	int				i, k, threshold[3], max[3],reservedx[ROWS][COLUMNS],reservedy[ROWS][COLUMNS];
	FILE			*fp;
	unsigned char	image[ROWS][COLUMNS], ximage[ROWS][COLUMNS], yimage[ROWS][COLUMNS], bimage[ROWS][COLUMNS],head[32],sgmimage[ROWS][COLUMNS];

	char			filename[3][50], outfilename[3][50], tempfilename[50];

	int sobel_x[3][3] = {
							{-1, 0, 1},
							{-2, 0, 2},
							{-1, 0, 1}
						};
	int sobel_y[3][3] = {
							{1, 2, 1},
							{0, 0, 0},
							{-1, -2, -1}
						};

	threshold[0] = 40;
	threshold[1] = 10;
	threshold[2] = 10;

	strcpy( filename[0], "input/image1" );
	strcpy( filename[1], "input/image2" );
	strcpy( filename[2], "input/image3" );

    strcpy( outfilename[0], "output/image1" );
    strcpy( outfilename[1], "output/image2" );
    strcpy( outfilename[2], "output/image3" );


	header ( ROWS, COLUMNS, head );

	printf( "\t\tMax_Gx\t\tMax_Gy\t\tMax_SGM\n" );
	for ( k = 0; k < 3; k ++)
	{
		clear( ximage );
		clear( yimage );

		/* Read in the image */

		if (( fp = fopen( strcat(filename[k], ".raw"), "rb" )) == NULL )
		{
		  fprintf( stderr, "error: couldn't open %s\n", filename[k] );
		  exit( 1 );
		}			
		for ( i = 0; i < ROWS ; i++ )
		  if ( fread( image[i], sizeof(char), COLUMNS, fp ) != COLUMNS )
		  {
			fprintf( stderr, "error: couldn't read enough stuff\n" );
			exit( 1 );
		  }
		fclose( fp );

		/* Compute Gx, Gy, SGM, find out the maximum and normalize*/
        memset(max,0,sizeof(int)*3);
		/* Gx */
		sobel(image, ximage, reservedx,sobel_x, &max[0]);
		
		/* Gy */
		sobel(image, yimage, reservedy,sobel_y, &max[1]);

		/* SGM */
		sgm(reservedx, reservedy, sgmimage, &max[2]);

		/* Binary */
		binary(sgmimage, bimage, threshold[k]);


		/* Write Gx to a new image*/
        strcpy( tempfilename, outfilename[k] );
		if (!( fp = fopen( strcat( tempfilename, "_x.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", tempfilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for ( i = 0 ; i < ROWS ; i++ ) fwrite( ximage[i], 1, COLUMNS, fp );	
	    fclose( fp );
					
		/* Write Gy to a new image */
        strcpy( tempfilename, outfilename[k] );
		if (!( fp = fopen( strcat( tempfilename, "_y.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", tempfilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for ( i = 0 ; i < ROWS ; i++ ) fwrite( yimage[i], 1, COLUMNS, fp );		
		fclose( fp );

		/* Write SGM to a new image */
        strcpy( tempfilename, outfilename[k] );
		if (!( fp = fopen( strcat( tempfilename, "_s.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n",tempfilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for ( i = 0 ; i < ROWS ; i++ ) fwrite( sgmimage[i], 1, COLUMNS, fp );	
		fclose( fp );
		
		/* Write the binary image to a new image */
        strcpy( tempfilename, outfilename[k] );
		if (!( fp = fopen( strcat( tempfilename, "_b.ras" ), "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", tempfilename );
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for ( i = 0 ; i < ROWS ; i++ ) fwrite( bimage[i], 1, COLUMNS, fp );
		fclose( fp );

		printf( "Image%d:  %5d\t\t%5d\t\t%5d\n", k+1,max[0], max[1], max[2] );

	}

	return 0;
}


void sobel(unsigned char input[ROWS][COLUMNS], unsigned char output[ROWS][COLUMNS], int reserved[ROWS][COLUMNS],int operator[3][3], int *max)
{
	int i, j;
    int delx, dely;
    int sobel_sum;
	float norm;


	memset(max,0,sizeof(int));

	for (i = 1; i < (ROWS - 1); i++) {
		for (j = 1; j < (COLUMNS - 1); j++) {
			sobel_sum = 0;
			for (delx = -1; delx < 2; delx++) {
				for (dely = -1; dely < 2; dely++) {
                    int a=input[i+delx][j+dely];
                    int b=operator[delx+1][dely+1];
					sobel_sum += a*b;
				}
			}

			reserved[i][j] = abs(sobel_sum);

			if (reserved[i][j] > *max) {
				*max = reserved[i][j];
			}
		}
	}

	norm = 255.0 / (float)(*max);

    for(i=0;i<ROWS;i++){
        for(j=0;j<COLUMNS;j++){
            output[i][j]=0;
        }
    }
	
	/* normalize the image */
	for (i = 1; i < ROWS-1; i++) {
		for (j = 1; j < COLUMNS-1; j++) {
			output[i][j] = (unsigned char)(norm * (float)abs(reserved[i][j]));
		}
	}
}


void sgm(int ximage[ROWS][COLUMNS], int yimage[ROWS][COLUMNS], unsigned char output[ROWS][COLUMNS], int *max)
{
	/* iterate through the pixels */
	int i, j;	/* used for convolution */
	float norm;	/* normalizing factor */
	int temp[ROWS][COLUMNS];	/* int image used to save unnormalized image */

	/* reset max */
    memset(max,0,sizeof(int));

	for (i = 1; i < (ROWS-1); i++) {
		for (j = 1; j < (COLUMNS-1); j++) {
			/* save the raw value of sgm */
			temp[i][j] = (ximage[i][j] * ximage[i][j]) + (yimage[i][j] * yimage[i][j]);
			
			/* find the maximum */
			if (temp[i][j] > *max) {
				*max = temp[i][j];
			}
		}
	}

	/* calculate the normalizing factor */
	norm = 255.0 / (float)(*max);
	
	/* normalize the image */
	for (i = 0; i < ROWS; i++) {
		for (j = 0; j < COLUMNS; j++) {
			output[i][j] = (unsigned char)(norm * (float)abs(temp[i][j]));
		}
	}
}


void binary(unsigned char sgmimage[ROWS][COLUMNS], unsigned char output[ROWS][COLUMNS], int threshold)
{
	int i, j;

	/* iterate through the image pixels */
	for (i = 0; i < ROWS; i++)
	{
		for (j = 0; j < COLUMNS; j++)
		{
			/* compare pixel with threshold */
			if (sgmimage[i][j] > threshold)
			{
				output[i][j] = 255;
			}
			else
			{
				output[i][j] = 0;
			}
		}
	}
}


void clear( unsigned char image[][COLUMNS] )
{
	int	i,j;
	for ( i = 0 ; i < ROWS ; i++ )
		for ( j = 0 ; j < COLUMNS ; j++ ) image[i][j] = 0;
}

void header( int row, int col, unsigned char head[32] )
{
	int *p = (int *)head;
	char *ch;
	int num = row * col;

	/* Choose little-endian or big-endian header depending on the machine. Don't modify this */
	/* Little-endian for PC */
	
	*p = 0x956aa659;
	*(p + 3) = 0x08000000;
	*(p + 5) = 0x01000000;
	*(p + 6) = 0x0;
	*(p + 7) = 0xf8000000;

	ch = (char*)&col;
	head[7] = *ch;
	ch ++; 
	head[6] = *ch;
	ch ++;
	head[5] = *ch;
	ch ++;
	head[4] = *ch;

	ch = (char*)&row;
	head[11] = *ch;
	ch ++; 
	head[10] = *ch;
	ch ++;
	head[9] = *ch;
	ch ++;
	head[8] = *ch;
	
	ch = (char*)&num;
	head[19] = *ch;
	ch ++; 
	head[18] = *ch;
	ch ++;
	head[17] = *ch;
	ch ++;
	head[16] = *ch;
	

	/* Big-endian for unix */
	/*
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
