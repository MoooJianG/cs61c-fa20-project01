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
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("Opening file failed!");
        return NULL;
    }

    char magic[3];
    int max_color;
    if (fscanf(fp, "%2s\n", magic) != 1 || strcmp(magic, "P3") != 0)
    {
        printf("Invalid PPM file format!");
        fclose(fp);
        return NULL;
    }

    int rows, cols;

    if (fscanf(fp, "%u %u\n", &cols, &rows) != 2)
    {
        printf("Invalid image dimensions!");
        fclose(fp);
        return NULL;
    }

    if (fscanf(fp, "%d\n", &max_color) != 1 || max_color != 255)
    {
        printf("Invalid max color value!");
        fclose(fp);
        return NULL;
    }

    Image *ima = (Image *)malloc(sizeof(Image));
    if (ima == NULL)
    {
        printf("Memory allocation failed!");
        fclose(fp);
        return NULL;
    }

    ima->cols = cols;
    ima->rows = rows;
    ima->image = (Color **)malloc(sizeof(Color *) * rows);
    if (ima->image == NULL)
    {
        printf("Memory allocation failed!");
        fclose(fp);
        free(ima);
        return NULL;
    }

    for (int i = 0; i < rows; i++)
    {
        ima->image[i] = (Color *)malloc(sizeof(Color) * cols);
        if (ima->image[i] == NULL)
        {
            printf("Memory allocation failed!");
            fclose(fp);
            for (int j = 0; j < i; j++)
            {
                free(ima->image[j]);
            }
            free(ima->image);
            free(ima);
            return NULL;
        }
    }

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (fscanf(fp, "%hhu %hhu %hhu", &ima->image[i][j].R, &ima->image[i][j].G, &ima->image[i][j].B) != 3)
            {
                printf("Invalid color values!");
                fclose(fp);
                freeImage(ima);
                return NULL;
            }
        }
    }

    fclose(fp);
    return ima;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image) {
		if (image == NULL) {
				printf("Invalid input");
				return;
		}
		printf("P3\n");
		
		printf("%u %u\n", image->cols, image->rows);
		printf("255\n");
		for (int i = 0; i < image->rows; i++) {
				for (int j = 0; j < image->cols; j++) {
						printf("%3d %3d %3d", image->image[i][j].R, image->image[i][j].G,
									 image->image[i][j].B);
						if (j < image->cols - 1) {
								printf("   ");
						}
				}
				printf("\n");
		}
}
//Frees an image
void freeImage(Image *image){
		if (image == NULL) {
			printf("Invalid input");
			return;
		}
		for (int i = 0; i < image->rows; i++) {
				free(image->image[i]);
		}
		free(image->image);
		free(image);
  // YOUR CODE HERE
}
