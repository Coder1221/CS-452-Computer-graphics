#include <stdlib.h>
#include "GL/glut.h"
static int year = 0;
static int day = 0;
static int mars = 0;
static int mars2 = 0;

void init(void){
    glClearColor(0.0,0.0, 0.0,0.0);
    glShadeModel(GL_SMOOTH);
}
void display(void){
    // glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    // glEnable( GL_DEPTH_TEST );
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable ( GL_TEXTURE_3D );
    glColor3f(1.0,1.0,1.0);
    glPushMatrix();
        glColor4f(1,1,0,1);
        glutSolidSphere(1,20,12);
        
        glRotatef((GLfloat)year ,0.0,1.0,0.0); // rotate around sun
        glColor4f(1,0,0,1);
        glTranslatef(2.0,0.0,0.0); // rotation around origon scale to get required positon
        glRotatef((GLfloat)day , 0.0 , 1.0, 0.0);
        glutSolidSphere(0.4,10,4); // create plannet
 
        // custom moon of earth xd
        glColor4f(0,1,0,1);
        glRotatef((GLfloat)year ,0.0,1.0,0.0); // rotate around sun
        glTranslatef(0.8,0.0,0.0); // rotation around origon scale to get required positon
        glRotatef((GLfloat)day , 0.0 , 1.0, 0.0);
        glutSolidSphere(0.2,10,4);
    glPopMatrix();


    //1
    glPushMatrix();
        glColor4f(1,0,1,0);
        glRotatef((GLfloat)year/2 ,0.0,1.0,0.0); // rotate around sun
        glTranslatef(4.0,0,0.0); // rotation around origon scale to get required positon
        glutSolidSphere(0.50,10,4); // create plannet
    
        glColor4f(180,22,180,0);
        glTranslatef(2.0,0.0,0.0); // rotation around origon scale to get required positon
        glutSolidSphere(0.45,10,4); // create plannet
    glPopMatrix();
    
    glPushMatrix();
        glColor4f(0,0,255,0);
        glRotatef((GLfloat)mars/2 ,0.0,1.0,0.0); // rotate around sun
        glTranslatef(8.0,0,0.0); // rotation around origon scale to get required positon
        glutSolidSphere(0.30,10,4); // create plannet
    
        glColor4f(0,255,0,0);
        glTranslatef(2.0,0.0,0.0); // rotation around origon scale to get required positon
        glutSolidSphere(0.25,10,4); // create plannet
    glPopMatrix();
    

    //2
    glPushMatrix();
        glColor4f(255,0,0,2);
        glRotatef((GLfloat)mars/4 ,0.0,1.0,0.0); // rotate around sun
        glTranslatef(6.0,-0.001,0.0); // rotation around origon scale to get required positon
        glutSolidSphere(0.20,10,4); // create plannet
        
        glColor4f(101,102,0,10);
        glTranslatef(3.0,0.002,0.0); // rotation around origon scale to get required positon
        glutSolidSphere(0.10,10,4); // create plannet    
    glPopMatrix();

    glutSwapBuffers();
}
void reshape(int w , int h){
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(88.0,(GLfloat)w/(GLfloat)h ,1.0 ,20.0);
    gluLookAt(0.0, 0.0, 3.5, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void timer(int i){
    year = (year + 2) % 360;
    day =  (day + 10) % 360;
    mars = (mars + 1) % 360;
    mars2 = (mars2 + 5) % 360;

    glutPostRedisplay();
    glutTimerFunc(50,timer , 1);
}

void keyboard(unsigned char key,int x,int y){
    switch (key){
        case 27:
            exit(0);
            break;
    }
}

int main(int argc ,char ** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(1300,300);
    glutInitWindowPosition(100,100);
    glutCreateWindow("DONT know what i am doing xd ");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(50,timer,year);
    glutMainLoop();
    return 0;
}