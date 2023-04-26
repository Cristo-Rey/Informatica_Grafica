// Etapa4.cpp
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
float orbit_radius = 1.0; // Radio de la órbita de la esfera

// Variables de estado de la cámara
float camPosX = 0.0f;
float camPosY = 0.0f;
float camPosZ = 5.0f;
float camDirX = 0.0f;
float camDirY = 0.0f;
float camDirZ = -1.0f;
float camUpX = 0.0f;
float camUpY = 1.0f;
float camUpZ = 0.0f;

// Variables de estado de la perspectiva
int windowWidth = 800;
int windowHeight = 600;
float fov = 45.0f;
float aspect = (float)windowWidth / (float)windowHeight;
float nearClip = 0.1f;
float farClip = 1000.0f;


void dibuixaEixos() {
	glBegin(GL_LINES);
	// Eje X en rojo
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-100000.0f, 0.0f, 0.0f);
	glVertex3f(100000.0f, 0.0f, 0.0f);
	// Eje Y en verde
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0f, -100000.0f, 0.0f);
	glVertex3f(0.0f, 100000.0f, 0.0f);
	// Eje Z en azul
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0f, 0.0f, -100000.0f);
	glVertex3f(0.0f, 0.0f, 100000.0f);
	glEnd();
}

// Función de manejo de eventos de teclado
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		camPosX += camDirX * 0.1f;
		camPosY += camDirY * 0.1f;
		camPosZ += camDirZ * 0.1f;
		break;
	case 's':
		camPosX -= camDirX * 0.1f;
		camPosY -= camDirY * 0.1f;
		camPosZ -= camDirZ * 0.1f;
		break;
	case 'a':
		camPosX += camUpX * 0.1f;
		camPosY += camUpY * 0.1f;
		camPosZ += camUpZ * 0.1f;
		break;
	case 'd':
		camPosX -= camUpX * 0.1f;
		camPosY -= camUpY * 0.1f;
		camPosZ -= camUpZ * 0.1f;
		break;
	case 'q':
		camDirX = camDirX * cos(0.1f) + camUpX * sin(0.1f);
		camDirY = camDirY * cos(0.1f) + camUpY * sin(0.1f);
		camDirZ = camDirZ * cos(0.1f) + camUpZ * sin(0.1f);
		break;
	case 'e':
		camDirX = camDirX * cos(-0.1f) + camUpX * sin(-0.1f);
		camDirY = camDirY * cos(-0.1f) + camUpY * sin(-0.1f);
		camDirZ = camDirZ * cos(-0.1f) + camUpZ * sin(-0.1f);
		break;

		// plano cenital
	case '1':
		camPosX = 0.00f;
		camPosY = 5.0f;
		camPosZ = 0.00f;

		camDirX = 0.001f;
		camDirY = -2.6f;
		camDirZ = 0.001f;

		camUpX = 0.0f;
		camUpY = 1.0f;
		camUpZ = 0.0f;

		break;
		// Plano picado
	case '2':
		camPosX = -5.0f;
		camPosY = 5.0f;
		camPosZ = 0.0f;

		camDirX = 0.5f;
		camDirY = -0.5f;
		camDirZ = 0.0f;

		camUpX = 0.0f;
		camUpY = 1.0f;
		camUpZ = 0.0f;
		break;
		// Plano Normal
	case '3':
		camPosX = 0.0f;
		camPosY = 0.0f;
		camPosZ = 5.0f;
		camDirX = 0.0f;
		camDirY = 0.0f;
		camDirZ = -1.0f;
		camUpX = 0.0f;
		camUpY = 1.0f;
		camUpZ = 0.0f;

		break;
		// Plano contrapicado
	case '4':
		camPosX = 0.0f;
		camPosY = 3.0f;
		camPosZ = 3.0f;

		camDirX = 0.0f;
		camDirY = -0.5f;
		camDirZ = -0.5f;

		camUpX = 0.0f;
		camUpY = 1.0f;
		camUpZ = 0.0f;

		break;
		// Plano Nadir
	case '5':
		camPosX = 0.00f;
		camPosY = -5.0f;
		camPosZ = 0.00f;

		camDirX = 0.001f;
		camDirY = 2.6f;
		camDirZ = 0.001f;

		camUpX = 0.0f;
		camUpY = 1.0f;
		camUpZ = 0.0f;
		break;
	}
	std::cout << "--------------------------------------------------------------------------" << std::endl;
	std::cout << "Camera position: (" << camPosX << ", " << camPosY << ", " << camPosZ << ")" << std::endl;
	std::cout << "Camera direction: (" << camDirX << ", " << camDirY << ", " << camDirZ << ")" << std::endl;
	std::cout << "Camera up vector: (" << camUpX << ", " << camUpY << ", " << camUpZ << ")" << std::endl;
}

// Función de manejo de eventos de teclado especial
void handleSpecialKeypress(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		camDirY = camDirY * cos(0.1f) - camDirZ * sin(0.1f);
		camDirZ = camDirZ * cos(0.1f) + camDirY * sin(0.1f);
		break;
	case GLUT_KEY_DOWN:
		camDirY = camDirY * cos(-0.1f) - camDirZ * sin(-0.1f);
		camDirZ = camDirZ * cos(-0.1f) + camDirY * sin(-0.1f);
		break;
	case GLUT_KEY_LEFT:
		camDirX = camDirX * cos(0.1f) - camDirZ * sin(0.1f);
		camDirZ = camDirZ * cos(0.1f) + camDirX * sin(0.1f);
		break;
	case GLUT_KEY_RIGHT:
		camDirX = camDirX * cos(-0.1f) - camDirZ * sin(-0.1f);
		camDirZ = camDirZ * cos(-0.1f) + camDirX * sin(-0.1f);
		break;
	}
	std::cout << "--------------------------------------------------------------------------" << std::endl;
	std::cout << "Camera position: (" << camPosX << ", " << camPosY << ", " << camPosZ << ")" << std::endl;
	std::cout << "Camera direction: (" << camDirX << ", " << camDirY << ", " << camDirZ << ")" << std::endl;
	std::cout << "Camera up vector: (" << camUpX << ", " << camUpY << ", " << camUpZ << ")" << std::endl;
}

// Funci�n que visualiza la escena OpenGL
void Display(void)
{
	// Limpiar el buffer de color y profundidad
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Configurar la perspectiva de la cámara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, nearClip, farClip);

	// Configurar la posición y dirección de la cámara
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(camPosX, camPosY, camPosZ, camPosX + camDirX, camPosY + camDirY, camPosZ + camDirZ, camUpX, camUpY, camUpZ);

	dibuixaEixos();

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0); // Establecer el color rojo
	//glRotatef(rotacion, 0.0, 1.0, 1.0);
	glutWireTeapot(1); // Dibujar una tetera 
	glPopMatrix();

	glPushMatrix();
	// Dibujar la esfera que orbita
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(orbit_radius * cos(orbit_angle), 0.0, orbit_radius * sin(orbit_angle));
	glutWireSphere(0.5, 20, 20);
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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);


	// Creamos la nueva ventana
	glutCreateWindow("Mi primera cuarta Ventana");

	// Indicamos cuales son las funciones de redibujado e idle
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);

	// Establecemos la función de reshape
	glutReshapeFunc(reshape);

	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glFrustum(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 10.0f);
	//glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f);

	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleSpecialKeypress);

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}