// Etapa1.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <iostream>

#include "GL/glut.h"
#include "GL/gl.h"
#include "GL/glu.h"

const int W_WIDTH = 500; // Tama�o incial de la ventana
const int W_HEIGHT = 500;
GLfloat fAngulo; // Variable que indica el �ngulo de rotaci�n de los ejes. 
GLfloat tamany = 0;
GLboolean creix = true;
GLfloat radius = 0.25f;
GLint numSegments = 30;
GLfloat pos = 0;

// Funci�n que visualiza la escena OpenGL
void Display(void)
{
	// Borramos la escena
	glClear(GL_COLOR_BUFFER_BIT);

	glPushMatrix();
	// Creamos a continuaci�n dibujamos los tres poligonos

	/*
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 1.0f);

	glVertex3f(0.0, 0.0,0);
	glVertex3f(0.25, -0.25,0);

	glBegin(GL_LINES);
	glVertex3f(0.25, 0.25, 0);
	glVertex3f(-0.25, -0.25, 0);

	glEnd();
	*/

	// Quadrat superior esquerre
	
	glTranslatef(-0.5f,0.5f, 0.0f);
	glRotatef(fAngulo, 0.0f, 0.0f, 1.0f);
	
	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 1.0f);
	glVertex3f(-0.25f, 0.25f, 0.0f);
	glVertex3f(0.25f, 0.25f, 0.0f);
	glVertex3f(0.25f, -0.25f, 0.0f);
	glVertex3f(-0.25f, -0.25f, 0.0f);
	glEnd();

	

	glPopMatrix();

	// Polígon superior dret
	glPushMatrix();

	glTranslatef(0.5f, 0.5f, 0.0f);
	glScalef(tamany, tamany, 0.0f);

	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f(-0.25f, 0.25f, 0.0f);
	glVertex3f(0.25f, 0.25f, 0.0f);
	glVertex3f(0.25f, -0.25f, 0.0f);
	glVertex3f(-0.25f, -0.25f, 0.0f);
	glVertex3f(-0.35f, -0.15f, 0.0f);
	glEnd();

	glPopMatrix();

	// Triangle inferior esquerre

	glPushMatrix();
	//glTranslatef(0.5f, 0.5f, 0.0f);
	
	
	glTranslatef(-0.5f, -0.5f, 0.0f);
	glScalef(tamany-0.5f, tamany-0.5f, 0.0f);
	glRotatef(fAngulo, 0.0f, 0.0f, 1.0f);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);
	glEnd();

	glPopMatrix();

	// Quadrat inferior dret
	glPushMatrix();

	glTranslatef(0.5f, -0.5f, 0.0f);
	glTranslatef(pos, pos, 0.0f);

	glVertex2f(0.0f, 0.0f);

	glBegin(GL_TRIANGLE_FAN);
	glColor3f(1.0f, 0.0f, 0.0f);
	for (int i = 0; i <= numSegments; i++) {
		GLfloat angle = i * 2.0f * 3.1415 / numSegments;
		GLfloat x = radius * cosf(angle);
		GLfloat y = radius * sinf(angle);
		glVertex2f(x, y);
	}
	glEnd();

	
	
	glPopMatrix();

	// =========== Dibuixam Eixos ===========
	// Eix Y
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -1.0f, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glEnd();
	

	glFlush();

	glutSwapBuffers();
}

// Funci�n que se ejecuta cuando el sistema no esta ocupado
void Idle(void)
{
	// Incrementamos el �ngulo
	fAngulo += 0.3f;
	// Si es mayor que dos pi la decrementamos
	if (fAngulo > 360)
		fAngulo -= 360;


	if (creix) {
		pos += 0.0001f;
		tamany += 0.001f;
	}
	else{
		tamany -= 0.001f;
		pos -= 0.0001f;
	}
	if (tamany > 1.5)
		creix = false;
	if (tamany < 0.5)
		creix = true;
	// Indicamos que es necesario repintar la pantalla
	glutPostRedisplay();
}

// Función que cambia el tamaño de la figura en base al tamaño de la ventana

void reshape(int width, int height)
{
	if (width > height) {
		glViewport((width - height) / 2, 0, height, height);
	}
	else {
		glViewport(0, (height - width) / 2, width, width);
	}
}

// Funci�n principal
int main(int argc, char** argv)
{
	// Inicializamos la librer�a GLUT
	glutInit(&argc, argv);

	// Indicamos como ha de ser la nueva ventana
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	// Creamos la nueva ventana
	glutCreateWindow("Mi segunda segunda Ventana");

	// Indicamos cuales son las funciones de redibujado e idle
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);

	// Establecemos la función de reshape
	glutReshapeFunc(reshape);

	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f);

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}