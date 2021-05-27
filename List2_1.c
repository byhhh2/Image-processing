/***************************************************************************
 * File: filter operation test                                             *
 *                                                                         *
 * Desc: This program performs filtering operation with zone plate image   *
 ***************************************************************************/


#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "ip.h"

 //#define operation(VALUE)        ((float) VALUE * 1.0) // original image

extern void write_pnm(image_ptr ptr, char filein[], int rows,
    int cols, int magic_number);
extern image_ptr read_pnm(char* filename, int* rows, int* cols,
    int* type);

void convolve(image_ptr source, int cols, int rows, int kwidth, int kheight,
    float* kernel, int bias, char* filename);

void swap(int* array, int i, int j);

void quicksort(int* array, int left, int right);

int median(int* window, int wsize);

void median_filt(image_ptr source, int cols, int rows, char* filename, int side);

int mean(int* window, int wsize);

void mean_filt(image_ptr source, int cols, int rows, char* filename, int side);

void scale_pnm(image_ptr buffer, char* fileout, int rows, int cols,
    float x_scale, float y_scale, int type);

void bilinear(image_ptr buffer, char* fileout, int rows, int cols,
    float x_scale, float y_scale, int type);

void cubic(image_ptr buffer, char* fileout, int rows, int cols,
    float x_scale, float y_scale, int type);

unsigned char cubic_interpolation(unsigned char image[4][4],
    float x, float y, double a);

int main(int argc, char* argv[])
{
    char filein[100];                   /* name of input file */
    char fileout[100];                  /* name of output file */
    int rows, cols;                     /* image rows and columns */
    unsigned long i;                    /* counting index */
    unsigned long bytes_per_pixel;      /* number of bytes per image pixel */
    image_ptr buffer;                   /* pointer to image buffer */
    // image_ptr obuffer;                   /* pointer to image buffer */
    unsigned long number_of_pixels;     /* total number of pixels in image */
    int type;                           /* what type of image data */

//////////////////////////////////////////////////////////////////////////////////////////
    // Convolution 계수 저장
    //float mask[3][3] = { {-0.111, -0.111, -0.111},{-0.111, 1.889, -0.111},{-0.111, -0.111, -0.111} };

    //Prewitt operator 
    //Gx
    //float mask[3][3] = { {-1.,-1.,-1.}, {0.,0.,0.}, {1.,1.,1.} };
    //Gy
    //float mask[3][3] = { {-1.,0.,1.}, {-1.,0.,1.}, {-1.,0.,1.} };

    //Sobel operator 
    //Gx
    //float mask[3][3] = { {-1.,-2.,-1.}, {0.,0.,0.}, {1.,2.,1.} };
    //Gy
    //float mask[3][3] = { {-1.,0.,1.}, {-2.,0.,2.}, {-1.,0.,1.} };

    //Compass
    /*
    //Prewitt
    //west
    float mask[3][3] = { {-1.,1.,1.}, {-1.,-2.,1.}, {-1.,1.,1.} };
    //northwest
    float mask[3][3] = { {-1.,-1.,1.}, {-1.,-2.,1.}, {1.,1.,1.} };
    //north
    float mask[3][3] = { {-1.,-1.,-1.}, {1.,-2.,1.}, {1.,1.,1.} };
    //northeast
    float mask[3][3] = { {1.,-1.,-1.}, {1.,-2.,-1.}, {1.,1.,1.} };
    //southwest
    float mask[3][3] = { {1.,1.,1.}, {-1.,-2.,1.}, {-1.,-1.,1.} };
    //south
    float mask[3][3] = { {1.,1.,1.}, {1.,-2.,1.}, {-1.,-1.,-1.} };
    //southeast
    float mask[3][3] = { {1.,1.,1.}, {1.,-2.,-1.}, {1.,-1.,-1.} };
    //east
    float mask[3][3] = { {1.,1.,-1.}, {1.,-2.,-1.}, {1.,1.,-1.} };
    */

    //Kirsh
    //west
    //float mask[3][3] = { {-3.,-3.,5.}, {-3.,0.,5.}, {-3.,-3.,5.} };
    //northwest 
    //float mask[3][3] = { {-3.,-3.,-3.}, {-3.,0.,5.}, {-3.,5.,5.} };
    //north
    //float mask[3][3] = { {-3.,-3.,-3.}, {-3.,0.,-3.}, {5.,5.,5.} };
    //northeast
    //float mask[3][3] = { {-3.,-3.,-3.}, {5.,0.,-3.}, {5.,5.,-3.} };
    //southwest
    //float mask[3][3] = { {-3.,5.,5.}, {-3.,0.,5.}, {-3.,-3.,-3.} };
    //south
    //float mask[3][3] = { {5.,5.,5.}, {-3.,0.,-3.}, {-3.,-3.,-3.} };
    //southeast
    //float mask[3][3] = { {5.,5.,-3.}, {5.,0.,-3.}, {-3.,-3.,-3.} };
    //east
    //float mask[3][3] = { {5.,-3.,-3.}, {5.,0.,-3.}, {5.,-3.,-3.} };


    //Robinson 3-level //lena
    //west
    //float mask[3][3] = { {-1.,0.,1.}, {-1.,0.,1.}, {-1.,0.,1.} };
    //northwest 
    //float mask[3][3] = { {-1.,-1.,0.}, {-1.,0.,1.}, {0.,1.,1.} };
    //north
    //float mask[3][3] = { {-1.,-1.,-1.}, {0.,0.,0.}, {1.,1.,1.} };
    //northeast
    //float mask[3][3] = { {0.,-1.,-1.}, {1.,0.,-1.}, {1.,1.,0.} };
    //southwest
    //float mask[3][3] = { {0.,1.,1.}, {-1.,0.,1.}, {-1.,-1.,0.} };
    //south
    //float mask[3][3] = { {1.,1.,1.}, {0.,0.,0.}, {-1.,-1.,-1.} };
    //southeast
    //float mask[3][3] = { {1.,1.,0.}, {1.,0.,-1.}, {0.,-1.,-1.} };
    //east
    //float mask[3][3] = { {1.,0.,-1.}, {1.,0.,-1.}, {1.,0.,-1.} };


    //Robinson 5-level //czp
    //west
    //float mask[3][3] = { {-1.,0.,1.}, {-2.,0.,2.}, {-1.,0.,1.} };
    //northwest 
    //float mask[3][3] = { {-2.,-1.,0.}, {-1.,0.,1.}, {0.,1.,2.} };
    //north
    //float mask[3][3] = { {-1.,-2.,-1.}, {0.,0.,0.}, {1.,2.,1.} };
    //northeast
    //float mask[3][3] = { {0.,-1.,-2.}, {1.,0.,-1.}, {2.,1.,0.} };
    //southwest
    //float mask[3][3] = { {0.,1.,2.}, {-1.,0.,1.}, {-2.,-1.,0.} };
    //south
    //float mask[3][3] = { {1.,2.,1.}, {0.,0.,0.}, {-1.,-2.,-1.} };
    //southeast
    //float mask[3][3] = { {2.,1.,0.}, {1.,0.,-1.}, {0.,-1.,-2.} };
    //east
    //float mask[3][3] = { {1.,0.,-1.}, {2.,0.,-2.}, {1.,0.,-1.} };


    //LoG
    //float mask[5][5] = { {0.,0.,-1.,0.,0.}, {0.,-1.,-2.,-1.,0.}, {-1.,-2.,16.,-2.,-1.}, {0.,-1.,-2.,-1.,0.}, {0.,0.,-1.,0.,0.} };



    //west
    //northwest 
    //north
    //northeast
    //southwest
    //south
    //southeast
    //east


    /* set input filename and output file name */
    if (argc == 3)
    {
        strcpy(filein, argv[1]);
        strcpy(fileout, argv[2]);
    }
    else
    {
        printf("Input name of input file\n");
        gets(filein);
        printf("\nInput name of output file\n");
        gets(fileout);
        printf("\n");
    }

    buffer = read_pnm(filein, &rows, &cols, &type);

    /* determine bytes_per_pixel, 3 for color, 1 for gray-scale */
    if (type == PPM)
        bytes_per_pixel = 3;
    else
        bytes_per_pixel = 1;

    number_of_pixels = bytes_per_pixel * rows * cols;

    ////////////////////////////////////////////////////////////////
     // convolve 함수 call을 이용한 convolution 기능 수행
    //convolve(buffer, cols, rows, 5, 5, mask, 0, fileout);

    //median filter
    //median_filt(buffer, cols, rows, fileout, 3);


    //mean filter
    //mean_filt(buffer, cols, rows, fileout, 3);

    //scale pnm
    //scale_pnm(buffer, fileout, rows, cols, 3, 3, 5);

    //bilinear
    //bilinear(buffer, fileout, rows, cols, 2, 2, 5);

    //cubic
    cubic(buffer, fileout, rows, cols, 2, 2, 5); //cubic

    ////////////////////////////////////////////////////////////////
     // makeczp 함수 call을 이용한 zone plate 영상 생성

        //write_pnm(buffer, fileout, rows, cols, type);
    IP_FREE(buffer);
    return 0;
}

/***************************************************************************
 * Func: convolve                                                           *
 *                                                                         *
 * Desc: convolves an image with the floating point kernel passed  and     *
 *       writes out new image one line at a time                           *
 *                                                                         *
 * Params: source - pointer to image in memory                             *
 *         cols - number of columns in image                               *
 *         rows - number of rows in image                                  *
 *         kwidth - width of convolution kernel                            *
 *         kheight - height of convolution kernel                          *
 *         kernel - pointer to convolution kernel                          *
 *         bias - value to add to convolution sum                          *
 *         filename - name of output file                                  *
 ***************************************************************************/

void convolve(image_ptr source, int cols, int rows, int kwidth, int kheight,
    float* kernel, int bias, char* filename)
{
    int x, y, i;           /* image loop variables */
    int kernx, kerny;      /* kernel loop variables */
    int index;             /* image index */
    int xextra, yextra;       /* size of boundary */
    int conv_line;            /* size of output line */
    float sum;                /* accumulator used during convolution */
    unsigned long destadd;    /* destination image address */
    unsigned long sourceadd;  /* index into source image */
    unsigned long sourcebase; /* address of line */
    unsigned char* dest;      /* destination image line */
    FILE* fp;                 /* output file pointer */
    unsigned char left[25];   /* storage of left pixel for duplication */
    unsigned char right[25];  /* storage of right pixel for duplication */
    int xpad, ypad;           /* number of pixels to duplicate at edges */
    int last_line;            /* last line to process */

    yextra = (kheight / 2) * 2;
    ypad = yextra / 2;
    xextra = (kwidth / 2) * 2;
    xpad = xextra / 2;
    conv_line = cols - xextra;
    last_line = rows - yextra;
    dest = malloc(cols);

    if ((fp = fopen(filename, "wb")) == NULL)
    {
        printf("Unable to open %s for output\n", filename);
        exit(1);
    }
    fprintf(fp, "P5\n%d %d\n255\n", cols, rows); /* print out header */

    for (y = 0; y < last_line; y++)
    {
        sourcebase = (unsigned long)cols * y;
        destadd = 0;
        for (x = xextra / 2; x < (cols - xpad); x++)
        {
            sum = 0.0;
            index = 0;
            for (kerny = 0; kerny < kheight; kerny++)
                for (kernx = 0; kernx < kwidth; kernx++)
                {
                    sourceadd = sourcebase + kernx + kerny * cols;
                    sum += (source[sourceadd] * kernel[index++]);
                }

            sum += bias;
            CLIP(sum, 0.0, 255.0);

            dest[destadd++] = (unsigned char)sum;
            sourcebase++;
        } /* for x */
        for (i = 0; i < xpad; i++)
            left[i] = dest[0];
        for (i = 0; i < xpad; i++)
            right[i] = dest[conv_line - 1];
        if (y == 0)
            for (i = 0; i < ypad; i++)
            {
                fwrite(left, 1, xpad, fp);
                fwrite(dest, 1, conv_line, fp);
                fwrite(right, 1, xpad, fp);
            }
        fwrite(left, 1, xpad, fp);
        fwrite(dest, 1, conv_line, fp);
        fwrite(right, 1, xpad, fp);
        if (y == (last_line - 1))
            for (i = 0; i < ypad; i++)
            {
                fwrite(left, 1, xpad, fp);
                fwrite(dest, 1, conv_line, fp);
                fwrite(right, 1, xpad, fp);
            }
    } /* for y */

}


/***************************************************************************
 * Func: swap                                                              *
 *                                                                         *
 * Desc: swaps two elements in an array                                    *
 *                                                                         *
 * Params: array - array of element (two of which will be swapped)         *
 *         i - first element in array to swap                              *
 *         j - second element in array to swap                             *
 ***************************************************************************/
void swap(int* array, int i, int j)
{
    int temp;

    temp = array[i];
    array[i] = array[j];
    array[j] = temp;
}


/***************************************************************************
 * Func: quicksort                                                        *
 *                                                                         *
 * Desc: sorts an array of integers using the quicksort algorithm         *
 *                                                                         *
 * Params: array - the array of integers                                  *
 *         left - leftmost entry in array                                 *
 *         right - rightmost entry in array                               *
 ***************************************************************************/

void quicksort(int* array, int left, int right)
{
    int i, last;

    if (left >= right)
        return;
    swap(array, left, (left + right) / 2);
    last = left;
    for (i = left + 1; i <= right; i++)
        if (array[i] < array[left])
            swap(array, ++last, i);
    swap(array, left, last);
    quicksort(array, left, last - 1);
    quicksort(array, last + 1, right);
}

/****************************************************************************
 * Func: median                                                             *
 *                                                                          *
 * Desc: finds median value of an image block                               *
 *                                                                          *
 * Param: window - 3x3 window of source image                               *
 *                                                                          *
 * Returns: the median value                                                *
 ****************************************************************************/

int median(int* window, int wsize)
{
    quicksort(window, 0, wsize);
    return window[wsize / 2];
}

int mean(int* window, int wsize)
{
    int i;
    int sum = 0;
    int avg;

    for (i = 0; i < wsize; i++)
    {
        sum += window[i];
    }

    avg = sum / wsize;
    window[wsize / 2] = avg;
    return window[wsize / 2];
}


/***************************************************************************
 * Func: median_filt                                                       *
 *                                                                         *
 * Desc: median filters an image using a block mask                        *
 *                                                                         *
 * Params: source - pointer to image in memory                             *
 *         cols - number of columns in image                               *
 *         rows - number of rows in image                                  *
 *         filename - name of output file                                  *
 *         side - width of block                                           *
 ***************************************************************************/

void median_filt(image_ptr source, int cols, int rows, char* filename, int side)
{
    int x, y, i;             /* image loop variables */
    int index;               /* image index */
    int winx, winy;          /* index into the samples window */
    int xextra, yextra;      /* size of boundary */
    int conv_line;           /* size of output line */
    unsigned long destadd;   /* destination image address */
    unsigned long sourceadd, sourcebase; /* source addressing */
    char dest[1024];         /* destination image line */
    FILE* fp;                /* output file pointer */
    int* window;             /* window for block samples */
    unsigned char left[25];  /* storage for left edge pixel duplication */
    unsigned char right[25]; /* storage for right edge pixel duplication */
    int xpad, ypad;          /* size of padding */
    int last_line;           /* last line to process */
    int new_pixel;           /* newly processed pixel */
    int wsize;               /* number of elements in block */

    wsize = side * side;
    yextra = (side / 2) * 2;
    ypad = yextra / 2;
    xextra = yextra;
    xpad = ypad;
    conv_line = cols - xextra;

    window = malloc(wsize * sizeof(int));
    if ((fp = fopen(filename, "wb")) == NULL)
    {
        printf("Unable to open %s for output\n", filename);
        exit(1);
    }
    fprintf(fp, "P5\n%d %d\n255\n", cols, rows); /* print out header */
    last_line = rows - yextra;

    for (y = 0; y < last_line; y++)
    {
        sourcebase = (unsigned long)cols * y;
        destadd = 0;
        for (x = xpad; x < (cols - xpad); x++)
        {
            index = 0;
            for (winy = 0; winy < side; winy++)
                for (winx = 0; winx < side; winx++)
                {
                    sourceadd = sourcebase + winx + winy * cols;
                    window[index++] = (int)source[sourceadd];
                }
            new_pixel = median(window, wsize);
            dest[destadd++] = (unsigned char)new_pixel;
            sourcebase++;
        } /* for x */
        for (i = 0; i < xpad; i++)
            left[i] = dest[0];
        for (i = 0; i < xpad; i++)
            right[i] = dest[conv_line - 1];
        if (y == 0)
            for (i = 0; i < ypad; i++)
            {
                fwrite(left, 1, xpad, fp);
                fwrite(dest, 1, conv_line, fp);
                fwrite(right, 1, xpad, fp);
            }
        fwrite(left, 1, xpad, fp);
        fwrite(dest, 1, conv_line, fp);
        fwrite(right, 1, xpad, fp);
        if (y == (last_line - 1))
            for (i = 0; i < ypad; i++)
            {
                fwrite(left, 1, xpad, fp);
                fwrite(dest, 1, conv_line, fp);
                fwrite(right, 1, xpad, fp);
            }
    } /* for y */

    //free(window);
}



void mean_filt(image_ptr source, int cols, int rows, char* filename, int side)
{
    int x, y, i;             /* image loop variables */
    int index;               /* image index */
    int winx, winy;          /* index into the samples window */
    int xextra, yextra;      /* size of boundary */
    int conv_line;           /* size of output line */
    unsigned long destadd;   /* destination image address */
    unsigned long sourceadd, sourcebase; /* source addressing */
    char dest[1024];         /* destination image line */
    FILE* fp;                /* output file pointer */
    int* window;             /* window for block samples */
    unsigned char left[25];  /* storage for left edge pixel duplication */
    unsigned char right[25]; /* storage for right edge pixel duplication */
    int xpad, ypad;          /* size of padding */
    int last_line;           /* last line to process */
    int new_pixel;           /* newly processed pixel */
    int wsize;               /* number of elements in block */

    wsize = side * side;
    yextra = (side / 2) * 2;
    ypad = yextra / 2;
    xextra = yextra;
    xpad = ypad;
    conv_line = cols - xextra;

    window = malloc(wsize * sizeof(int));
    if ((fp = fopen(filename, "wb")) == NULL)
    {
        printf("Unable to open %s for output\n", filename);
        exit(1);
    }
    fprintf(fp, "P5\n%d %d\n255\n", cols, rows); /* print out header */
    last_line = rows - yextra;

    for (y = 0; y < last_line; y++)
    {
        sourcebase = (unsigned long)cols * y;
        destadd = 0;
        for (x = xpad; x < (cols - xpad); x++)
        {
            index = 0;
            for (winy = 0; winy < side; winy++)
                for (winx = 0; winx < side; winx++)
                {
                    sourceadd = sourcebase + winx + winy * cols;
                    window[index++] = (int)source[sourceadd];
                }
            new_pixel = mean(window, wsize);
            dest[destadd++] = (unsigned char)new_pixel;
            sourcebase++;
        } /* for x */
        for (i = 0; i < xpad; i++)
            left[i] = dest[0];
        for (i = 0; i < xpad; i++)
            right[i] = dest[conv_line - 1];
        if (y == 0)
            for (i = 0; i < ypad; i++)
            {
                fwrite(left, 1, xpad, fp);
                fwrite(dest, 1, conv_line, fp);
                fwrite(right, 1, xpad, fp);
            }
        fwrite(left, 1, xpad, fp);
        fwrite(dest, 1, conv_line, fp);
        fwrite(right, 1, xpad, fp);
        if (y == (last_line - 1))
            for (i = 0; i < ypad; i++)
            {
                fwrite(left, 1, xpad, fp);
                fwrite(dest, 1, conv_line, fp);
                fwrite(right, 1, xpad, fp);
            }
    } /* for y */

    //free(window);
}



/****************************************************************************
 * Func: scale_pnm                                                          *
 *                                                                          *
 * Desc: scale an image using nearest neighbor interpolation                *
 *                                                                          *
 * Params: buffer - pointer to image in memory                              *
 *         fileout - name of output file                                    *
 *         rows - number of rows in image                                   *
 *         cols - number of columns in image                                *
 *         x_scale - scale factor in X direction                            *
 *         y_scale - scale factor in Y direction                            *
 *         type - graphics file type (5 = PGM    6 = PPM)                   *
 ****************************************************************************/
void scale_pnm(image_ptr buffer, char* fileout, int rows, int cols,
    float x_scale, float y_scale, int type) //nearest neighbor 
{
    unsigned long x, y;          /* loop indices for columns and rows */
    unsigned long index;        /* index into line buffer */
    unsigned long source_index; /* address of source pixel in image buffer */
    unsigned char* line_buff;   /* output line buffer */
    int new_rows, new_cols;     /* values of rows and columns for new image */
    unsigned line;              /* number of pixels in one scan line */
    FILE* fp;                   /* output file pointer */
    unsigned long X_Source, Y_Source;  /* x and y address of source pixel */
    pixel_ptr color_buff;       /* pointer to a color image in memory */

    /* open new output file */
    if ((fp = fopen(fileout, "wb")) == NULL)
    {
        printf("Unable to open %s for output\n", fileout);
        exit(1);
    }

    new_cols = cols * x_scale;
    new_rows = rows * y_scale;

    /* print out the portable bitmap header */
    fprintf(fp, "P%d\n%d %d\n255\n", type, new_cols, new_rows);

    if (type == 5)        /* PGM file */
        line = new_cols;
    else                 /* PPM file */
    {
        line = new_cols * 3;
        color_buff = (pixel_ptr)buffer;
    }

    line_buff = (unsigned char*)malloc(line);

    for (y = 0; y < new_rows; y++)
    {
        index = 0;
        for (x = 0; x < new_cols; x++)
        {
            X_Source = (unsigned long)((x / x_scale) + 0.5);
            Y_Source = (unsigned long)((y / y_scale) + 0.5);
            source_index = Y_Source * cols + X_Source;
            if (type == 5)      /* PGM */
                line_buff[index++] = buffer[source_index];
            else               /* PPM */
            {
                line_buff[index++] = color_buff[source_index].r;
                line_buff[index++] = color_buff[source_index].g;
                line_buff[index++] = color_buff[source_index].b;
            }
        }
        fwrite(line_buff, 1, line, fp);
    }
    fclose(fp);
}


void bilinear(image_ptr buffer, char* fileout, int rows, int cols,
    float x_scale, float y_scale, int type)
{
    unsigned long x, y;          /* loop indices for columns and rows */
    unsigned long index;        /* index into line buffer */
    unsigned long source_index; /* address of source pixel in image buffer */
    unsigned char* line_buff;   /* output line buffer */
    int new_rows, new_cols;     /* values of rows and columns for new image */
    unsigned line;              /* number of pixels in one scan line */
    FILE* fp;                   /* output file pointer */
    unsigned long X_Source, Y_Source;  /* x and y address of source pixel */
    pixel_ptr color_buff;       /* pointer to a color image in memory */


    /*
    unsigned char NW, NE, SW, SE;
    float EWweight, NSweight;
    unsigned char EWtop, EWbottom;
    */

    /*
    float NW, NE, SW, SE;
    float EWweight, NSweight;
    float EWtop, EWbottom;
    */

    //이거
   /*
   unsigned long NW, NE, SW, SE;
   unsigned long EWweight, NSweight;
   unsigned long EWtop, EWbottom;
   */

   //unsigned long long NW, NE, SW, SE;
   //unsigned long long EWweight, NSweight;
   //unsigned long long EWtop, EWbottom;


   /*
   long NW, NE, SW, SE; //이거
   long EWweight, NSweight;
   long EWtop, EWbottom;
   */

    float NW, NE, SW, SE;
    float EWweight, NSweight;
    float EWtop, EWbottom;


    unsigned char new_index;

    //unsigned char EWweight, NSweight;
       //unsigned char EWtop, EWbottom;



       /* open new output file */
    if ((fp = fopen(fileout, "wb")) == NULL)
    {
        printf("Unable to open %s for output\n", fileout);
        exit(1);
    }

    new_cols = cols * x_scale;
    new_rows = rows * y_scale;

    /* print out the portable bitmap header */
    fprintf(fp, "P%d\n%d %d\n255\n", type, new_cols, new_rows);

    if (type == 5)        /* PGM file */
        line = new_cols;
    else                 /* PPM file */
    {
        line = new_cols * 3;
        color_buff = (pixel_ptr)buffer;
    }

    line_buff = (unsigned char*)malloc(line);

    for (y = 0; y < new_rows; y++)
    {
        index = 0;
        for (x = 0; x < new_cols; x++)
        {
            X_Source = (unsigned long)(x / x_scale);
            Y_Source = (unsigned long)(y / y_scale);
            source_index = Y_Source * cols + X_Source;
            if (type == 5)      /* PGM */
            {

                NW = buffer[Y_Source * cols + X_Source];
                NE = buffer[Y_Source * cols + X_Source + 1];
                SW = buffer[(Y_Source + 1) * cols + X_Source];
                SE = buffer[(Y_Source + 1) * cols + X_Source + 1];

                EWweight = (x / x_scale) - (float)X_Source;
                NSweight = (y / y_scale) - (float)Y_Source; //첫번째 보간

                EWtop = NW + EWweight * (NE - NW);
                EWbottom = SW + EWweight * (SE - SW); //두번째 보간

                new_index = (unsigned char)(EWtop + NSweight * (EWbottom - EWtop));
                //세번째 보간 

                line_buff[index++] = new_index;
            }
            else               /* PPM */
            {
                line_buff[index++] = color_buff[source_index].r;
                line_buff[index++] = color_buff[source_index].g;
                line_buff[index++] = color_buff[source_index].b;
            }
        }
        fwrite(line_buff, 1, line, fp);
    }
    fclose(fp);
}

void cubic(image_ptr buffer, char* fileout, int rows, int cols,
    float x_scale, float y_scale, int type) //cubic
{
    unsigned long x, y;          /* loop indices for columns and rows */
    unsigned long index;        /* index into line buffer */
    unsigned long source_index; /* address of source pixel in image buffer */
    unsigned char* line_buff;   /* output line buffer */
    int new_rows, new_cols;     /* values of rows and columns for new image */
    unsigned line;              /* number of pixels in one scan line */
    FILE* fp;                   /* output file pointer */
    unsigned long X_Source, Y_Source;  /* x and y address of source pixel */
    pixel_ptr color_buff;       /* pointer to a color image in memory */

    //--------------------------------
    float new_pixel_value; //보간된 값 

    float x_float, y_float;
    float x_diff, y_diff;

    int X_SourceMinusOne, X_SourcePlusOne, X_SourcePlusTwo;
    int Y_SourceMinusOne, Y_SourcePlusOne, Y_SourcePlusTwo;

    unsigned char image[4][4];

    /* open new output file */
    if ((fp = fopen(fileout, "wb")) == NULL)
    {
        printf("Unable to open %s for output\n", fileout);
        exit(1);
    }

    new_cols = cols * x_scale;
    new_rows = rows * y_scale;

    /* print out the portable bitmap header */
    fprintf(fp, "P%d\n%d %d\n255\n", type, new_cols, new_rows);

    if (type == 5)        /* PGM file */
        line = new_cols;
    else                 /* PPM file */
    {
        line = new_cols * 3;
        color_buff = (pixel_ptr)buffer;
    }

    line_buff = (unsigned char*)malloc(line);

    for (y = 0; y < new_rows; y++)
    {
        index = 0;
        for (x = 0; x < new_cols; x++)
        {
            X_Source = (unsigned long)(x / x_scale);
            Y_Source = (unsigned long)(y / y_scale);
            source_index = Y_Source * cols + X_Source;
            //--
            x_float = ((float)x / x_scale);
            y_float = ((float)y / y_scale);
            x_diff = x_float - X_Source; //x의 d 
            y_diff = y_float - Y_Source; //y의 d

            // buffer index
            X_SourceMinusOne = ((X_Source - 1) + abs(X_Source - 1)) / 2;   // for negative number, // when X_Source==0
            X_SourcePlusOne = X_Source + 1;
            X_SourcePlusTwo = X_Source + 2;

            Y_SourceMinusOne = ((Y_Source - 1) + abs(Y_Source - 1)) / 2;   // for negative number, // when Y_Source==0
            Y_SourcePlusOne = Y_Source + 1;
            Y_SourcePlusTwo = Y_Source + 2;

            if (X_Source == (unsigned)(cols - 2))
                X_SourcePlusTwo = X_Source + 1;         // when X_Source==cols-2

            if (X_Source == (unsigned)(cols - 1))
            {
                X_SourcePlusOne = X_Source;            // when X_Source==cols-1
                X_SourcePlusTwo = X_Source;
            }

            if (Y_Source == (unsigned)(rows - 2))
                Y_SourcePlusTwo = Y_Source + 1;         // when Y_Source==rows-2

            if (Y_Source == (unsigned)(rows - 1))
            {
                Y_SourcePlusOne = Y_Source;            // when Y_Source==rows-1
                Y_SourcePlusTwo = Y_Source;
            }
            image[0][0] = buffer[Y_SourceMinusOne * cols + X_SourceMinusOne];
            image[0][1] = buffer[Y_SourceMinusOne * cols + X_Source];
            image[0][2] = buffer[Y_SourceMinusOne * cols + X_SourcePlusOne];
            image[0][3] = buffer[Y_SourceMinusOne * cols + X_SourcePlusTwo];

            image[1][0] = buffer[Y_Source * cols + X_SourceMinusOne];
            image[1][1] = buffer[Y_Source * cols + X_Source];
            image[1][2] = buffer[Y_Source * cols + X_SourcePlusOne];
            image[1][3] = buffer[Y_Source * cols + X_SourcePlusTwo];

            image[2][0] = buffer[Y_SourcePlusOne * cols + X_SourceMinusOne];
            image[2][1] = buffer[Y_SourcePlusOne * cols + X_Source];
            image[2][2] = buffer[Y_SourcePlusOne * cols + X_SourcePlusOne];
            image[2][3] = buffer[Y_SourcePlusOne * cols + X_SourcePlusTwo];

            image[3][0] = buffer[Y_SourcePlusTwo * cols + X_SourceMinusOne];
            image[3][1] = buffer[Y_SourcePlusTwo * cols + X_Source];
            image[3][2] = buffer[Y_SourcePlusTwo * cols + X_SourcePlusOne];
            image[3][3] = buffer[Y_SourcePlusTwo * cols + X_SourcePlusTwo];


            // cubic_interpolation

            new_pixel_value = (float)cubic_interpolation(image, x_diff, y_diff, -1.0);


            if (type == 5)      /* PGM */
            {
                line_buff[index++] = (unsigned char)new_pixel_value;
            }
            else               /* PPM */
            {
                line_buff[index++] = color_buff[source_index].r;
                line_buff[index++] = color_buff[source_index].g;
                line_buff[index++] = color_buff[source_index].b;
            }
        }
        fwrite(line_buff, 1, line, fp);
    }
    fclose(fp);
}


/***************************************************************************
 * Func: cubic_interpolation                                               *
 *                                                                         *
 * Desc: takes 16 unsigned characters in an array and outputs the results  *
 *       of a cubic interpolation.  The sample points of the interpolation *
 *       function f(-1-d) and f(-d) are evaluated as f(1+d) and f(d).  The *
 *       computed constants are a0 = f(1+d)                                *
 *                              a1 = f(d)                                  *
 *                              a2 = f(1-d)                                *
 *                              a3 = f(2-d)                                *
 *                                                                         *
 * Params: image - array of 16 image sample points                         *
 *                 (0,0)           (0,3)                                   *
 *                   *---*---*---*---*                                     *
 *                   |   |   |   |   |                                     *
 *                   *---*---*---*---*                                     *
 *                   |   | o |   |   |    o is point to be interpolated    *
 *                   *---*---*---*---*                                     *
 *                   |   |   |   |   |                                     *
 *                   *---*---*---*---*                                     *
 *                 (3,0)           (3,3)                                   *
 *                                                                         *
 *         x_diff - x difference from interpolated address to image sample *
 *         y_diff - y difference from interpolated address to image sample *
 *         a - cubic spline interpolation function constant                *
 *                                                                         *
 * Returns: interpolated value                                             *
 ***************************************************************************/

 // 주변 픽셀 값에 가중치(가중치 함수 이용)를 곱한 값을 모두 더해 픽셀 값 계산 
 // v = v1 * f(1+d) + v2 * f(d) + v3 * f(1-d) + v4 * f(2-d)
 // 행(row)에 대해서 먼저 보간 -> 그 값을 이용해 열(column) 보간

unsigned char cubic_interpolation(unsigned char image[4][4],
    float x, float y, double a)
{
    double column[4];                /* storage for row interpolation results */
    double a0, a1, a2, a3;           /* interpolation coefficients */
    double x_plus_1, y_plus_1;       /* x + 1, y + 1 */
    double one_minus_x, one_minus_y; /* 1 - x, 1 - y */
    double two_minus_x, two_minus_y; /* 2 - x, 2 - y */
    int i;                           /* loop index */
    double pixel;                    /* newly interpolated value */

    /* Do we really need to interpolate ? */
    if ((x == 0.0) && (y == 0.0))
        return image[1][1];

    if (x == 0.0)                   /* Do we need to interpolate the rows? */
        for (i = 0; i < 4; i++)         /* NO */
            column[i] = (double)image[i][1];
    else                           /* YES, we do need to interpolate rows */
    {
        x_plus_1 = x + 1.0;
        one_minus_x = 1.0 - x;
        two_minus_x = 2.0 - x;

        a0 = ((a * x_plus_1 - 5.0 * a) * x_plus_1 + 8.0 * a) * x_plus_1 - 4.0 * a;
        a1 = ((a + 2.0) * x - (a + 3.0)) * x * x + 1;
        a2 = ((a + 2.0) * one_minus_x - (a + 3.0)) * one_minus_x * one_minus_x + 1;
        a3 = ((a * two_minus_x - 5.0 * a) * two_minus_x + 8.0 * a) * two_minus_x - 4.0 * a;

        if (y == 0.0)   /* can we just process one row */
        {
            pixel = a0 * image[1][0] + a1 * image[1][1] +
                a2 * image[1][2] + a3 * image[1][3];
            CLIP(pixel, 0.0, 255.0);
            return (unsigned char)pixel;
        }
        else
            for (i = 0; i < 4; i++)
                column[i] = a0 * image[i][0] + a1 * image[i][1] +
                a2 * image[i][2] + a3 * image[i][3];
    }

    /* interpolate the resulting column */

    y_plus_1 = y + 1.0;
    one_minus_y = 1.0 - y;
    two_minus_y = 2.0 - y;

    a0 = ((a * y_plus_1 - 5.0 * a) * y_plus_1 + 8.0 * a) * y_plus_1 - 4.0 * a;
    a1 = ((a + 2.0) * y - (a + 3.0)) * y * y + 1;
    a2 = ((a + 2.0) * one_minus_y - (a + 3.0)) * one_minus_y * one_minus_y + 1;
    a3 = ((a * two_minus_y - 5.0 * a) * two_minus_y + 8.0 * a) * two_minus_y - 4.0 * a;

    pixel = column[0] * a0 + column[1] * a1 + column[2] * a2 + column[3] * a3;
    CLIP(pixel, 0.0, 255.0);
    return (unsigned char)pixel;
}