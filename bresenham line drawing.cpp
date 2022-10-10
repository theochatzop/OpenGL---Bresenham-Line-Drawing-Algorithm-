#include <gl/glut.h>
#include <stdio.h>

// global variables
GLuint window, subWin;
bool translateUp = false, translateDown = false, translateRight = false, translateLeft = false;
bool zoomIn = false, zoomOut = false;
int h, w;
int xs, ys, xe, ye;
float color1 = 0.0f, color2 = 0.0f, color3 = 0.0f;
int transRL = 0, transUD = 0, moveRL = 0, moveUD = 0, fullScr = 0;
int point = 0;

void timer(int value)
{
    // changes line color every 16ms
    color1 += 0.001f;
    color2 += 0.003f;
    color3 += color2;
    if (color1 > 1.0)
        color1 = 0;
    if (color2 > 1.0)
        color2 = 0;
    if (color3 > 1.0)
        color3 = 0;

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void display()
{
    glClearColor(0.8, 0.4, 0.4, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    // gets window dimensions
    h = glutGet(GLUT_WINDOW_HEIGHT);
    w = glutGet(GLUT_WINDOW_WIDTH);
}

// initializes subwindow
void myInit()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 500, 0, 500);
}

// draws point
void setPixel(int x, int y)
{

    glPointSize(3);
    glColor3f(color1, color2, color3);
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// finds points on the line
void drawLine(int xs, int xe, int ys, int ye)
{
    int dx, dy, i, e;
    int addx, addy, inc1, inc2;
    int x, y;

    dx = xe - xs;
    dy = ye - ys;

    if (dx < 0)
        dx = -dx;
    if (dy < 0)
        dy = -dy;
    addx = 1;

    // octants 3,4,5,6
    if (xe < xs)
        addx = -1;

    // line parallel to the y-axis
    if (xs == xe)
        addx = 0;
    addy = 1;

    // octants 5,6,7,8
    if (ye < ys)
        addy = -1;

    // line parallel to the x-axis
    if (ys == ye)
        addy = 0;
    x = xs;
    y = ys;

    // driving axis is x
    if (dx > dy)
    {
        e = -(dx >> 1);
        inc1 = dy;
        inc2 = dx;
        // octants 1,8
        if (xe >= xs)
        {
            while (x <= xe)
            {
                setPixel(x, y);

                x += addx;
                e += inc1;
                if (e >= 0)
                {
                    e -= inc2;
                    y += addy;
                }
            }
        }
        // octants 4,5
        else if (xs > xe)
        {
            while (x >= xe)
            {
                setPixel(x, y);

                x += addx;
                e += inc1;

                if (e >= 0)
                {
                    e -= inc2;
                    y += addy;
                }
            }
        }
    }

    // driving axis is y
    else
    {
        e = -(dy >> 1);
        inc1 = dy;
        inc2 = dx;
        // octants 6,7
        if (ys >= ye)
        {
            while (y >= ye)
            {
                setPixel(x, y);
                e += inc2;
                y += addy;
                if (e >= 0)
                {
                    x += addx;
                    e -= inc1;
                }
            }
        }
        // octants 2,3
        else if (ys < ye)
        {
            while (y < ye)
            {
                setPixel(x, y);
                e += inc2;
                y += addy;
                if (e >= 0)
                {
                    x += addx;
                    e -= inc1;
                }
            }
        }
    }
}

// draws axes
void drawAxes()
{
    glPushAttrib(GL_CURRENT_BIT);
    glColor3f(0.4, 0.1, 0.3);
    glLineWidth(2);

    glBegin(GL_LINES);
    glVertex2f(250, 0);
    glVertex2f(250, 500);
    glVertex2f(0, 250);
    glVertex2f(500, 250);
    glEnd();
    glPopAttrib();
}

// gets start point and endpoint from user
void mouseClicked(int button, int state, int x, int y)
{

    if (state == GLUT_DOWN)
    {
        if (button == GLUT_LEFT_BUTTON)
        {

            if (point == 2)
            {
                point = 0;
            }
            point++;

            if (point == 1)
            {

                xs = x;
                ys = 500 - y;
            }

            if (point == 2)
            {

                xe = x;
                ye = 500 - y;
            }
        }
    }
}

// subwindow scene
void subDisplay()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    drawAxes();

    // draws line after getting user input
    if (point == 2)
    {
        drawLine(xs, xe, ys, ye);
    }

    // adjusts scene in fullscreen mode
    if (h > 600)
    {
        fullScr = 1;
        glutSetWindow(window);
        glClearColor(0.8, 0.4, 0.4, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
        glutSetWindow(subWin);
        glutReshapeWindow(h, 0.45 * w);
        glutPositionWindow(0.45 * h, 0.0625 * h);
    }

    // adjusts scene in non fullscreen mode
    if (h == 600 && fullScr)
    {
        glutReshapeWindow(500, 500);
        glutPositionWindow(50, 50);
    }

    //  translation
    if (translateUp == true)
    {
        glTranslatef(0, 25, 0);
        translateUp = false;
    }

    if (translateDown == true)
    {
        glTranslatef(0, -25, 0);
        translateDown = false;
    }

    if (translateRight == true)
    {
        glTranslatef(25, 0, 0);
        translateRight = false;
    }

    if (translateLeft == true)
    {
        glTranslatef(-25, 0, 0);
        translateLeft = false;
    }

    // scale
    if (zoomIn == true)
    {
        glScalef(1.1, 1.1, 0);
        zoomIn = false;
    }

    if (zoomOut == true)
    {
        glScalef(0.9, 0.9, 0);
        zoomOut = false;
    }

    // line translation
    glPushMatrix();
    glClear(GL_COLOR_BUFFER_BIT);
    glTranslatef(transRL, transUD, 0);
    if (point == 2)
    {
        drawLine(xs, xe, ys, ye);
    }
    glPopMatrix();
    drawAxes();

    glFlush();
}

// menu
void goMenu(int choice)
{

    switch (choice)
    {
    case 1:
        transUD += 25;
        break;
    case 2:
        translateUp = true;
        break;
    case 3:
        transUD -= 25;
        break;
    case 4:
        translateDown = true;
        break;
    case 5:
        transRL += 25;
        break;
    case 6:
        translateRight = true;
        break;
    case 7:
        transRL -= 25;
        break;
    case 8:
        translateLeft = true;
        break;
    case 9:
        zoomIn = true;
        break;
    case 10:
        zoomOut = true;
        break;
    case 11:
        exit(0);
        break;
    default:
        break;
    }
}

int main(int argc, char **argv)
{

    GLuint moveUp, moveDown, moveRight, moveLeft, mainMenu;

    glutInit(&argc, argv);                                  // initializes the GLUT library
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);            // sets initial display mode
    glutInitWindowSize(600, 600);                           // sets window size
    glutInitWindowPosition(0, 0);                           // sets window position
    window = glutCreateWindow("Bresenham's Line Drawing");  // creates window
    glutDisplayFunc(display);                               // sets the display callback
    subWin = glutCreateSubWindow(window, 50, 50, 500, 500); // creates subwindow
    myInit();
    glutDisplayFunc(subDisplay);

    // adds submenu to some items in main menu
    moveUp = glutCreateMenu(goMenu);
    glutAddMenuEntry("Line only", 1);
    glutAddMenuEntry("Both Line and Axis", 2);

    moveDown = glutCreateMenu(goMenu);
    glutAddMenuEntry("Line only", 3);
    glutAddMenuEntry("Both Line and Axis", 4);

    moveRight = glutCreateMenu(goMenu);
    glutAddMenuEntry("Line only", 5);
    glutAddMenuEntry("Both Line and Axis", 6);

    moveLeft = glutCreateMenu(goMenu);
    glutAddMenuEntry("Line only", 7);
    glutAddMenuEntry("Both Line and Axis", 8);

    // creates main menu
    mainMenu = glutCreateMenu(goMenu);
    glutAddSubMenu("Move Up", moveUp);
    glutAddSubMenu("Move Down", moveDown);
    glutAddSubMenu("Move Right", moveRight);
    glutAddSubMenu("Move Left", moveLeft);
    glutAddMenuEntry("Zoom In", 9);
    glutAddMenuEntry("Zoom out", 10);
    glutAddMenuEntry("Exit", 11);

    glutAttachMenu(GLUT_RIGHT_BUTTON); // displays menu after mouse right click
    glutTimerFunc(0, timer, 0);
    glutMouseFunc(mouseClicked);
    glutMainLoop(); // enters the GLUT event processing loop

    return 0;
}