#include <iostream>
#define GLFW_INCLUDE_NONE

#include <vector>
#include <string>
#include "CShape.h"

#include "stb_image.h"
#include <assimp/Importer.hpp>			// |Assimp
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>
#include "imgui.h"
#include "SGUI.h"

CShape::CShape() {
}

std::vector<CShape *> CShape::list = std::vector<CShape *>();


void CShape::init(const char* pathobj, const char * path, int pIndex) {

    static Assimp::Importer importer;
    const aiScene * scene = importer.ReadFile(pathobj, aiProcess_Triangulate
        | aiProcess_FlipUVs);

    if (!scene) {
        fprintf(stderr, "%s", importer.GetErrorString());
        getchar();
        return;
    }

    this->scene = importer.GetOrphanedScene();

    CShape::list.push_back(this);

    const aiMesh * mesh = scene->mMeshes[pIndex];
    GLuint numFaces = mesh->mNumFaces;
    GLuint numVertices = mesh->mNumVertices;
    aiVector3D * vertices = mesh->mVertices;

    this->mNumElements = numFaces * 3;

    aiVector3D * myVertices = (aiVector3D *)malloc(numVertices * sizeof(aiVector3D));
    GLfloat * myUVs = (float *)malloc(numVertices * sizeof(float) * 2);
    GLuint * myElements = (unsigned int *)malloc(numFaces * sizeof(unsigned int) * 3);

    const aiFace * faces = mesh->mFaces;

    myVertices = vertices;
    /*
    int j = 0;
    for (int i = 0; i < numVertices; i++) {
        myVertices[j] = vertices[i].x;
        myVertices[j + 1] = vertices[i].y;
        myVertices[j + 2] = vertices[i].z;
        j += 3;
    }
*/
    int j = 0;
    for (int i = 0; i < numFaces; i++) {
        GLuint mIndices = faces[i].mNumIndices;
        GLuint * indices = faces[i].mIndices;

        myElements[j] = indices[0];
        myElements[j + 1] = indices[1];
        myElements[j + 2] = indices[2];
        j += 3;
    }

    std::cout << "=========================================================" << std::endl;
    std::cout << "Mesh: " << pathobj << std::endl;
    std::cout << "mNumMeshes: " << scene->mNumMeshes << std::endl;
    std::cout << "HasTexture: " << scene->HasTextures() << std::endl;
    std::cout << "HasTextureCoords: " << mesh->HasTextureCoords(0) << std::endl;

    if (mesh->HasTextureCoords(0)) {
        const unsigned int *  mNumUV = mesh->mNumUVComponents;
        std::cout << "mNumUvCompnents: " << *mNumUV << std::endl;

        int j = 0;
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            myUVs[j] = mesh->mTextureCoords[0][i].x;
            myUVs[j + 1] = mesh->mTextureCoords[0][i].y;
            j += 2;
        }

        std::cout << "NOMBRE UVS: " << mesh->mNumUVComponents << std::endl;

    }
    std::cout << "---------------------------------------------------------" << std::endl;


    this->vertices = myVertices;
    this->indices = myElements;
    this->uv = myUVs;
    this->path = path;

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &UVO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(aiVector3D)*numVertices, this->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*numFaces * 3, this->indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, UVO);
    if (mesh->HasTextureCoords(0))
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices * 2, this->uv, GL_STATIC_DRAW);
    else
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*numVertices * 3, this->vertices, GL_STATIC_DRAW);

    this->setup();
}


void CShape::setup() {
    glGenTextures(1, &this->TextureID);
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(this->path, &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cout << "Load image SUCCESS" << std::endl;
        std::cout << "Image size : " << width << ", " << height << std::endl;
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

void CShape::draw() {
    glBindTexture(GL_TEXTURE_2D, this->TextureID);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, UVO);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glDrawElements(GL_TRIANGLES, this->mNumElements, GL_UNSIGNED_INT, 0);


    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void CShape::FreeMemory() {

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &UVO);
    glDeleteVertexArrays(1, &VAO);
    free(this->vertices);
    free(this->indices);
    free(this->uv);
    //free(this->path);
}

glm::mat4 CShape::getModel()
{
    return this->model;
}

void CShape::translateX(float x) {
    this->model = glm::translate(this->model, glm::vec3(x, 0.0, 0.0));
}

void CShape::rotateZ(float x) {
    this->model = glm::rotate(this->model, x, glm::vec3(0.f, 0.f, 1.f));
}

void CShape::rotateY(float y) {
    this->model = glm::rotate(this->model, y, glm::vec3(0.f, 1.f, 0.f));
}

void CShape::scale(float s) {
    this->model = glm::scale(this->model, glm::vec3(s, s, s));
}
