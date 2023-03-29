/*
     start bezier + mouse
*/
#include <GL/freeglut.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>


struct point {
    double x;    double y;
};
struct POINT3D {
    double x;    double y;    double z;
};
struct POINT3D  cub[512];

struct PLAN {
    POINT3D octagon[8];
};

PLAN planuri[6];

// simple linear interpolation between two points
void lerp(point& dest, const point& a, const point& b, const float t)
{
    dest.x = a.x + (b.x-a.x)*t;
    dest.y = a.y + (b.y-a.y)*t;
}
// evaluate a point on a bezier-curve. t goes from 0 to 1.0
void bezier(point &dest, const point& a, const point& b, const point& c, const point& d, const float t)
{
    point ab,bc,cd,abbc,bccd;
    lerp(ab, a,b,t);
    lerp(bc, b,c,t);
    lerp(cd, c,d,t);
    lerp(abbc, ab,bc,t);
    lerp(bccd, bc,cd,t);
    lerp(dest, abbc,bccd,t);
}


//////////////////////////////////////////////
void genereaza() {
    double a=2*3.1415/8, sina, cosa;

    // punctele determinate de curba Bezier
    point A = { 0, 4 };
    point B = { 1.4, 0 };
    point C = { 0, 1.4 };
    point D = { 4, 0};

    int n=-1;  // nr total de puncte inregistrate

    // inregistrez coord pct Bezier

    for (int i=0; i<7; i++) // punctele de pe curba Bezier
    {

        point p;
        float t = static_cast<float>(i)/6.0;
        bezier(p, A, B, C, D, t);
        n++;
        cub[n].x = p.x;
        cub[n].y = p.y;
        cub[n].z = 0;
        planuri[i].octagon[0].x=p.x;
        planuri[i].octagon[0].y=p.y;
        planuri[i].octagon[0].z=0;


        if(n!=0)
        {
            int k = n;            // pozitia nodului crt de pe curba Bezier
            // am inregistrat un punct de pe curba Bezier
            // mai sunt de inregistrat celelalte 7 puncte din planul orizontal
            // care vor constitui octogonul
            for(int j=1; j<8; j++)      // doar 7
            {
                n++;
                cub[n].y = cub[k].y;
                planuri[i].octagon[j].y=cub[k].y;
                sina = sin(j*a);
                cosa = cos(j*a);

                cub[n].x = cub[k].x * cosa - cub[k].z * sina;
                cub[n].z = cub[k].x * sina + cub[k].z * cosa;

                planuri[i].octagon[j].x=cub[k].x * cosa - cub[k].z * sina;
                planuri[i].octagon[j].z=cub[k].x * sina + cub[k].z * cosa;

            }
        }



    }

    //int test;

    //for(test=0;test<6;test++){
    //    std::cout<<planuri[test].octagon[0].y<<" ";
    //}

}















/* Mouse */
struct mouse_input_t
{
    int buttons[3];
    int x, y;
} mouse;

/* Camera vectors */
struct Vector3d
{
    float x, y, z;

} rot, eye;


/**
 * Application termination
 */
static void
cleanup ()
{
}

/**
 * Application initialisation. Setup keyboard input,
 * mouse input, timer, camera and OpenGL.
 */
static void
init ()
{
    /*
     * Application initialization
     */

    /* Init mouse */
    mouse.buttons[GLUT_LEFT_BUTTON] = GLUT_UP;
    mouse.buttons[GLUT_MIDDLE_BUTTON] = GLUT_UP;
    mouse.buttons[GLUT_RIGHT_BUTTON] = GLUT_UP;
    mouse.x = 0;
    mouse.y = 0;

    /* Init camera input */
    rot.x = 0.0f;
    eye.x = 0.0f;
    rot.y = 0.0f;
    eye.y = 0.0f;
    rot.z = 0.0f;
    eye.z = 7.0f;

    /*
     * OpenGL initialization
     */
    glClearColor (1.f, 1.f, 1.f, 1.0f);
    glShadeModel (GL_SMOOTH);

    genereaza();
    //glEnable (GL_DEPTH_TEST);
}

/**
 * GLUT's reshape callback function.
 * Update the viewport and the projection matrix.
 */
static void
reshape (int w, int h)
{
    if (h == 0)
        h = 1;

    glViewport (0, 0, (GLsizei)w, (GLsizei)h);

    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();

/*
    fovy - Specifies the field of view angle, in degrees, in the y direction.
    aspect - Specifies the aspect ratio that determines the field of view in the x direction. The aspect ratio is the ratio of x (width) to y (height).
    zNear - Specifies the distance from the viewer to the near clipping plane (always positive).
    zFar - Specifies the distance from the viewer to the far clipping plane (always positive).
    */
    gluPerspective (60.0, (GLfloat)w/(GLfloat)h, 0.1, 50.0);
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity ();
    glutPostRedisplay ();
}

/**
 * GLUT's display callback function.
 * Render the main OpenGL scene.
 */
static void
display ()
{
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity ();

    /* Camera rotation */
    glTranslated (-eye.x, -eye.y, -eye.z);
    glRotated (rot.x, 1.0f, 0.0f, 0.0f);
    glRotated (rot.y, 0.0f, 1.0f, 0.0f);
    glRotated (rot.z, 0.0f, 0.0f, 1.0f);

    glColor3f(1.0, 0.0, 0.0);
    /* Draw utah-teapot */
    //glutWireTeapot(1.0f);

    glBegin(GL_LINE_STRIP);
    // traseaza conturul vertical k
    glVertex3f(cub[0].x, cub[0].y, cub[0].z);
    for(int i=1; i<49; i+=8)
        glVertex3f(cub[i].x, cub[i].y, cub[i].z);
    glEnd();

    int pl;


    for(int j=0;j<8;j++){
            glBegin(GL_LINE_STRIP);
    glVertex3f(planuri[0].octagon[0].x,planuri[0].octagon[0].y,planuri[0].octagon[0].z);
    for(int i=1;i<7;i++){

        glVertex3f(planuri[i].octagon[j].x,planuri[i].octagon[j].y,planuri[i].octagon[j].z);
    }
    glEnd();
    }


    for(pl=1;pl<7;pl++){

         glBegin(GL_LINE_STRIP);
            int j;

            for(j=0;j<8;j++){
                glVertex3f(planuri[pl].octagon[j].x,planuri[pl].octagon[j].y,planuri[pl].octagon[j].z);
            }
            glVertex3f(planuri[pl].octagon[0].x,planuri[pl].octagon[0].y,planuri[pl].octagon[0].z);

            glEnd();
    }
    glColor3f(1., 0., 0.);
    glTranslatef(cub[0].x, cub[0].y, cub[0].z);
    glutSolidSphere(0.15,8,8);
    glTranslatef(-cub[0].x, -cub[0].y, -cub[0].z);
    for(int i=1; i<=41; i+=8)
    {
        glTranslatef(cub[i].x, cub[i].y, cub[i].z);
       // glutSolidSphere(0.15,8,8);
        glTranslatef(-cub[i].x, -cub[i].y, -cub[i].z);
    }

    glutSwapBuffers ();
}

/**
 * GLUT's mouse motion callback function.
 * Called when the user move the mouse. Update the
 * camera.
 */
static void
mouseMotion (int x, int y)
{
    if (mouse.buttons[GLUT_RIGHT_BUTTON] == GLUT_DOWN)
    {
        /* Zoom */
        eye.z += (x - mouse.x) * 0.1;
    }
    else if (mouse.buttons[GLUT_LEFT_BUTTON] == GLUT_DOWN)
    {
        /* Rotation */
        rot.x += (y - mouse.y);
        rot.y += (x - mouse.x);
    }

    mouse.x = x;
    mouse.y = y;

    glutPostRedisplay ();
}

/**
 * GLUT's mouse button callback function.
 * Called when the user press a mouse button. Update mouse
 * state and keyboard modifiers.
 */
static void
mouseButton (int button, int state, int x, int y)
{
    /* Update mouse state */
    mouse.buttons[button] = state;
    mouse.x = x;
    mouse.y = y;
}


/**
 * This is the main program.
 */
int
main (int argc, char *argv[])
{
    /* Initialize GLUT */
    glutInit (&argc, argv);

    /* Create an OpenGL window */
    glutInitDisplayMode (GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize (640, 480);
    glutCreateWindow ("Simple glut window");

    /* Initialize application */
    atexit (cleanup);
    init ();

    /* Setup GLUT's callback functions */
    glutReshapeFunc (reshape);
    glutDisplayFunc (display);

    glutMotionFunc (mouseMotion);
    glutMouseFunc (mouseButton);

    /* Enter the main loop */
    glutMainLoop ();

    return 0;
}
