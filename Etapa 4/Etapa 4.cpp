// Etapa1.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <iostream>
#include <cmath>

#include "GL/glut.h"
#include "GL/gl.h"
#include "GL/glu.h"

#define PI 3.14159265358979323846

const int W_WIDTH = 500; // Tama�o incial de la ventana
const int W_HEIGHT = 500;
GLfloat rotacion = 0.0;

float orbit_angle = 0.0; // Ángulo de la órbita de la esfera
float orbit_radius = 0.90; // Radio de la órbita de la esfera


void dibuixaEixos() {
	glBegin(GL_LINES);
	// Eje X en rojo
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-1000.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);
	// Eje Y en verde
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0f, -1000.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	// Eje Z en azul
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0f, 0.0f, -1000.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	glEnd();
}

// Funci�n que visualiza la escena OpenGL
void Display(void)
{
	// Borramos la escena
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glFrustum(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 10.0f);

	glEnable(GL_DEPTH_TEST);


	// Pes si volem inclinar lleument la càmara i poder apreciar els 3 eixos
	//gluLookAt(0.0, 1.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//gluLookAt(0.0, 2.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	dibuixaEixos();

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0); // Establecer el color rojo
	glRotatef(rotacion, 0.0, 1.0, 1.0);
	glutWireTeapot(0.5); // Dibujar una tetera 
	glPopMatrix();

	glPushMatrix();
	// Dibujar la esfera que orbita
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(orbit_radius * cos(orbit_angle), 0.0, orbit_radius * sin(orbit_angle));
	glutWireSphere(0.2, 20, 20);
	glTranslatef(-orbit_radius * cos(orbit_angle), 0.0, -orbit_radius * sin(orbit_angle));
	glPopMatrix();

	glFlush();

	glutSwapBuffers();
}

// Funci�n que se ejecuta cuando el sistema no esta ocupado
void Idle(void)
{
	if (rotacion > 360.0) rotacion = 0.0;
	rotacion += 0.1f;

	orbit_angle += 0.001; // Incrementar el ángulo de la órbita

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
	glutCreateWindow("Mi primera tercera Ventana");

	// Indicamos cuales son las funciones de redibujado e idle
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);

	// Establecemos la función de reshape
	glutReshapeFunc(reshape);

	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glFrustum(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 10.0f);
	//glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f);

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}