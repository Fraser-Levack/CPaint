#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#include <windows.h>
#include "utils/array_utils.h"

int windowWidth;
int windowHeight;
int isDrawing = 0;
int colour = 0; // Initialize to a valid value

struct block {
    int colour;
    int pointCount;
    float **points;
    struct block *nextBlock;
};

struct block *headBlock = NULL;
struct block *currentBlock = NULL;

float colours[4][3] = {{1.0, 1.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};

void addBlock(const int colour, const int pointCount, const float **points, struct block *currentBlock) {
    struct block *newBlock = malloc(sizeof(struct block));
    newBlock->colour = colour;
    newBlock->pointCount = pointCount;
    newBlock->points = malloc(pointCount * sizeof(float *));
    for (int i = 0; i < pointCount; i++) {
        newBlock->points[i] = malloc(2 * sizeof(float));
        newBlock->points[i][0] = points[i][0];
        newBlock->points[i][1] = points[i][1];
    }
    newBlock->nextBlock = NULL;
    if (currentBlock != NULL) {
        currentBlock->nextBlock = newBlock;
    } else {
        headBlock = newBlock;
    }
}

void removeMostRecentBlock() {
    if (headBlock == NULL) {
        return; // No blocks to remove
    }

    if (headBlock->nextBlock == NULL) {
        // Only one block in the list
        free2DArray(headBlock->points, headBlock->pointCount);
        free(headBlock);
        headBlock = NULL;
        currentBlock = NULL;
    } else {
        // More than one block in the list
        struct block *tempBlock = headBlock;
        while (tempBlock->nextBlock->nextBlock != NULL) {
            tempBlock = tempBlock->nextBlock;
        }
        free2DArray(tempBlock->nextBlock->points, tempBlock->nextBlock->pointCount);
        free(tempBlock->nextBlock);
        tempBlock->nextBlock = NULL;
        currentBlock = tempBlock;
    }
}

void freeAllBlocks(struct block *headBlock) {
    struct block *tempBlock = headBlock;
    while (tempBlock != NULL) {
        struct block *nextBlock = tempBlock->nextBlock;
        free2DArray(tempBlock->points, tempBlock->pointCount);
        free(tempBlock);
        tempBlock = nextBlock;
    }
    headBlock = NULL;
    currentBlock = NULL;
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    struct block *tempBlock = headBlock;
    while (tempBlock != NULL) {
        int blockColour = tempBlock->colour;
        if (blockColour < 0 || blockColour > 3) {
            blockColour = 0; // Default to white if out of bounds
        }
        glColor3f(colours[blockColour][0], colours[blockColour][1], colours[blockColour][2]);
        const int blockpointCount = tempBlock->pointCount;

        glPointSize(5.0);

        glBegin(GL_POINTS);
        for (int i = 0; i < blockpointCount; i++) {
            glVertex2f(tempBlock->points[i][0], tempBlock->points[i][1]);
        }
        glEnd();
        tempBlock = tempBlock->nextBlock;
    }
    glFlush();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (isDrawing == 0) {
            isDrawing = 1;
            float **points = malloc(1 * sizeof(float *));
            points[0] = malloc(2 * sizeof(float));
            points[0][0] = (float)x / windowWidth;
            points[0][1] = 1 - (float)y / windowHeight;
            addBlock(colour, 1, (const float **)points, currentBlock);
            if (currentBlock == NULL) {
                currentBlock = headBlock;
            } else {
                currentBlock = currentBlock->nextBlock;
            }
        } else {
            currentBlock->pointCount++;
            currentBlock->points = realloc(currentBlock->points, currentBlock->pointCount * sizeof(float *));
            currentBlock->points[currentBlock->pointCount - 1] = malloc(2 * sizeof(float));
            currentBlock->points[currentBlock->pointCount - 1][0] = (float)x / windowWidth;
            currentBlock->points[currentBlock->pointCount - 1][1] = 1 - (float)y / windowHeight;
        }
    } else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        isDrawing = 0;
    }
    glutPostRedisplay();
}

void motion(int x, int y) {
    if (isDrawing) {
        currentBlock->pointCount++;
        currentBlock->points = realloc(currentBlock->points, currentBlock->pointCount * sizeof(float *));
        currentBlock->points[currentBlock->pointCount - 1] = malloc(2 * sizeof(float));
        currentBlock->points[currentBlock->pointCount - 1][0] = (float)x / windowWidth;
        currentBlock->points[currentBlock->pointCount - 1][1] = 1 - (float)y / windowHeight;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ESC key
        freeAllBlocks(headBlock);
        headBlock = malloc(sizeof(struct block));
        headBlock->nextBlock = NULL;
        currentBlock = headBlock;
        glutPostRedisplay();
    } else if (key == 119) {
        colour = 0;
    } else if (key == 114) {
        colour = 1;
    } else if (key == 103) {
        colour = 2;
    } else if (key == 98) { // w, r, g, b
        colour = 3;
    } else if (key == 26) { // CTRL + Z
        if (headBlock->nextBlock != NULL) {
            removeMostRecentBlock(currentBlock);
        }
        glutPostRedisplay();
        Sleep(100);
    }
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    windowWidth = glutGet(GLUT_SCREEN_WIDTH);
    windowHeight = glutGet(GLUT_SCREEN_HEIGHT);

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("Cpaint");

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    headBlock = malloc(sizeof(struct block));
    headBlock->nextBlock = NULL;
    currentBlock = headBlock;

    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}