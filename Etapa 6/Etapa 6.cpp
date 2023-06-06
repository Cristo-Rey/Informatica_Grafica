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

float avioX = 0.0;
float avioY = 0.0;
float avioZ = 0.0;
float angle_helix = 0.0;

const float ANELLS_COORDS[10][3] = { {0.0,0.0,5.0},{5.0,5.0,5.0},{20.0,15.0,10.0},{7.0,5.0,7.0},{-5.0,5.0,5.0},{-10.0,15.0,10.0},{-10.0,-15.0,10.0},{-7.0,5.0,5.0},{-7.0,-5.0,5.0},{-10.0,0.0,5.0} };

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
	if(filename=="helix.obj"){
		glRotatef(angle_helix,0.0,0.0,1.0);
		angle_helix=angle_helix+20.0;
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


void drawCylinderWithSphere(float x, float y, float z, float h, float r, float sphereRadius, int slices, int stacks) {
    // Set the color of the cylinder and sphere
    glColor3f(0.5f, 0.5f, 0.5f);

    // Draw the top and bottom disks of the cylinder
    const float diskHeight = h / 2.0f;
    const int diskSlices = slices;
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, y + diskHeight, z);
    for (int i = 0; i <= diskSlices; i++) {
        float angle = i * 2.0f * PI / diskSlices;
        glVertex3f(x + r * cos(angle), y + diskHeight, z + r * sin(angle));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(x, y - diskHeight, z);
    for (int i = 0; i <= diskSlices; i++) {
        float angle = i * 2.0f * PI / diskSlices;
        glVertex3f(x + r * cos(angle), y - diskHeight, z + r * sin(angle));
    }
    glEnd();

    // Draw the sides of the cylinder
    const float sideHeight = h;
    const int sideSlices = slices;
    const int sideStacks = stacks;
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= sideSlices; i++) {
        float angle = i * 2.0f * PI / sideSlices;
        float x1 = x + r * cos(angle);
        float z1 = z + r * sin(angle);
        float x2 = x + r * cos(angle);
        float z2 = z + r * sin(angle);
        for (int j = 0; j <= sideStacks; j++) {
            float y = -diskHeight + j * sideHeight / sideStacks;
            glVertex3f(x1, y, z1);
            glVertex3f(x2, y + sideHeight / sideStacks, z2);
        }
    }
    glEnd();

    // Draw the sphere on top of the cylinder
    const int sphereSlices = slices;
    const int sphereStacks = stacks;
    glColor3f(1.0f, 1.0f, 0.0f); // set the color of the sphere to yellow
    glPushMatrix();
    glTranslatef(x, y + diskHeight + sphereRadius, z); // move to the top of the cylinder
    glutSolidSphere(sphereRadius, sphereSlices, sphereStacks); // draw a sphere
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
		avioZ = avioZ + 1.0;
		break;
	case 'a':
		avioX = avioX + 1.0;
		break;
	case 's':
		avioZ = avioZ - 1.0;
		break;
	case 'd':
		avioX = avioX - 1.0;
		break;
	}
}

// Función de manejo de eventos de teclado especial
void handleSpecialKeypress(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_DOWN:
		avioY = avioY - 1.0;
		break;
	case GLUT_KEY_UP:
		avioY = avioY + 1.0;
		break;
	}
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
		glTranslatef(5.0 + 2.7 * i, 5.0 + 1.5 * i, 10.0 * i);
		glutWireTorus(0.5, 3.5, 75, 50);
		glPopMatrix();
	}

	drawCylinderWithSphere(0.0f, 0.0f, -10.0f, 4.0f, 1.0f, 0.5f, 160, 80);

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
	glClearColor(0.53f, 0.81f, 0.92f, 1.0f);

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