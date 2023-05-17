// Etapa6.cpp
// Fichero principal 
////////////////////////////////////////////////////
#include <iostream>
#include <cmath>

#include "GL/glut.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>


void drawModel(const aiScene* scene)
{
    for (unsigned int i = 0; i < scene->mNumMeshes; i++)
    {
        const aiMesh* mesh = scene->mMeshes[i];

        glBegin(GL_TRIANGLES);

        for (unsigned int j = 0; j < mesh->mNumFaces; j++)
        {
            const aiFace& face = mesh->mFaces[j];

            for (unsigned int k = 0; k < face.mNumIndices; k++)
            {
                unsigned int index = face.mIndices[k];

                glVertex3f(mesh->mVertices[index].x, mesh->mVertices[index].y, mesh->mVertices[index].z);
            }
        }

        glEnd();
    }
}

int main()
{
    GLFWwindow* window;

    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(640, 480, "Model Viewer", NULL, NULL);

    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile("avio.obj", aiProcess_Triangulate | aiProcess_GenSmoothNormals);

    if (!scene)
    {
        std::cerr << "Failed to load model" << std::endl;
        glfwTerminate();
        return -1;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)640 / (float)480, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window))
    {
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(0, 0, 5, 0, 0, 0, 0, 1, 0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        drawModel(scene);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}