#include "MShape.h"
#include <assimp/postprocess.h>
#include <glad/glad.h>

#include <iostream>

void MShape::load(std::string pathToFile){
    this->shapes = std::vector<SubShape>();

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


void MShape::draw(){
    #pragma omp parallel for
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
