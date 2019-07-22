#include "SubShape.h"

#ifndef STB_I
#define STB_I
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <iostream>
#define GLFW_INCLUDE_NONE


SubShape::SubShape(aiMesh * mesh, aiString texturePath){
    this->path = texturePath;

    this->numVertices = mesh->mNumVertices;
    this->numIndices = mesh->mNumFaces * 3;
    this->numUV = this->numVertices*2;
    this->numFaces = mesh->mNumFaces;

    //std::cerr << "\tLoading vertices" << std::endl;
    this->vertices = mesh->mVertices;

    //std::cerr << "\tLoading indices" << std::endl;
    this->indices = (unsigned int *)malloc(numIndices*sizeof(unsigned int));

    int j = 0;
    for (int i = 0; i < mesh->mNumFaces; i++) {
        GLuint * indices = mesh->mFaces[i].mIndices;
        this->indices[j] = indices[0];
        this->indices[j + 1] = indices[1];
        this->indices[j + 2] = indices[2];
        j += 3;
    }

    //std::cerr << "\tLoading UVs" << std::endl;
    this->uv = (float *)malloc(this->numVertices * sizeof(float)*2);

    j = 0;
    if (mesh->HasTextureCoords(0)){
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            this->uv[j] = mesh->mTextureCoords[0][i].x;
            this->uv[j + 1] = mesh->mTextureCoords[0][i].y;
            j += 2;
        }
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
    std::string pathbase = "../../res/";
    std::string subpath = this->path.C_Str();
    std::replace(subpath.begin(), subpath.end(), '\\', '/');
    std::string fullPath = pathbase + subpath;
    unsigned char *data = stbi_load(fullPath.c_str(), &width, &height, &nrChannels, 0);
    //unsigned char *data = stbi_load("../../res/TextureDefault.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        switch (nrChannels) {
        case 3:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            break;
        case 4:
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            break;
        default:
            break;
        }

        glGenerateMipmap(GL_TEXTURE_2D);
        //std::cout << "\tLoad image SUCCESS" << std::endl;
        //std::cout << "\tImage size : " << width << ", " << height << std::endl;
    }
    else {
        std::cout << "\tFAILED TO LOAD TEXTURE: " << fullPath.c_str() << std::endl;
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
