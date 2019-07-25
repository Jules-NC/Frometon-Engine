#include "SubShape.h"

#ifndef STB_I
#define STB_I
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

#include <iostream>
#define GLFW_INCLUDE_NONE
#include "imgui.h"


SubShape::SubShape(aiMesh * mesh, SubTexture * texture){
    this->texture = texture;

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
}


void SubShape::draw(){
    this->texture->bindTexture();
    glBindVertexArray(this->VAO);
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

void SubShape::freeThis(){
    std::free(this->indices);
    std::free(this->uv);
}

void SubShape::showGUI(){
    ImGui::Text("Vertices %d", this->numVertices);
}
