
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <iostream>
#include <sstream>

// Tamano inicial de la ventana
GLsizei winWidth =600, winHeight =600;

GLubyte numeros[16] = { '0', '0', '1', '1','2', '2', '3', '3', '4','4',
    '5', '5', '6', '6', '7','7'};
GLubyte expuesta[16] = { false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,};

int primerEscogido=-1; //si ya ha sido escogido una carta en el turno
int turnos=0;
int correctos = 0;
int actual=-1;
int antepenultimo=-1;

int decimas=0, segundos=0, dsegundos=0, minutos=0;
bool pausa=true;
bool inicio=false;
bool equivoco=false;

void drawText(float x, float y, std::string text, void* font) {
    glRasterPos3f(x, y, 0);
    for (std::string::iterator i = text.begin(); i != text.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
}

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

std::string toString(int value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}


void display(){
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    if (correctos==8){
        glRasterPos2i(0,0);
        drawText(-500, 0, "Felicidades, lo lograste en ", GLUT_BITMAP_TIMES_ROMAN_24);
        drawText(50, 0, toString(turnos), GLUT_BITMAP_TIMES_ROMAN_24);
        drawText(100, 0, " turnos!", GLUT_BITMAP_TIMES_ROMAN_24);
        pausa=true;
    }
    
    for(int x=0;x<16;x++){
        if(x%2==0){
            glColor3f(.33, .33, 1.0);
        }else{
            glColor3f(.13, .2, .6);
        }
        if (expuesta[x]){
            glColor3f(.5,.2,.1);
        }
        if(x<8){
            glBegin(GL_QUADS);
                glVertex2d(150*x-600, 600);
                glVertex2d(150*x-600, 400);
                glVertex2d(150*x-450, 400);
                glVertex2d(150*x-450, 600);
            glEnd();
            glColor3f(1, 1, 1);
            glRasterPos2f(150*x-575,500);
        } else{
            glBegin(GL_QUADS);
            glVertex2d(150*(x-8)-600, 350);
            glVertex2d(150*(x-8)-600, 150);
            glVertex2d(150*(x-8)-450, 150);
            glVertex2d(150*(x-8)-450, 350);
            glEnd();
            glColor3f(1, 1, 1);
            glRasterPos2f(150*(x-8)-575,250);
        }
        if (expuesta[x]) {
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, numeros[x]);
        }
    }
    
    glColor3ub(0,255,155);
    drawText(-500, -300, toString(minutos) + ":" + toString(dsegundos) + toString(segundos) + ":" + toString(decimas), GLUT_BITMAP_9_BY_15);
    
    drawText(400, -300, "Turnos: " + toString(turnos), GLUT_BITMAP_9_BY_15);
//    
    
    drawText(-500, -400, "Autores: Roberto Mtz y Adrian Rangel", GLUT_BITMAP_HELVETICA_18);
    drawText(-500, -450, "I-Iniciar, P-Pausa, R-Reiniciar, Esc-Salir", GLUT_BITMAP_HELVETICA_18);
    
    
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
            if(inicio){
                if(pausa){ //continue time
                    pausa = false;
                }
                else{
                    pausa = true;
                }
            }
            break;
        case 'i':
        case 'I':
            inicio=true;
            pausa = false;
            std::random_shuffle(&numeros[0], &numeros[16]);
            break;
            
        case 'R':
        case 'r': //resets time to zero
            pausa = true;
            decimas=0, segundos=0, dsegundos=0, minutos=0;
            std::random_shuffle(&numeros[0], &numeros[16]);
            turnos=0;
            correctos=0;
            for (int a=0;a<16;a++){
                expuesta[a]=false;
            }
            break;
        case 'Q':
        case 'q':        
        case 27:
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
                    if (equivoco){
                        expuesta[actual]=expuesta[antepenultimo]=false;
                        equivoco=false;
                    }
                    actual=(2*x)/150;
                    if(!expuesta[actual]){
                        expuesta[actual]=true;
                        if(primerEscogido==-1){
                            primerEscogido=actual;
                        }else{
                            if (!(numeros[actual]==numeros[primerEscogido])) {
                                equivoco=true;
                                antepenultimo=primerEscogido;
                            }else{
                                correctos++;
                            }
                            primerEscogido=-1;
                            turnos++;
                        }
                    }
                } else if (y<=185){
                    if (equivoco){
                        expuesta[actual]=expuesta[antepenultimo]=false;
                        equivoco=false;
                    }
                    actual=8+(2*x)/150;
                    if(!expuesta[actual]){
                        expuesta[actual]=true;
                        if(primerEscogido==-1){
                            primerEscogido=actual;
                        }else{
                            if (!(numeros[actual]==numeros[primerEscogido])) {
                                equivoco=true;
                                antepenultimo=primerEscogido;
                            }else{
                                correctos++;
                            }
                            primerEscogido=-1;
                            turnos++;
                        }
                        
                    }else{}
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