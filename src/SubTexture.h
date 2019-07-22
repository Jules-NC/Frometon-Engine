#pragma once
#include <assimp/scene.h>
#include <glad/glad.h>


class SubTexture{
public:
    SubTexture(aiString texturePath);
    void bindTexture();

private:
    GLuint TextureID;
    aiString path;
};
