// Fichero principal 
////////////////////////////////////////////////////
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using namespace std;
#include <iostream>
#include <cmath>
#include <vector>

#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <random>

#include "GL/glut.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include <SOIL/SOIL.h>


#define QT_ANELLS 10
#define PI 3.14159265358979323846

bool llum=true;

float avioX = 0.0;
float avioY = 0.0;
float avioZ = 0.0;
float angle_helix = 0.0;

float camX = 0.0;
float camY = 0.0;
float camZ = 0.0;

const int W_WIDTH = 1280; // Tama�o incial de la ventana
const int W_HEIGHT = 720;
GLfloat rotacion = 0.0;

// Variables de estado de la perspectiva
float fov = 45.0f;
float aspect = (float)W_WIDTH / (float)W_HEIGHT;
float nearClip = 0.1f;
float farClip = 1000.0f;



// Variables de la luz
GLfloat light_ambient[] = { 0.0025, 0.0025, 0.0025, 1.0 };
GLfloat llumAvio[] = { 0.0,0.0,0.0,1.0 };

// ID de la textura
GLuint texturaID; // ID de la textura

void drawBlenderModel(float x, float y, float z, string filename) {

	// Create Assimp importer and read file
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);
	if (!scene) {
		cerr << "Error: Unable to load model file '" << filename << "': " << importer.GetErrorString() << endl;
		return;
	}
	// Draw meshes
	glPushMatrix();
	glTranslatef(x, y, z);
	if (filename == "helix.obj") {
		glRotatef(angle_helix, 0.0, 0.0, 1.0);
		angle_helix = angle_helix + 20.0;
	}
	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		const aiMesh* mesh = scene->mMeshes[i];
		glBegin(GL_TRIANGLES);
		for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
			const aiFace& face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++) {
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


void dibuixaFanal(float x, float y, float z) {
	GLUquadricObj* sphere = gluNewQuadric();
	GLUquadricObj* cylinder = gluNewQuadric();
	// Set the color of the cylinder and sphere
	glPushMatrix();
	glTranslatef(x, y, z);
	gluSphere(sphere, 1.0, 50, 50);
	glColor3f(0.15f, 0.15f, 0.15f);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	gluCylinder(cylinder, 1.0, 1.0, 10.0, 50, 50);
	glColor3f(0.85f, 0.85f, 0.85f);

	glPopMatrix();
}

void dibuixaEnterra() {

	// Cargar la textura
	glGenTextures(1, &texturaID);
	glBindTexture(GL_TEXTURE_2D, texturaID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// Cargar la imagen de la textura con SOIL
	int width, height, channels;
	unsigned char* image = SOIL_load_image("hierba.png", &width, &height, &channels, SOIL_LOAD_RGB);
	if (!image) {
		std::cout << "Error al cargar la imagen de la textura" << std::endl;
		return;
	}

	// Asignar la imagen de la textura a OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	// Liberar la memoria de la imagen cargada con SOIL
	SOIL_free_image_data(image);

	// Dibujar el suelo
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturaID);

	glBegin(GL_QUADS);
	glColor3f(1.0f, 1.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-10000.0f, -10.0f, -10000.0f);

	glTexCoord2f(1000.0f, 0.0f);
	glVertex3f(10000.0f, -10.0, -10000.0f);

	glTexCoord2f(1000.0f, 1000.0f);
	glVertex3f(10000.0f, -10.0f, 10000.0f);

	glTexCoord2f(0.0f, 1000.0f);
	glVertex3f(-10000.0f, -10.0f, 10000.0f);

	glEnd();

	glDisable(GL_TEXTURE_2D);
}


// Función de manejo de eventos de teclado
void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		// Cambio coordenada X de la luz móvil
	case 'w':
		avioY = avioY + 0.5;
		break;
	case 'a':
		avioX = avioX + 0.5;
		break;
	case 's':
		avioY = avioY - 0.5;
		break;
	case 'd':
		avioX = avioX - 0.5;
		break;
	case ' ':
		if (camZ == 0.0) {
			camX = 0.0;
			camY = 0.0;
			camZ = 20.0;
		}
		else {
			camX = 0.0;
			camY = 0.0;
			camZ = 0.0;
		}
		break;
	case 'r':
		avioX = 0.0;
		avioY = 0.0;
		avioZ = -20.0;
		break;
	}
}


void dibuixaBombilla(float x, float y, float z) {
	glPushMatrix();
	glTranslatef(x, y, z-0.2); // move the cube to the specified coordinates

	glBegin(GL_QUADS);

	// Front face
	glColor3f(1.0f, 1.0f, 0.0f); // color groc
	glVertex3f(-0.05f, -0.05f, 0.05f);
	glVertex3f(0.05f, -0.05f, 0.05f);
	glVertex3f(0.05f, 0.05f, 0.05f);
	glVertex3f(-0.05f, 0.05f, 0.05f);

	// Back face
	glVertex3f(-0.05f, -0.05f, -0.05f);
	glVertex3f(-0.05f, 0.05f, -0.05f);
	glVertex3f(0.05f, 0.05f, -0.05f);
	glVertex3f(0.05f, -0.05f, -0.05f);

	// Top face
	glVertex3f(-0.05f, 0.05f, -0.05f);
	glVertex3f(-0.05f, 0.05f, 0.05f);
	glVertex3f(0.05f, 0.05f, 0.05f);
	glVertex3f(0.05f, 0.05f, -0.05f);

	// Bottom face
	glVertex3f(-0.05f, -0.05f, -0.05f);
	glVertex3f(0.05f, -0.05f, -0.05f);
	glVertex3f(0.05f, -0.05f, 0.05f);
	glVertex3f(-0.05f, -0.05f, 0.05f);

	// Right face
	glVertex3f(0.05f, -0.05f, -0.05f);
	glVertex3f(0.05f, 0.05f, -0.05f);
	glVertex3f(0.05f, 0.05f, 0.05f);
	glVertex3f(0.05f, -0.05f, 0.05f);

	// Left face
	glVertex3f(-0.05f, -0.05f, -0.05f);
	glVertex3f(-0.05f, -0.05f, 0.05f);
	glVertex3f(-0.05f, 0.05f, 0.05f);
	glVertex3f(-0.05f, 0.05f, -0.05f);

	glEnd();

	glPopMatrix();
}

// Función de manejo de eventos de teclado especial
void handleSpecialKeypress(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:
		camY = camY - 1.0;
		break;
	case GLUT_KEY_UP:
		camY = camY + 1.0;
		break;
	case GLUT_KEY_LEFT:
		camX = camX - 1.0;
		break;
	case GLUT_KEY_RIGHT:
		camX = camX + 1.0;
		break;
	}
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
		avioX + camX, avioY + camY + 2.0, avioZ + camZ - 10.0,
		avioX, avioY, avioZ,
		0.0f, 1.0f, 0.0f
	);


	dibuixaEnterra();
	glPushMatrix();
	glColor3f(1.0, 1.0, 1.0);
	drawBlenderModel(avioX, avioY, avioZ, "avio.obj");
	glPopMatrix();
	glEnd();

	glPushMatrix();
	glColor3f(0.8, 0.7, 0.6);
	drawBlenderModel(avioX, avioY, avioZ, "helix.obj");
	glPopMatrix();
	glEnd();



	for (int i = 0; i < QT_ANELLS; i++) {
		glPushMatrix();
		glTranslatef(5.0 + 2.7 * i, 5.0 + 1.5 * i, 20.0 * i);
		glutSolidTorus(0.5, 3.5, 75, 50);
		glPopMatrix();
	}

	llumAvio[0] = avioX;
	llumAvio[1] = avioY;
	llumAvio[2] = avioZ +0.15f;

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, llumAvio);

	dibuixaBombilla(llumAvio[0], llumAvio[1], llumAvio[2]);



	float fanal_0_position[] = { 10.0f, -5.0f, 60.0f };
	float fanal_1_position[] = { -10.0f, -5.0f, 60.0f };
	float fanal_2_position[] = { 10.0f, -5.0f, 200.0f };
	float fanal_3_position[] = { -10.0f, -5.0f, 200.0f };
	float fanal_4_position[] = { 10.0f, -5.0f, 340.0f };
	float fanal_5_position[] = { -10.0f, -5.0f, 340.0f };


	glColor3f(0.85f, 0.15f, 0.15f);
	dibuixaFanal(fanal_0_position[0], fanal_0_position[1], fanal_0_position[2]);

	glColor3f(0.15f, 0.85f, 0.15f);
	dibuixaFanal(fanal_1_position[0], fanal_1_position[1], fanal_1_position[2]);

	glColor3f(0.85f, 0.15f, 0.15f);
	dibuixaFanal(fanal_2_position[0], fanal_2_position[1], fanal_2_position[2]);

	glColor3f(0.15f, 0.85f, 0.15f);
	dibuixaFanal(fanal_3_position[0], fanal_3_position[1], fanal_3_position[2]);

	glColor3f(0.85f, 0.15f, 0.15f);
	dibuixaFanal(fanal_4_position[0], fanal_4_position[1], fanal_4_position[2]);

	glColor3f(0.15f, 0.85f, 0.15f);
	dibuixaFanal(fanal_5_position[0], fanal_5_position[1], fanal_5_position[2]);


	// Ponemos una luz

	// Ponemos una luz ambiental no muy potente
	glEnable(GL_LIGHT7);
	glLightfv(GL_LIGHT7, GL_AMBIENT, light_ambient);


	glFlush();

	glutSwapBuffers();

}

// Funci�n que se ejecuta cuando el sistema no esta ocupado
void Idle(void)
{
	if (rotacion > 360.0) rotacion = 0.0;
	rotacion += 0.1f;

	avioZ += 0.5;

	// Indicamos que es necesario repintar la pantalla
	glutPostRedisplay();
}

// Función que cambia el tamaño de la figura en base al tamaño de la ventana

void reshape(int width, int height)
{
	const int targetWidth = 1280;
	const int targetHeight = 720;

	float targetAspectRatio = static_cast<float>(targetWidth) / targetHeight;
	float currentAspectRatio = static_cast<float>(width) / height;

	int viewportWidth, viewportHeight;
	int offsetX, offsetY;

	if (currentAspectRatio > targetAspectRatio) {
		viewportHeight = height;
		viewportWidth = static_cast<int>(viewportHeight * targetAspectRatio);
		offsetX = (width - viewportWidth) / 2;
		offsetY = 0;
	}
	else {
		viewportWidth = width;
		viewportHeight = static_cast<int>(viewportWidth / targetAspectRatio);
		offsetX = 0;
		offsetY = (height - viewportHeight) / 2;
	}

	glViewport(offsetX, offsetY, viewportWidth, viewportHeight);
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

	// Establecemos la función de reshape
	glutReshapeFunc(reshape);

	// El color de fondo ser� el negro (RGBA, RGB + Alpha channel)
	glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(handleSpecialKeypress);

	// Habilitamos la renderización de luz
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);


	// Habilitar la niebla
	glEnable(GL_FOG);

	// Establecer el color de la niebla (en RGB)
	GLfloat fogColor[4] = { 0.5, 0.5, 0.5, 1.0 }; // Gris
	glFogfv(GL_FOG_COLOR, fogColor);

	// Establecer la densidad de la niebla
	GLfloat fogDensity = 0.01; // Valor entre 0.0 y 1.0
	glFogf(GL_FOG_DENSITY, fogDensity);



	// Establecer la distancia inicial y final de la niebla
	GLfloat fogStart = 100.0; // Distancia inicial de la niebla
	GLfloat fogEnd = 1000.0; // Distancia final de la niebla
	glFogf(GL_FOG_START, fogStart);
	glFogf(GL_FOG_END, fogEnd);

	// Comienza la ejecuci�n del core de GLUT
	glutMainLoop();
	return 0;
}