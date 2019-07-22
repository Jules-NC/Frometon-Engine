#include "SubTexture.h"
#include "stb_image.h"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION


SubTexture::SubTexture(aiString texturePath){
    this->path = texturePath;
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

    if (data){
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
        std::cerr << "\tLoad image " << fullPath.c_str() << ", " << nrChannels << " channels, SUCCESSFULL" << std::endl;
    }
    else {
        std::cerr << "\tLoad image " << fullPath.c_str() << ", " << nrChannels << " channels, FAILED" << std::endl;
    }
    stbi_image_free(data);
}

void SubTexture::bindTexture(){
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
}

void SubTexture::free(){
    //
}
