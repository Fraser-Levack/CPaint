//
// Created by fraser on 24/08/2024.
//
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <windows.h>
#include "utils/array_utils.h"

int windowWidth;
int windowHeight;
int isDrawing = 0;
int colour = 0;

// block struct describes list of actions preformed being what colour and how many points added
struct block {
    int colour;
    int pointCount;
};

// block array stores the actions preformed
struct block actionBlocks[40000];
int blockCount = 0;
int currentBlockPointCount = 0;
int pointCount[4] = {0,0,0,0}; // 4 Array to store the number of points for each colour
int positionInEachColour[4] = {0,0,0,0}; // used to store current position in each colour
float lastX = -1, lastY = -1;
float colours[4][3] = {{1.0, 1.0, 1.0},{1.0, 0.0, 0.0},{0.0, 1.0, 0.0},{0.0, 0.0, 1.0}};

// Dynamically allocate the points array
float ***points;

void initializePointsArray(const int numColors,const int *pointCounts) {
    points = malloc(numColors * sizeof(float **));
    for (int i = 0; i < numColors; i++) {
        points[i] = malloc(pointCounts[i] * sizeof(float *));
        for (int j = 0; j < pointCounts[i]; j++) {
            points[i][j] = malloc(2 * sizeof(float));
        }
    }
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    int positionInEachColour[4] = {0, 0, 0, 0}; // Reset at the beginning of display

    for (int n = 0; n < blockCount; n++) {

        colour = actionBlocks[n].colour;
        const int blockpointCount = actionBlocks[n].pointCount;
        positionInEachColour[colour] += blockpointCount; // Update position before drawing the next block

        glColor3f(colours[colour][0], colours[colour][1], colours[colour][2]);
        glPointSize(5.0);

        glBegin(GL_POINTS);
        for (int i = positionInEachColour[colour]; i < positionInEachColour[colour] + blockpointCount - 1; i++) {
            glVertex2f(points[colour][i][0], points[colour][i][1]);
        }

        glEnd();
        //printf("position in each colour: %d, %d, %d, %d\n", positionInEachColour[0], positionInEachColour[1], positionInEachColour[2], positionInEachColour[3]);

    }
    glFlush();

}

void addActionBlock (const int colour, const int pointCount) {
    actionBlocks[blockCount].colour = colour;
    actionBlocks[blockCount].pointCount = pointCount - 1;
    blockCount++;
    //printf("Block %d: Colour %d, Points %d\n", blockCount, colour, pointCount);
    currentBlockPointCount = 0;
}

void mouse(const int button,const int state,const int x,const int y) {
    if (button == GLUT_LEFT_BUTTON) {

        if (currentBlockPointCount > 80) {
            addActionBlock(colour, currentBlockPointCount);
        }

        const float fx = (x - windowWidth / 2) / (windowWidth / 2.0);
        const float fy = (windowHeight / 2 - y) / (windowHeight / 2.0);
        if (state == GLUT_DOWN) {
            isDrawing = 1;
            lastX = fx;
            lastY = fy;
        } else if (state == GLUT_UP) {
            isDrawing = 0;
            points[colour][pointCount[colour]][0] = fx;
            points[colour][pointCount[colour]][1] = fy;
            pointCount[colour]++;
            currentBlockPointCount++;
            lastX = -1;
            lastY = -1;
            glutPostRedisplay();
        }
    }
}

void motion(const int x, const int y) {
    if (isDrawing)
    {
        const float fx = (x - windowWidth / 2) / (windowWidth / 2.0);
        const float fy = (windowHeight / 2 - y) / (windowHeight / 2.0);


        if (lastX != -1 && lastY != -1) {
            const float dx = fx - lastX;
            const float dy = fy - lastY;
            const float distance = sqrt(dx * dx + dy * dy);

            const float threshold = 0.008; // Threshold for the distance between points

            if (distance > threshold) {
                const int steps = (int)(distance * 250); // Number of intermediate points

                for (int i = 0; i <= steps; i++) {
                    const float t = (float)i / (float)steps;
                    points[colour][pointCount[colour]][0] = lastX + t * dx;
                    points[colour][pointCount[colour]][1] = lastY + t * dy;
                    pointCount[colour]++;
                    currentBlockPointCount++;
                }
            } else {
                points[colour][pointCount[colour]][0] = fx;
                points[colour][pointCount[colour]][1] = fy;
                pointCount[colour]++;
                currentBlockPointCount++;
            }
        }
        if (currentBlockPointCount > 80){
            addActionBlock(colour, currentBlockPointCount);
        }
        lastX = fx;
        lastY = fy;
        glutPostRedisplay();
    }
}


void keyboard(const unsigned char key, const int x, const int y) {
    if (key == 27) { // ASCII code for the ESC key
        // free the memory for points
        printf("freeing points array\n");
        freePointsArray(points, 4, pointCount);

        // clear all the points
        for (int i = 0; i < 4; i++) {
            pointCount[i] = 0;
        }
        blockCount = 0;
        currentBlockPointCount = 0;

        // Reinitialize the points array
        const int pointCounts[4] = {40000, 40000, 40000, 40000};
        initializePointsArray(4, pointCounts);

        glutPostRedisplay();
    }
    // make a new block no matter what if changing colour
    else if (key == 119) {
        colour = 0;
        addActionBlock(colour, currentBlockPointCount);
    }
    else if (key == 114) {
        colour = 1;
        addActionBlock(colour, currentBlockPointCount);
    }
    else if (key == 103) {
        colour = 2;
        addActionBlock(colour, currentBlockPointCount);
    }
    else if (key == 98) {
        colour = 3;
        addActionBlock(colour, currentBlockPointCount);
    }
    // undo step using ctrl + z keys
    else if (key == 26) {
        if (blockCount > 0) {

            pointCount[actionBlocks[blockCount].colour] -= actionBlocks[blockCount].pointCount;
            blockCount--;
            currentBlockPointCount = actionBlocks[blockCount].pointCount;

            glutPostRedisplay();
            Sleep(500); // Pause for 500 milliseconds to give the user time to release the key
        }
    }

}


int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    windowWidth = glutGet(GLUT_SCREEN_WIDTH);
    windowHeight = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("CPaint");
    glutDisplayFunc(display);
    glutMouseFunc(mouse); // Register the mouse callback function
    glutMotionFunc(motion); // Register the motion callback function
    glutKeyboardFunc(keyboard); // Register the keyboard callback function

    int pointCounts[4] = {40000, 40000, 40000, 40000};
    initializePointsArray(4, pointCounts);

    glutMainLoop();

    // Free the points array before exiting
    freePointsArray(points, 4, pointCounts);
    return 0;
}

