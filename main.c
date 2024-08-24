//
// Created by fraser on 24/08/2024.
//
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>

int windowWidth;
int windowHeight;
int isDrawing = 0;
int colour = 0;

// array struct describes list of points for each colour
float white[10000][2];
float red[10000][2];
float green[10000][2];
float blue[10000][2];

// an array containing the 4 pointers to arrays of point colours
float (*points[4])[2] = {white, red, green, blue};

// block struct describes list of actions preformed being what colour and how many points added
struct block {
    int colour;
    int pointCount;
};

// block array stores the actions preformed
struct block actionBlocks[20000];
int blockCount = 0;
int currentBlockPointCount = 0;
int pointCount[4] = {0,0,0,0}; // 4 Array to store the number of points for each colour
float lastX = -1, lastY = -1;
float colours[4][3] = {{1.0, 1.0, 1.0},{1.0, 0.0, 0.0},{0.0, 1.0, 0.0},{0.0, 0.0, 1.0}};

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    // set the colour to the colour of the block
    // set the pointCount to the pointCount of the block

    int positionInEachColour[4] = {0,0,0,0}; // used to store current position in each colour
    for (int n = 0; n < blockCount; n++) {

        colour = actionBlocks[n].colour;// set the colour to the colour of the block
        const int blockpointCount = actionBlocks[n].pointCount;// set the pointCount to the pointCount of the block
        positionInEachColour[colour] += blockpointCount;// add the pointCount to the positionInEachColour array

        glColor3f(colours[colour][0],colours[colour][1],colours[colour][2]);
        glPointSize(5.0);

        // loop through the corresponding array of points in the colour

        glBegin(GL_POINTS);
        // from current position in the colour to the pointCount of the block
        for (int i = positionInEachColour[colour]; i < positionInEachColour[colour] + blockpointCount; i++)
        {
            glVertex2f(points[colour][i][0], points[colour][i][1]);

        }
        glEnd();
    }
    glFlush(); // Ensure all commands are executed

    for (int i = 0; i < 4; i++) {
        positionInEachColour[i] = 0; // reset the positionInEachColour array for each
    }
}

void addActionBlock (const int colour, const int pointCount) {
    actionBlocks[blockCount].colour = colour;
    actionBlocks[blockCount].pointCount = pointCount;
    blockCount++;
    //printf("Block %d: Colour %d, Points %d\n", blockCount, colour, pointCount);
    currentBlockPointCount = 0;
}

void mouse(const int button,const int state,const int x,const int y) {
    if (button == GLUT_LEFT_BUTTON) {

        if (currentBlockPointCount == 100) {
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
        // clear all the points
        for (int i = 0; i < 4; i++) {
            pointCount[i] = 0;
        }
        blockCount = 0;
        currentBlockPointCount = 0;
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
    glutMainLoop();
    return 0;
}

