#include "MShape.h"
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include "imgui.h"
#include <iostream>


void MShape::load(std::string pathToFile){
    this->shapes = std::vector<SubShape>();
    this->modelMatrix = glm::mat4(1.0);
    this->position = glm::vec3(0, 0, 0);
     this->scale = glm::vec3(0.05, 0.05, 0.05);
    this->rotationAngle = glm::vec3(0.f, 1.f, 0.f);
    this->rotation = 0.0f;


    std::cerr << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
    std::cerr << "[Begin loading of new mesh]" << std::endl;

    this->scene = importer.ReadFile(pathToFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenUVCoords|aiProcess_TransformUVCoords | aiProcess_FindInvalidData);
    if (!scene) {
        fprintf(stderr, "%s", importer.GetErrorString());
        getchar();
        return;
    }
    std::cerr << "N° SHAPES: " << scene->mNumMeshes << "\n";
    if (scene->HasMaterials())
    {
        for(int i=0; i<scene->mNumMaterials; i++){
            this->loadTexture(scene->mMaterials[i]);
        }
    }

    aiNode * rootNode = scene->mRootNode;
    this->loadNode(rootNode);

    std::cerr << "[End]" << std::endl;
    std::cerr << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

}


void MShape::loadTexture(aiMaterial * material){
    aiString texturePath;
    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0 && material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
    {
        SubTexture stext  =SubTexture(texturePath);
        this->textures.push_back(stext);
    }
    else{
        SubTexture stext  =SubTexture(aiString("TextureDefault.jpg"));
        this->textures.push_back(stext);


        std::cerr <<"\nNo diffuse texture was found..." << std::endl;
    }
}


void MShape::loadNode(aiNode * node)
{
    for(int i=0; i<node->mNumMeshes; ++i)
    {
        this->loadMesh(node->mMeshes[i]);
    }

    for(int i=0; i<node->mNumChildren; ++i)
    {
        //std::cerr << "\nLoad node Children " << i+1 << "/" << node->mNumChildren<< " node n0: " << node->mChildren[i] << std::endl;
        this->loadNode(node->mChildren[i]);
    }
}


void MShape::loadMesh(unsigned int i)
{
    aiMesh * vMesh = this->scene->mMeshes[i];
    SubShape sb = SubShape(vMesh, &this->textures[vMesh->mMaterialIndex]);

    this->shapes.push_back(sb);
}


const glm::mat4 MShape::getModelMatrix(){
    this->modelMatrix = glm::mat4(1.0);
    this->modelMatrix = glm::translate(this->modelMatrix, this->position);

    if (rotationAngle[0] + rotationAngle[1] + rotationAngle[2] != 0){
        this->modelMatrix = glm::rotate(this->modelMatrix, this->rotation, this->rotationAngle);
    }

    this->modelMatrix = glm::scale(this->modelMatrix, this->scale);
    return this->modelMatrix;
}


void MShape::draw(){
    //#pragma omp parallel for
    for(int i=0; i<this->shapes.size(); ++i){
        shapes[i].draw();
    }
}

void MShape::free(){
    std::cerr << "N SHAPES: " << this->shapes.size() << "\n";
    for(int i=0; i<this->shapes.size(); ++i){
        (&this->shapes[i])->freeThis();
    }

    for(int i=0; i<this->textures.size(); ++i){
        this->textures[i].free();
    }
}

void MShape::showGUI(){
    if(ImGui::TreeNode("Mesh:")){
        ImGui::SliderFloat("trans x", &this->position[0], -100, 100);
        ImGui::SliderFloat("trans y", &this->position[1], -100, 100);
        ImGui::SliderFloat("trans z", &this->position[2], -100, 100);

        ImGui::Spacing();
        ImGui::SliderFloat("rotation", &this->rotation, -1.58, 1.58);
        ImGui::SliderFloat("rot x", &this->rotationAngle[0], -1, 1);
        ImGui::SliderFloat("rot y", &this->rotationAngle[1], -1, 1);
        ImGui::SliderFloat("rot z", &this->rotationAngle[2], -1, 1);

        ImGui::Spacing();
        ImGui::SliderFloat("scal x", &this->scale[0], -1, 1);
        ImGui::SliderFloat("scal y", &this->scale[1], -1, 1);
        ImGui::SliderFloat("scal z", &this->scale[2], -1, 1);


        ImGui::TreePop();
    }

    if(ImGui::TreeNode("Textures:")){
        for (int i = 0; i<this->textures.size(); i++)
        {
            const char * path = (*this->textures[i].getPath()).C_Str();
            if (ImGui::TreeNode((void*)(intptr_t)i, "%s", path))
            {
                this->textures[i].showGUI();
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }

    if(ImGui::TreeNode("Meshes:")){
        for (int i = 0; i<this->shapes.size(); i++)
        {
            if (ImGui::TreeNode((void*)(intptr_t)i, "Mesh %d", i))
            {
                this->shapes[i].showGUI();
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
}
