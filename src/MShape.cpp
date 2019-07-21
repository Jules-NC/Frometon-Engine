#include "MShape.h"
#include <assimp/postprocess.h>
#include <glad/glad.h>

#include <iostream>

void MShape::load(std::string pathToFile){
    this->shapes = std::vector<SubShape>();
    this->texturesPaths = std::vector<aiString>();


    std::cerr << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

    std::cerr << "[Begin loading of new mesh]" << std::endl;

    this->scene = importer.ReadFile(pathToFile, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenUVCoords|aiProcess_TransformUVCoords | aiProcess_FindInvalidData);
    if (!scene) {
        fprintf(stderr, "%s", importer.GetErrorString());
        getchar();
        return;
    }

    if (scene->HasMaterials())
    {
        for (unsigned int i = 0; i < scene->mNumMaterials; i++)
        {
            const aiMaterial* material = scene->mMaterials[i];
            aiString texturePath;

            unsigned int numTextures= material->GetTextureCount(aiTextureType_DIFFUSE);   // always 0
            if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0 && material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
            {
                this->texturesPaths.push_back(texturePath);
            }
        }
    }

    aiNode * rootNode = scene->mRootNode;
    this->loadNode(rootNode);
    std::cerr << "[End]" << std::endl;

    std::cerr << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

}


void MShape::loadNode(aiNode * node)
{
    #pragma omp parallel for
    for(int i=0; i<node->mNumMeshes; ++i)
    {
        this->loadMesh(node->mMeshes[i]);
    }

    #pragma omp parallel for
    for(int i=0; i<node->mNumChildren; ++i)
    {
        std::cerr << "Load node Children " << i+1 << "/" << node->mNumChildren<< " node n0: " << node->mChildren[i] << std::endl;
        this->loadNode(node->mChildren[i]);
    }
}


void MShape::loadMesh(unsigned int i)
{
    aiMesh * vMesh = this->scene->mMeshes[i];
    std::cerr << "Mesh n°" << i << " want to be loaded with texture: " << this->texturesPaths[vMesh->mMaterialIndex].C_Str() << std::endl;
    SubShape sb = SubShape(vMesh, this->texturesPaths[vMesh->mMaterialIndex]);
    this->shapes.push_back(sb);
}


void MShape::draw(){
    #pragma omp parallel for
    for(int i=0; i<this->shapes.size(); ++i){
            shapes[i].draw();
        }
}
