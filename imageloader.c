/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
		FILE *fp = fopen(filename, "b");
		if (fp == NULL) {
				printf("Opening file failed!");
				fclose(fp);
				return NULL;
		}

		char magic[3];
		int max_color;
		fscanf(fp, "%2s\n", magic);
		if (strcmp(magic, "P3") != 0) {
				printf("Invalid PPM file format!");
				fclose(fp);
				return NULL;
		}

		int rows, cols;

		fscanf(fp, "%u %u\n", &cols, &rows);
		fscanf(fp, "%d\n", &max_color);

		Image *ima = (Image *) malloc (sizeof(Image));
		if (ima == NULL) {
				printf("Memory allocation failed!");
				fclose(fp);
				return NULL;
		}

		ima->cols = cols;
		ima->rows = rows;
		ima->image = (Color **) malloc (sizeof(Color*) * rows);
		if (ima->image == NULL) {
				printf("Memory allocation failed!");
				fclose(fp);
				free(ima);
				return NULL;
		}

		for (int i = 0; i < rows; i++) {
				ima->image[i] = (Color *) malloc (sizeof(Color) * cols);
				if (ima->image[i] == NULL) {
						printf("Memory allocation failed!");
						fclose(fp);
						free(ima);
						free(ima->image);
						return NULL;
				}
		}

		for (int i = 0; i < rows; i++) {
				for (int j = 0; j < cols; j++) {
						fscanf(fp, "%hhu %hhu %hhu", &ima->image[i][j].R, &ima->image[i][j].G, &ima->image[i][j].B);
				}
		}

		fclose(fp);
		return ima;

		//YOUR CODE HERE
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
}
