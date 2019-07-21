#include "SubShape.h"

#ifndef STB_I
#define STB_I
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <iostream>
#define GLFW_INCLUDE_NONE


SubShape::SubShape(aiMesh * mesh, std::string texturePath){
    this->path = texturePath;

    this->numVertices = mesh->mNumVertices;
    this->numIndices = mesh->mNumFaces * 3;
    this->numUV = this->numVertices*2;
    this->numFaces = mesh->mNumFaces;

    this->vertices = mesh->mVertices;

    this->indices = (unsigned int *)malloc(numIndices*sizeof(unsigned int));

    int j = 0;
    for (int i = 0; i < mesh->mNumFaces; i++) {
        GLuint * indices = mesh->mFaces[i].mIndices;
        this->indices[j] = indices[0];
        this->indices[j + 1] = indices[1];
        this->indices[j + 2] = indices[2];
        j += 3;
    }

    this->uv = (float *)malloc(this->numVertices * sizeof(float)*2);

    j = 0;
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {

        this->uv[j] = mesh->mTextureCoords[0][i].x;
        this->uv[j + 1] = mesh->mTextureCoords[0][i].y;
        j += 2;
    }

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

    this->initTexture();
}


void SubShape::initTexture(){
    glGenTextures(1, &this->TextureID);
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(this->path.c_str(), &width, &height, &nrChannels, 0);
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

void SubShape::draw(){
    glBindTexture(GL_TEXTURE_2D, this->TextureID);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, UVO);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glDrawElements(GL_TRIANGLES, this->numFaces*3, GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}
