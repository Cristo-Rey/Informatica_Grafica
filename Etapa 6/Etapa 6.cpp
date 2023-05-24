// Fichero principal 
////////////////////////////////////////////////////
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;
#include <iostream>
#include <cmath>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <random>


#include "GL/glut.h"
#include "GL/gl.h"
#include "GL/glu.h"

#define PI 3.14159265358979323846

const int W_WIDTH = 500; // Tama�o incial de la ventana
const int W_HEIGHT = 500;
GLfloat rotacion = 0.0;

float orbit_angle = 0.0; // Ángulo de la órbita de la esfera
float orbit_radius = 2.0; // Radio de la órbita de la esfera

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
float camera_pitch = 0.0f; // ángulo de inc linación de la cámara
float camera_yaw = 0.0f; // ángulo de giro de la cámara

// Variables de la luz
GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

void drawBlenderModel(float x, float y, float z, string filename) {
	std::random_device rd;
	std::mt19937 gen(rd());

	// Define the range of possible values
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	// Create Assimp importer and read file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene) {
		cerr << "Error: Unable to load model file '" << filename << "': " << importer.GetErrorString() << endl;
		return;
	}
	float colors[3][3] = { {0.0,0.24,0.02},{0.17,0.09,0.03}, {0.54, 0.26, 0.01} };
	// Draw meshes
	glPushMatrix();
	glTranslatef(x, y, z);
	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		const aiMesh* mesh = scene->mMeshes[i];
		glBegin(GL_TRIANGLES);
		for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
			const aiFace& face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++) {
				int randomn = dist(gen);
				//glColor3f(colors[randomn][0],colors[randomn][1],colors[randomn][2]);
				glColor3f(dist(gen),dist(gen), dist(gen));
				unsigned int index = face.mIndices[k];
				if (mesh->HasNormals()) {
					glNormal3fv(&mesh->mNormals[index].x);
				}
				glVertex3fv(&mesh->mVertices[index].x);
			}
		}
		glEnd();
	}
	glPopMatrix();
}

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
	}
}

// Función de manejo de eventos de teclado especial
void handleSpecialKeypress(int key, int x, int y) {
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
	// Habilitar el z-buffer
	glEnable(GL_DEPTH_TEST);

	// Configurar la función de comparación de profundidad
	glDepthFunc(GL_LEQUAL);

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


	dibuixaEixos();
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	glTranslatef(orbit_radius * cos(orbit_angle), 0.0, orbit_radius * sin(orbit_angle));
	drawBlenderModel(0, 0, 0, "avionsi.obj");
	glColor3f(0.8, 0.7, 0.6);
	drawBlenderModel(0, 0, 0, "helixi.obj");
	glTranslatef(-orbit_radius * cos(orbit_angle), 0.0, -orbit_radius * sin(orbit_angle));
	glPopMatrix();

	glEnd();

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
	rotacion += 0.05f;

	orbit_angle += 0.1; // Incrementar el ángulo de la órbita

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