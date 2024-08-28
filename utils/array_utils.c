//
// Created by Fraser on 24/08/2024.
//
#include <stdlib.h>
#include <math.h>
#include "array_utils.h"

#include <stdio.h>

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

float distance(const float *point1, const float *point2) {
    return sqrt(pow(point2[0] - point1[0], 2) + pow(point2[1] - point1[1], 2));
}

// Function to interpolate between points
float **interpolate(const float **points, const int numPoints, const float threshold, int *newNumPoints) {

    // Calculate the number of new points needed
    int count = 0;
    for (int i = 0; i < numPoints - 1; i++) {
        const float dist = distance(points[i], points[i + 1]);
        count += (int)(dist / threshold) + 1;
    }
    count++; // For the last point

    // Allocate memory for the new array of points
    float **newPoints = malloc(count * sizeof(float *));
    for (int i = 0; i < count; i++) {
        newPoints[i] = malloc(2 * sizeof(float));
    }

    // Interpolate points
    int index = 0;
    for (int i = 0; i < numPoints - 1; i++) {
        const float dist = distance(points[i], points[i + 1]);
        const int numInterpolations = (int)(dist / threshold);
        const float stepX = (points[i + 1][0] - points[i][0]) / (numInterpolations + 1);
        const float stepY = (points[i + 1][1] - points[i][1]) / (numInterpolations + 1);

        newPoints[index][0] = points[i][0];
        newPoints[index][1] = points[i][1];
        index++;

        for (int j = 1; j <= numInterpolations; j++) {
            newPoints[index][0] = points[i][0] + j * stepX;
            newPoints[index][1] = points[i][1] + j * stepY;
            index++;
        }
    }

    // Add the last point
    newPoints[index][0] = points[numPoints - 1][0];
    newPoints[index][1] = points[numPoints - 1][1];

    *newNumPoints = count;
    return newPoints;
}