
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <iostream>

// TamaÒo inicial de la ventana
GLsizei winWidth =600, winHeight =600;

GLubyte numeros[16] = { '0', '0', '1', '1','2', '2', '3', '3', '4','4',
    '5', '5', '6', '6', '7','7'};
GLubyte expuesta[16] = { false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,};

int primerEscogido=-1; //si ya ha sido escogido una carta en el turno
int turnos=0;
int correctos = 0;

int decimas=0, segundos=0, dsegundos=0, minutos=0;
bool pausa=true;

void myTimer(int i) {
    if (correctos<8) {
        if(!pausa){
            decimas++;
            if (decimas>9){
                decimas=0;
                segundos++;
                if (segundos>9) {
                    segundos=0;
                    dsegundos++;
                    if (dsegundos>5) {
                        dsegundos=0;
                        minutos++;
                    }
                }
            }
        }
    }
    glutPostRedisplay();
    glutTimerFunc(100,myTimer,1);
}


void display(){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (correctos==8){
        glRasterPos2i(0,0);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'F');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'i');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'n');
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (char)(((int)'0')+turnos));
        pausa=true;
    }
    
    for(int x=0;x<17;x++){
        if(x%2==0){
            glColor3f(.33, .33, 1.0);
        }else{
            glColor3f(.13, .2, .6);
        }
        if (expuesta[x]){
            glColor3f(.5,.2,.1);
        }
        glBegin(GL_QUADS);
            glVertex2d(75*x-600, 600);
            glVertex2d(75*x-600, 400);
            glVertex2d(75*x-525, 400);
            glVertex2d(75*x-525, 600);
        glEnd();
        glColor3f(1, 1, 1);
        glRasterPos2f(75*x-575,500);
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, numeros[x]);
    }
    
    glColor3ub(0,255,155);
    glRasterPos2i(-500,-500);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (char)(((int)'0')+minutos));
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ':');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (char)(((int)'0')+dsegundos));
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (char)(((int)'0')+segundos));
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ':');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (char)(((int)'0')+decimas));
    
    glRasterPos2i(400,-500);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ':');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, (char)(((int)'0')+turnos));
    
    
//    glRasterPos2f(-600,.9f);
//    for (int x=0; x<16;x++){
//        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, numeros[x]);
//    }
    
    
    
    glutSwapBuffers();//ya tiene integrado el glFlush
    
}


void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
    switch (theKey)
    {
        case 'p':
        case 'P':
            if(pausa){ //continue time
                pausa = false;
            }
            else{
                pausa = true;
            }
            break;
        case 'i':
        case 'I':
            pausa = false;
            std::random_shuffle(&numeros[0], &numeros[16]);
            break;
            
        case 'R':
        case 'r': //resets time to zero
            pausa = true;
            decimas=0, segundos=0, dsegundos=0, minutos=0;
            break;
        case 'E':
        case 'e':
            exit(-1);
            //terminate the program
        default:
            break;		      // do nothing
    }
}


void myMouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN)
    {
        //Si el usuario oprime el boton izq del mouse
        if (button == GLUT_LEFT_BUTTON)
        {
            if (!pausa) {
                if (y<=85){
                    int actual=(2*x)/75;
                    if(!expuesta[actual]){
                        expuesta[actual]=true;
                        if(primerEscogido==-1){
                            primerEscogido=actual;
                        }else{
                            if (!(numeros[actual]==numeros[primerEscogido])) {
                                expuesta[actual]=expuesta[primerEscogido]=false;
                            }else{correctos++;}
                            primerEscogido=-1;
                            turnos++;
                        }
                    }
                }
            }
        }
        else if (button == GLUT_RIGHT_BUTTON)
        {
        }
        std::cout<<x<<" "<<y<<std::endl;
    }
    else
        if (state == GLUT_UP)
        {
            if (button == GLUT_LEFT_BUTTON)
            {
                
            }
            else if (button == GLUT_RIGHT_BUTTON)
            {
            }
        }
}



void init(void)
{
    glClearColor(0,0,0,1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(-600,600, -600,600);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(600,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE ); //si pones el double tienes que poner glutswapbuffers
    glutCreateWindow("Animacion");
    glutDisplayFunc(display);
    init();
    glutMouseFunc( myMouse );
    glutKeyboardFunc(myKeyboard);
    glutTimerFunc(100,myTimer,1);
    glutMainLoop();
    return EXIT_SUCCESS;
}