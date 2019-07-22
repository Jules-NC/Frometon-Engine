#pragma once
#include <assimp/scene.h>
#include <glad/glad.h>


class SubTexture{
public:
    SubTexture(aiString texturePath);
    void bindTexture();
    void free();

    void showGUI();
    const aiString * getPath(){
        return &this->path;
    }

private:
    GLuint TextureID;
    aiString path;

    int width;
    int height;
    int nChannels;
};
