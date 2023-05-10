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
float orbit_radius = 2.0; // Radio de la órbita de la esfera

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

// Variables camara
int last_x = 0, last_y = 0;   // última posición del ratón
float camera_distance = 10.0f; // distancia de la cámara al origen
float camera_pitch = 0.0f; // ángulo de inclinación de la cámara
float camera_yaw = 0.0f; // ángulo de giro de la cámara

// Variables de la luz
GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLboolean sombreado = false;


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

		// Cambio coordenada X de la luz móvil
	case 'a':
		light_position[0] -= 0.5;
		break;
	case 'z':
		// Cambio coordenada X de la luz móvil
		light_position[0] += 0.5;
		break;
	case 's':
		// Cambio coordenada Y de la luz móvil
		light_position[1] -= 0.5;
		break;
	case 'x':
		// Cambio coordenada Y de la luz móvil
		light_position[1] += 0.5;
		break;
		// Cambio coordenada Z de la luz móvil
	case 'd':
		light_position[2] -= 0.5;
		break;
		// Cambio coordenada Z de la luz móvil
	case 'c':
		light_position[2] += 0.5;
		break;
		// Cambiamos el modo se sombreado entre Smooth y Flat
	case ' ':
		if (sombreado) {
			glShadeModel(GL_FLAT);
			sombreado = false;
		}
		else {
			glShadeModel(GL_SMOOTH);
			sombreado = true;
		}

		break;
	}
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
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
}

// Dibuixa un cub. Emprat per saber on tenim les llums col·locades
void dibuixaBombilla(float x, float y, float z) {

	glPushMatrix();
	glTranslatef(x, y, z); // move the cube to the specified coordinates

	glBegin(GL_QUADS);

	// Front face
	glColor3f(0.1f, 0.1f, 0.1f); // color blanc
	glVertex3f(-0.1f, -0.1f, 0.1f);
	glVertex3f(0.1f, -0.1f, 0.1f);
	glVertex3f(0.1f, 0.1f, 0.1f);
	glVertex3f(-0.1f, 0.1f, 0.1f);

	// Back face
	glVertex3f(-0.1f, -0.1f, -0.1f);
	glVertex3f(-0.1f, 0.1f, -0.1f);
	glVertex3f(0.1f, 0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, -0.1f);

	// Top face
	glVertex3f(-0.1f, 0.1f, -0.1f);
	glVertex3f(-0.1f, 0.1f, 0.1f);
	glVertex3f(0.1f, 0.1f, 0.1f);
	glVertex3f(0.1f, 0.1f, -0.1f);

	// Bottom face
	glVertex3f(-0.1f, -0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, -0.1f);
	glVertex3f(0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, -0.1f, 0.1f);

	// Right face
	glVertex3f(0.1f, -0.1f, -0.1f);
	glVertex3f(0.1f, 0.1f, -0.1f);
	glVertex3f(0.1f, 0.1f, 0.1f);
	glVertex3f(0.1f, -0.1f, 0.1f);

	// Left face
	glVertex3f(-0.1f, -0.1f, -0.1f);
	glVertex3f(-0.1f, -0.1f, 0.1f);
	glVertex3f(-0.1f, 0.1f, 0.1f);
	glVertex3f(-0.1f, 0.1f, -0.1f);

	glEnd();

	glPopMatrix();
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
	gluLookAt(
		camera_distance * sin(camera_yaw) * cos(camera_pitch),
		camera_distance * sin(camera_pitch),
		camera_distance * cos(camera_yaw) * cos(camera_pitch),
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	);
	//gluLookAt(camPosX, camPosY, camPosZ, camPosX + camDirX, camPosY + camDirY, camPosZ + camDirZ, camUpX, camUpY, camUpZ);


	dibuixaEixos();

	glPushMatrix();
	glColor3f(1.0, 0.0, 0.0); // Establecer el color rojo
	//glRotatef(rotacion, 0.0, 1.0, 1.0);
	glutSolidTeapot(1); // Dibujar una tetera 
	glPopMatrix();


	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 0.0);
	// Set the normal vector to (0,1,0)
	glNormal3f(0.0f, -1.0f, 0.0f);

	// Set the vertices of the square
	glVertex3f(-1.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, -1.0f);

	glEnd();



	glPushMatrix();
	// Dibujar la esfera que orbita
	glColor3f(0.0, 0.0, 1.0);
	glTranslatef(orbit_radius * cos(orbit_angle), 0.0, orbit_radius * sin(orbit_angle));
	glutSolidSphere(0.5, 20, 20);
	glTranslatef(-orbit_radius * cos(orbit_angle), 0.0, -orbit_radius * sin(orbit_angle));
	glPopMatrix();


	// Ponemos una luz
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	// Ponemos un cubo en el origen de la luz para ver donde está colocada la luz
	dibuixaBombilla(light_position[0], light_position[1], light_position[2]);

	// Ponemos una luz ambiental no muy potente
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);


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

// Función para controlar el movimiento del ratón
void motion(int x, int y)
{
	int delta_x = x - last_x;
	int delta_y = y - last_y;

	camera_yaw += delta_x * 0.01f;
	camera_pitch += delta_y * 0.01f;

	if (camera_pitch < -1.5f) {
		camera_pitch = -1.5f;
	}
	if (camera_pitch > 1.5f) {
		camera_pitch = 1.5f;
	}

	last_x = x;
	last_y = y;

	glutPostRedisplay();
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
	glutCreateWindow("Mi primera sexta Ventana");

	// Indicamos cuales son las funciones de redibujado e idle
	glutDisplayFunc(Display);
	glutIdleFunc(Idle);

	// Movimiento de la camara con el raton
	glutMotionFunc(motion);

	// Establecemos la función de reshape
	glutReshapeFunc(reshape);

	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glFrustum(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 10.0f);
	//glOrtho(-1.0, 1.0f, -1.0, 1.0f, -1.0, 1.0f);

	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleSpecialKeypress);

	// Habilitamos la renderización de luz
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}