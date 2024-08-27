//
// Created by Fraser on 24/08/2024.
//
#include <stdlib.h>
#include "array_utils.h"

//#include <stdio.h>

// Function to free a 2D array
void free2DArray(float **array, const int rows) {
    for (int i = 0; i < rows; i++) {
        free(array[i]);
    }
    free(array);
    // printf("freed 2D array\n");
}

// Function to free a 3D array
void free3DArray(float ***array, const int rows, const int cols) {
    for (int i = 0; i < rows; i++) {
        free2DArray(array[i], cols);
    }
    free(array);
    // printf("freed 3D array\n");
}

// Function to free a 3D array of points OUT OF DATE - DO NOT USE
void freePointsArray(float ***points, const int numColors, const int *pointCounts) {
    for (int i = 0; i < numColors; i++) {
        for (int j = 0; j < pointCounts[i]; j++) {
            free(points[i][j]);
        }
        free(points[i]);
    }
    free(points);
}