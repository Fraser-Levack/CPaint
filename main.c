#include <stdio.h>
#include <GL/glut.h>
// maths for sqrt
#include <math.h>

int windowWidth;
int windowHeight;
int isDrawing = 0;
float points[4][10000][2]; // 4 Array to store points for each colour
int pointCount[4] = {0,0,0,0}; // 4 Array to store the number of points for each colour
float lastX = -1, lastY = -1;
float colours[4][3] = {{1.0, 1.0, 1.0},{1.0, 0.0, 0.0},{0.0, 1.0, 0.0},{0.0, 0.0, 1.0}};
int colour = 0;

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    for (int n = 0; n < 4; n++)
    {
        glColor3f(colours[n][0],colours[n][1],colours[n][2]); //
        glPointSize(5.0);
        glBegin(GL_POINTS);
        for (int i = 0; i < pointCount[n]; i++) {
            glVertex2f(points[n][i][0], points[n][i][1]);
        }
        glEnd();
    }
    glFlush(); // Ensure all commands are executed
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            isDrawing = 1;
            lastX = (x - windowWidth / 2) / (windowWidth / 2.0);
            lastY = (windowHeight / 2 - y) / (windowHeight / 2.0);
        } else if (state == GLUT_UP) {
            isDrawing = 0;
            lastX = -1;
            lastY = -1;
        }
    }
}

void motion(int x, int y) {
    if (isDrawing) {
        float fx = (x - windowWidth / 2) / (windowWidth / 2.0);
        float fy = (windowHeight / 2 - y) / (windowHeight / 2.0);

        if (lastX != -1 && lastY != -1) {
            float dx = fx - lastX;
            float dy = fy - lastY;
            float distance = sqrt(dx * dx + dy * dy);
            int steps = (int)(distance * 200); // Number of intermediate points

            for (int i = 0; i <= steps; i++) {
                float t = (float)i / (float)steps;
                points[colour][pointCount[colour]][0] = lastX + t * dx;
                points[colour][pointCount[colour]][1] = lastY + t * dy;
                pointCount[colour]++;
            }
        }

        lastX = fx;
        lastY = fy;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) { // ASCII code for the ESC key
        for (int n = 0; n < 4; n++) {
            pointCount[n] = 0;
        }
        glutPostRedisplay();
    }
    else if (key == 119)
    {
        colour = 0;
    }
    else if (key == 114)
    {
        colour = 1;
    }
    else if (key == 103)
    {
        colour = 2;
    }
    else if (key == 98)
    {
        colour = 3;
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