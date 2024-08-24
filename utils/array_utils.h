//
// Created by Fraser on 24/08/2024.
//

#ifndef ARRAY_UTILS_H
#define ARRAY_UTILS_H


void free2DArray(float **array, int rows);
void free3DArray(float ***array, int rows, int cols);
void freePointsArray(float ***points, int numColors, const int *pointCounts);

#endif //ARRAY_UTILS_H
