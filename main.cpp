#include <windows.h>
#include <GL/glut.h>
#include <stdio.h>

typedef int Outcode;

const int INSIDE=0;
const int LEFT=1;
const int RIGHT=2;
const int BOTTOM=4;
const int TOP=8;

float xmin, ymin, xmax, ymax;
int x0, y0, x1, y1;

void init()
{
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,500,0,500);
    glFlush();
}

Outcode computecode( double x, double y)
{
  Outcode code = INSIDE;

  if(x < xmin)
    code |= LEFT;
  else if(x > xmax)
    code |= RIGHT;
  else if(y < ymin)
    code |= BOTTOM;
  else if(y > ymax)
    code |= TOP;

  return code;
}

void csla(double x0, double y0, double x1, double y1)
{
    bool accept = false;
    Outcode outcode0 = computecode(x0, y0);
    Outcode outcode1 = computecode(x1, y1);
    double x,y;

    while(true) {
        if (!(outcode0 | outcode1)) {
            accept = true;
            break;
        } else if (outcode0 & outcode1)  {
            break;
        } else {
            Outcode outcodeout = outcode0 ? outcode0 : outcode1;

            if (outcodeout & TOP) {
                x = x0 + (x1 - x0)*(ymax - y0)/(y1 - y0);
                y = ymax;
            } else if (outcodeout & BOTTOM) {
                x = x0 + (x1 - x0)*(ymin - y0)/(y1 - y0);
                y = ymin;
            } else if (outcodeout & RIGHT) {
                y = y0 + (y1 - y0)*(xmax - x0)/(x1 - x0);
                x = xmax;
            } else if (outcodeout & LEFT) {
                y = y0 + (y1 - y0)*(xmin - x0)/(x1 - x0);
                x = xmin;
            }

            if (outcodeout == outcode0) {
                x0 = x;
                y0 = y;
                outcode0 = computecode(x0, y0);
            } else {
                x1 = x;
                y1 = y;
                outcode1 = computecode(x1, y1);
            }
        }
    }

    if (accept) {
        glColor3f(1,0,0);
        glBegin(GL_LINE_LOOP);
        glVertex2f(4*xmin, 4*ymin);
        glVertex2f(4*xmax, 4*ymin);
        glVertex2f(4*xmax, 4*ymax);
        glVertex2f(4*xmin, 4*ymax);
        glEnd();

        glBegin(GL_LINES);
        glVertex2f(4*x0, 4*y0);
        glVertex2f(4*x1, 4*y1);
        glEnd();
    }
}

void drawline()  {
    glColor3f(1,0,1);
    glBegin(GL_LINES);
    glVertex2d(x0, y0);
    glVertex2d(x1, y1);
    glEnd();
}

void drawrect() {
    glColor3f(0,0,1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(xmin, ymin);
    glVertex2f(xmax, ymin);
    glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glEnd();
}

void display()  {
    drawline();
    drawrect();
    csla(x0, y0,  x1, y1);
    glFlush();
}

int main(int argc, char* argv[])
{
    printf("Enter the window coordinates:");
    scanf("%f%f%f%f", &xmin, &ymin, &xmax, &ymax);
    printf("Enter  End Points: ");
    scanf("%d%d%d%d", &x0, &y0, &x1, &y1);

    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutCreateWindow("COHEN-SUTHERLAND");
    init();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
