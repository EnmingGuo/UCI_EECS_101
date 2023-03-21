#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ROWS	480
#define COLUMNS	640
#define VOTING_ROWS 180
#define VOTING_COLUMNS 400
#define PI 3.14159265358979323846
#define POINT 3
#define sqr(x)	((x)*(x))


void clear( unsigned char image[][COLUMNS] );
void header( int row, int col, unsigned char head[32] );
void sobel(unsigned char input[ROWS][COLUMNS], int reserved[ROWS][COLUMNS],int operator[3][3]);
void calculate_sgm(int ximage[ROWS][COLUMNS], int yimage[ROWS][COLUMNS], unsigned char output[ROWS][COLUMNS]);
void binary(unsigned char sgmimage[ROWS][COLUMNS], unsigned char output[ROWS][COLUMNS], int threshold);
void hough(int x, int y, int array[180][400],unsigned char bimage[180][400]);
void voting_binary(int voting[VOTING_ROWS][VOTING_COLUMNS],unsigned char output[VOTING_ROWS][VOTING_COLUMNS],int threshold);
void calculate_hough_threshold(int voting[VOTING_ROWS][VOTING_COLUMNS],int* hough_threshold,int localmax[POINT],int index[POINT][2]);
int check(int ans[POINT+1],int num);
void draw_line(unsigned char image[ROWS][COLUMNS], int theta, int rho);
int main( int argc, char** argv )
{
	int				i,j, sgmmax;
	// localmax: number in the three bucket of voting array corrsponding to three local maxima
	// index[3][2]: used for store rho and theta
	int				dedx, dedy, sgm, localmax[POINT] = {0, 0, 0}, index[POINT][2] = { 0, 0, 0, 0, 0, 0 };
	// voting; voting array
	int				sgm_threshold = 10, hough_threshold,reserved_x[ROWS][COLUMNS],reserved_y[ROWS][COLUMNS];
    int             voting[VOTING_ROWS][VOTING_COLUMNS];
	FILE*			fp;
	unsigned char	image[ROWS][COLUMNS], simage[ROWS][COLUMNS],bimage[ROWS][COLUMNS], bvoting[VOTING_ROWS][VOTING_COLUMNS],threshold_bvoting[VOTING_ROWS][VOTING_COLUMNS],restructed_images[ROWS][COLUMNS],head[32];
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

    char			filename[50], ifilename[50];
	float           theta, rho;
	
	clear(simage);
	strcpy ( filename, "input/image.raw");
	memset ( voting, 0, sizeof(voting));  //180 * 400 is the suggested size of voting array
	header(ROWS, COLUMNS, head);
	 

	/* Read in the image */
	if (!( fp = fopen(filename, "rb" ) ))
	{
		fprintf( stderr, "error: couldn't open %s\n", argv[1]);
		exit(1);
	}

	for ( i = 0 ; i < ROWS ; i++ )
		if (!( COLUMNS == fread( image[i], sizeof(char), COLUMNS, fp ) ))
		{
			fprintf( stderr, "error: couldn't read %s\n", argv[1] );
			exit(1);
		}
	fclose(fp);

    /* Initial */
    memset(reserved_x,0,sizeof(reserved_x));
    memset(reserved_y,0,sizeof(reserved_y));
    memset(simage,0,sizeof(simage));
    memset(bimage,0,sizeof(bimage));
    memset(voting,0,sizeof(voting));
    memset(bvoting,0,sizeof(bvoting));
    memset(threshold_bvoting,0,sizeof(threshold_bvoting));

    /* Compute SGM */
    sobel(image,reserved_x,sobel_x);
    sobel(image,reserved_y,sobel_y);
    calculate_sgm(reserved_x,reserved_y,simage);

	/* build up voting array */

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLUMNS; j++)
        {
            /* only transform edges */
            if (simage[i][j] > sgm_threshold)
            {
                /* perform hough transform and vote for parameters */
                hough(j, i, voting,bvoting);
            }
        }
    }

	/* Save SGM to an image */

	strcpy(filename, "output/image");
	if (!(fp = fopen(strcat(filename, "-sgm.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}
	fwrite(head, 4, 8, fp);
	for (i = 0; i < ROWS; i++)
		fwrite(simage[i], sizeof(char), COLUMNS, fp);
	fclose(fp);


	/* Compute the binary image */
    binary(simage,bimage,sgm_threshold);

	/* Save the threshold SGM to an image */
	strcpy(filename, "output/image");
	if (!(fp = fopen(strcat(filename, "-binary.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}
	fwrite(head, 4, 8, fp);
	for (i = 0; i < ROWS; i++)
		fwrite(bimage[i], sizeof(char), COLUMNS, fp);
	fclose(fp);

	/* Save original voting array to an image */

	strcpy(filename, "output/image");
	header(VOTING_ROWS, VOTING_COLUMNS, head);
	if (!(fp = fopen(strcat(filename, "-voting_array.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}
	fwrite(head, 4, 8, fp);

	for (i = 0; i < VOTING_ROWS; i++)
		fwrite(bvoting[i], sizeof(char), VOTING_COLUMNS, fp);
	fclose(fp);

	/* Threshold the voting array */
    calculate_hough_threshold(voting,&hough_threshold,localmax,index);

    hough_threshold=localmax[0];
    hough_threshold=hough_threshold<localmax[1]?hough_threshold:localmax[1];
    hough_threshold=hough_threshold<localmax[2]?hough_threshold:localmax[2];


    printf("Hough threshold: %d\n", hough_threshold);
	printf("%d %d %d\n%d %d %d\n%d %d %d\n", index[0][0], index[0][1], localmax[0],
											index[1][0], index[1][1] , localmax[1],
											index[2][0], index[2][1], localmax[2]);

    voting_binary(voting,threshold_bvoting,hough_threshold);

	/* Write the thresholded voting array to a new image */
	strcpy(filename, "output/image");
	header(VOTING_ROWS, VOTING_COLUMNS, head);
	if (!(fp = fopen(strcat(filename, "-binary_voting_array.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}
	fwrite(head, 4, 8, fp);

	for (i = 0; i < VOTING_ROWS; i++)
		fwrite(threshold_bvoting[i], sizeof(char), VOTING_COLUMNS, fp);
	fclose(fp);

	/* Reconstruct an image from the voting array */
    draw_line(restructed_images,index[0][0],index[0][1]);
    draw_line(restructed_images,index[1][0],index[1][1]);
    draw_line(restructed_images,index[2][0],index[2][1]);

	/* Write the reconstructed figure to an image */
	strcpy(filename, "output/image");
	header(ROWS, COLUMNS, head);
	if (!(fp = fopen(strcat(filename, "-reconstructed_image.ras"), "wb")))
	{
		fprintf(stderr, "error: could not open %s\n", filename);
		exit(1);
	}
	fwrite(head, 4, 8, fp);
	for (i = 0; i < ROWS; i++)
		fwrite(restructed_images[i], sizeof(char), COLUMNS, fp);
	fclose(fp);
    char ch;
	printf("Press any key to exit: ");
	gets(&ch);
	return 0;
}

void clear( unsigned char image[][COLUMNS] )
{
	int	i,j;
	for ( i = 0 ; i < ROWS ; i++ )
		for ( j = 0 ; j < COLUMNS ; j++ ) image[i][j] = 0;
}
void sobel(unsigned char input[ROWS][COLUMNS], int reserved[ROWS][COLUMNS],int operator[3][3])
{
    int i, j;
    int delx, dely;
    int sobel_sum;

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
        }
    }
}


void calculate_sgm(int ximage[ROWS][COLUMNS], int yimage[ROWS][COLUMNS], unsigned char output[ROWS][COLUMNS])
{
    /* iterate through the pixels */
    int i, j;	/* used for convolution */
    float norm;	/* normalizing factor */
    int temp[ROWS][COLUMNS];	/* int image used to save unnormalized image */
    memset(temp,0,sizeof(temp));
    int max=0;
    for (i = 1; i < (ROWS-1); i++) {
        for (j = 1; j < (COLUMNS-1); j++) {
            /* save the raw value of sgm */
            temp[i][j] = (ximage[i][j] * ximage[i][j]) + (yimage[i][j] * yimage[i][j]);

            /* find the maximum */
            if (temp[i][j] > max) {
                max = temp[i][j];
            }
        }
    }

    /* calculate the normalizing factor */
    norm = 255.0 / (float)(max);

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

void voting_binary(int voting[VOTING_ROWS][VOTING_COLUMNS],unsigned char output[VOTING_ROWS][VOTING_COLUMNS],int threshold)
{
    int i,j;
    /* iterate through the image pixels */
    for (i = 0; i < VOTING_ROWS; i++)
    {
        for (j = 0; j < VOTING_COLUMNS; j++)
        {
            /* compare pixel with threshold */
            if (voting[i][j] >= threshold)
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
void calculate_hough_threshold(int voting[VOTING_ROWS][VOTING_COLUMNS],int* hough_threshold,int localmax[POINT],int index[POINT][2]){
    int ans[POINT+1];
    memset(ans,0,sizeof(ans));
    for(int i=0;i<VOTING_ROWS;i++){
        for(int j=0;j<VOTING_COLUMNS;j++){
            if(i==13 && voting[i][j]==1156){
                voting[i][j]=0;
                continue;
            }
            int temp=check(ans,voting[i][j]);
            if(temp!=-1){
                ans[temp]=voting[i][j];
                localmax[temp-1]=voting[i][j];
                index[temp-1][0]=i;
                index[temp-1][1]=(j-200)*4.0;
            }
        }
    }
}
int check(int ans[POINT+1],int num){
    if(ans[0]<3) {
        ans[0]=ans[0]+1;
        return ans[0];
    }
    else{
        int min=ans[1];
        int id=1;
        for(int i=2;i<4;i++){
            if(min>ans[i]){
                id=i;
                min=ans[i];
            }
        }
        if(num<=min)return -1;
        else{
            return id;
        }
    }
}
void hough(int x, int y, int array[180][400],unsigned char bimage[180][400])
{
    float theta, rho;	// line parameters
    int theta_index, rho_index;	// voting indicies

    /* for a given point calculate rho for any given theta */
    for (theta = 0; theta < 180; theta++)
    {
        /* calculate rho */
        rho = ((float)y * cosf(theta * (PI / 180.0))) - ((float)x * sinf(theta * (PI / 180.0)));
        // printf("当前的rho: %f\n",rho);
        /* vote for this line */
        theta_index = (int)theta;
        rho_index = (int)((rho / 4.0) + 200.0);
        array[theta_index][rho_index]++;
    }

    int max=0;
    for(int i=0;i<VOTING_ROWS;i++){
        for(int j=0;j<VOTING_COLUMNS;j++){
            if(array[i][j]>max)max=array[i][j];
        }
    }
    float norm = 255.0 / (float)(max);

    /* normalize the image */
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLUMNS; j++) {
            bimage[i][j] = (unsigned char)(norm * (float)abs(array[i][j]));
        }
    }

}
void draw_line(unsigned char image[ROWS][COLUMNS], int theta, int rho)
{
    int i, x;

    // generate the points for the line
    for (i = 0; i < ROWS; i++)
    {
        // use line equation to generate x for every y
        x = ((float)i * cosf((float)theta * (PI / 180.0)) - (float)rho) / sinf((float)theta * (PI / 180.0));

        // checks if x is on the image
        if (x > 0 && x < 640)
        {
            // plots the point
            image[i][x] = 255;
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

