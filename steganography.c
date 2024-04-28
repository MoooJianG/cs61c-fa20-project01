/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
		if (image == NULL) {
				printf("Invalid input!");
				return NULL;
		}

		Color * color = (Color *)malloc(sizeof(Color));
		if (color == NULL) {
				printf("Memory allocate failed!");
				return NULL;
		}
		if (image->image[row][col].B % 2 == 1) {
				color->R = 255;
				color->B = 255;
				color->G = 255;
		}
		else {
				color->R = 0;
				color->G = 0;
				color->B = 0;
		}
		return color;
	//YOUR CODE HERE
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
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
						Color * pcol = evaluateOnePixel(image, i, j);
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
        //YOUR CODE HERE
}

/*
Loads a file of ppm P3 format from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a file of ppm P3 format (not necessarily with .ppm file extension).
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
		//printf("%d\n", argc);
		printf("%s %s\n", argv[0], argv[1]);
		if (argc != 2) {
				printf("Usage: %s <filename>\n", argv[0]);
				return -1;
		}

		Image *ima = readData(argv[1]);
		if (ima == NULL) {
				printf("Failed to load image file: %s\n", argv[1]);
				return -1;
		}

		Image *processImage = steganography(ima);
		if (processImage == NULL) {
				printf("Failed to perform steganography!\n");
				freeImage(ima);
				return -1;
		}

		writeData(processImage);

		freeImage(processImage);
		freeImage(ima);
		return 0;
  // YOUR CODE HERE
}
