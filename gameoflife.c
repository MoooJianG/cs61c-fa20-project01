/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
		if (image == NULL) {
				printf("Failed to open image!");
				return NULL;
		}

		int imageRow = image->rows;
		int imageCol = image->cols;

		int rowAbove = (row - 1 + imageRow) % imageRow;
		int rowBelow = (row + 1) % imageRow;
		int colRight = (col + 1) % imageCol;
		int colLeft = (col - 1 + imageCol) % imageCol;

		Color colorAbove = image->image[rowAbove][col];
		Color colorBelow = image->image[rowBelow][col];
		Color colorRight = image->image[row][colRight];
		Color colorLeft = image->image[row][colLeft];
		Color colorUpperRight = image->image[rowAbove][colRight];
		Color colorUpperLeft = image->image[rowAbove][colLeft];
		Color colorLowerRight = image->image[rowBelow][colRight];
		Color colorLowerLeft = image->image[rowBelow][colLeft];
    int numAliveNeighbors = (colorAbove.B + colorBelow.B + colorLeft.B + colorRight.B + colorUpperLeft.B + colorUpperRight.B + colorLowerLeft.B + colorLowerRight.B) / 255;
		Color *color = (Color *)malloc(sizeof(Color));
		
		int flag = (image->image[row][col].B > 0) ? 1 : 0;
		int ff = rule >> (flag * 9 + numAliveNeighbors) & 1;
		printf("rule:%d num:%d ff= %d\n", rule, numAliveNeighbors, ff);
		if (ff) {
				color->B = 255;
				color->R = 255;
				color->G = 255;
		}
		else {
				color->R = 0;
				color->G = 0;
				color->B = 0;
		}
		return color;
                // YOUR CODE HERE
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
		if (image == NULL) {
				printf("Invalid input!");
				return NULL;
		}

		Image *ima = (Image *)malloc(sizeof(Image));
		if (ima == NULL) {
				printf("Memory allocate failed!1");
				return NULL;
		}

		ima->cols = image->cols;
		ima->rows = image->rows;

		ima->image = (Color **)malloc(sizeof(Color *) * ima->rows);
		if (ima->image == NULL) {
				printf("Memory allocate failed!2");
				free(ima);
				return NULL;
		}

		for (int i = 0; i < ima->rows; i++) {
				ima->image[i] = (Color *)malloc(sizeof(Color) * ima->cols);
				if (ima->image[i] == NULL) {
						printf("Memory allocate failed!3");
						for (int j = 0; j < i; j++) {
								free(ima->image[j]);
						}
						free(ima->image);
						free(ima);
						return NULL;
				}
		}

		for (int i = 0; i < ima->rows; i++) {
				for (int j = 0; j < ima->cols; j++) {
						Color * pcol = evaluateOneCell(image, i, j, rule);
						if (pcol == NULL) {
								for (int k = 0; k < ima->rows; k++) {
										free(ima->image[k]);
								}
								free(ima->image);
								free(ima);
						}
						ima->image[i][j] = *pcol;
						free(pcol);
				}
		}
		return ima;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
  // YOUR CODE HERE
  // printf("%d\n", argc);
	//	printf("%s %s\n", argv[0], argv[1]);
		if (argc != 3) {
				printf("usage: ./gameOfLife filename rule \n filename is an ASCII PPM file (type P3) with maximum value 255.\n rule is a hex number beginning with 0x; Life is 0x1808.\n");
				return -1;
		}

		Image *ima = readData(argv[1]);
		if (ima == NULL) {
				printf("Failed to load image file: %s\n", argv[1]);
				return -1;
		}
		char * endstr;
		uint32_t rule = strtol(argv[2], &endstr, 0);
		printf("argv %s rule %d\n", argv[2], rule);
		Image *processImage = life(ima, rule);
		if (processImage == NULL) {
				printf("Failed to perform steganography!\n");
				freeImage(ima);
				return -1;
		}

		writeData(processImage);
 
		freeImage(processImage);
		freeImage(ima);
		return 0;
}
