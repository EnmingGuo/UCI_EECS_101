#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ROWS	240
#define COLUMNS	240
#define PI 3.14159265358979323846

void clear( unsigned char image[][COLUMNS] );
void header( int row, int col, unsigned char head[32] );
void generate(unsigned char image[ROWS][COLUMNS], float r, float a, float m, float S[3],float V[3]);

int main( int argc, char** argv )
{
	int				i, j, k;
	FILE*			fp;
	unsigned char	image[ROWS][COLUMNS], head[32];
	char			filename[50], ch;
	float           S[3][3] = { { 0, 0, 1 }, { 1/sqrt(3), 1/sqrt(3), 1/sqrt(3) }, { 1, 0, 0 } }, V[] = { 0, 0, 1 }, H[3], N[3], E[ROWS][COLUMNS];
	float			R[3] = { 50, 10, 100 }, M[3] = { 1, 0.1, 10000 }, A[3] = { 0.5, 0.1, 1 };
	float			r, m, a, s[3], alpha, x, y, costheta, len;

	header ( ROWS, COLUMNS, head );
	
	for ( k = 0; k < 9; k ++ )
	{
		clear(image);
        char id = (k+1)+'0';
        sprintf(filename,"output/image%c.ras",id);

        switch(k){
            case 0:
                generate(image, R[0], A[0], M[0], S[0],V);
                printf("case 1 Finish!\n");
            break;
            case 1:
                generate(image, R[0], A[0], M[0], S[1],V);
                printf("case 2 Finish!\n");
            break;
            case 2:
                generate(image, R[0], A[0], M[0], S[2],V);
                printf("case 3 Finish!\n");
            break;
            case 3:
                generate(image, R[1], A[0], M[0], S[0],V);
                printf("case 4 Finish!\n");
            break;
            case 4:
                generate(image,R[2],A[0],M[0], S[0],V);
                printf("case 5 Finish!\n");
            break;
            case 5:
                generate(image, R[0], A[1], M[0], S[0],V);
                printf("case 6 Finish!\n");
            break;
            case 6:
                generate(image, R[0], A[2], M[0], S[0],V);
                printf("case 7 Finish!\n");
            break;
            case 7:
                generate(image, R[0], A[0], M[1],S[0],V);
                printf("case 8 Finish!\n");
            break;
            case 8:
                generate(image, R[0], A[0], M[2], S[0],V);
                printf("case 9 Finish!\n");
            break;
        }

		if (!( fp = fopen( filename, "wb" ) ))
		{
		  fprintf( stderr, "error: could not open %s\n", filename);
		  exit( 1 );
		}
		fwrite( head, 4, 8, fp );
		for ( i = 0 ; i < ROWS ; i++ )
		  fwrite( image[i], sizeof(char), COLUMNS, fp );
		fclose( fp );
	}

	printf("Press any key to exit: ");
	gets ( &ch );
	return 0;
}

void clear( unsigned char image[][COLUMNS] )
{
	int	i,j;
	for ( i = 0 ; i < ROWS ; i++ )
		for ( j = 0 ; j < COLUMNS ; j++ ) image[i][j] = 0;
}

void generate(unsigned char image[ROWS][COLUMNS], float r, float a, float m, float S[3], float V[3])
{
    int i, j, x, y;
    float H_mag, L_l, L_s, alpha, L;
    float H[3], N [3];

    // generate h vector
    for (i = 0; i < 3; i++)
    {
        H[i] = S[i] + V[i];
    }

    // normalize the vector
    H_mag = sqrt(pow(H[0], 2) + pow(H[1], 2) + pow(H[2], 2));
    for (i = 0; i < 3; i++)
    {
        H[i] = H[i] / H_mag;
    }

    // iterate through the image
    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLUMNS; j++)
        {
            //printf("%d %d\n",i,j);
            // change origin
           x = j - COLUMNS/2;

           y = ROWS/2 - i ;

            // check if the point is on the sphere
            if (pow(x, 2) + pow(y, 2) < pow(r, 2)) {
                float temp = sqrt(pow(r,2)-(pow(x,2)+pow(y,2)));
                // find the unit normal
                N[0] = x/temp;
                N[1] = y/temp;
                N[2] = 1;
            }
            else if(pow(x,2) + pow (y,2) == pow (r,2)){
                N[0]=x;
                N[1]=y;
                N[2]=0;
            }
            else{
                continue;
            }

            float N_mag = sqrt(pow(N[0],2)+pow(N[1],2)+pow(N[2],2));

            for(int i =0;i<3;i++){
                N[i] = N[i] / N_mag;
            }

            // use dot product of two unit vectors to find angle between them
            L_l = (N[0] * S[0]) + (N[1] * S[1]) + (N[2] * S[2]);

            L_l = L_l / (sqrt(pow(N[0],2)+pow(N[1],2)+pow(N[2],2))* sqrt(pow(S[0],2)+pow(S[1],2)+pow(S[2],2)));

            // use dot product to get alpha
            alpha = (N[0] * H[0]) + (N[1] * H[1]) + (N[2] * H[2]);

            alpha = alpha / (sqrt(pow(N[0],2)+pow(N[1],2)+pow(N[2],2))* sqrt(pow(H[0],2)+pow(H[1],2)+pow(H[2],2)));

            alpha = acosf(alpha);
            // calculate Ls
            L_s = exp((-1.0)*pow((alpha / m), 2));

            // calculate L
            L = (L_l * a) + (L_s * (1 - a));

            // remap L (0 -> 1) to pixel (0 -> 255)
            if (L >= 0.0)
            {
                image[i][j] = L * 255;
            } else if (L > 1.0)
            {
                image[i][j] = 255;
            }
            
        }
    }

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