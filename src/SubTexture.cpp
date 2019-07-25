#include "SubTexture.h"
#include "stb_image.h"
#include <iostream>
#include <imgui.h>

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

    this->width = width;
    this->height = height;
    this->nChannels = nrChannels;
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


void normalize(float * x, float * y){
    if(*x>300){
        float ratio = 300/ *x;
        *x = *x * ratio;
        std::cerr << "x: " << *x << std::endl;
        std::cerr << "y: " << *y << std::endl;

        *y = *y * ratio;
    }
}

void SubTexture::showGUI(){
    ImGui::Text("Channels %d", this->nChannels);
    ImGui::Text("Width %d", this->width);
    ImGui::Text("height %d", this->height);

    ImGui::Text("%.0fx%.0f", width, height);

    static float x = width;
    static float y = height;
    normalize(&x, &y);

    ImGui::Image((void*)(intptr_t)this->TextureID, ImVec2(x, y));
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 pos = ImGui::GetCursorScreenPos();
    if (ImGui::IsItemHovered())
         {
             ImGui::BeginTooltip();
             float region_sz = 32.0f;
             float region_x = io.MousePos.x - pos.x - region_sz * 0.5f; if (region_x < 0.0f) region_x = 0.0f; else if (region_x > width - region_sz) region_x = width - region_sz;
             float region_y = io.MousePos.y - pos.y - region_sz * 0.5f; if (region_y < 0.0f) region_y = 0.0f; else if (region_y > height - region_sz) region_y = height - region_sz;
             float zoom = 4.f;
             ImGui::Text("Min: (%.2f, %.2f)", region_x, region_y);
             ImGui::Text("Max: (%.2f, %.2f)", region_x + region_sz, region_y + region_sz);
             ImVec2 uv0 = ImVec2((region_x) / width, (region_y) / height);
             ImVec2 uv1 = ImVec2((region_x + region_sz) / width, (region_y + region_sz) / height);
             ImGui::Image((void*)(intptr_t)this->TextureID, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
             ImGui::EndTooltip();
         }
}
