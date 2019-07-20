#include "MShape.h"
#include <assimp/postprocess.h>
#include <glad/glad.h>

#include <iostream>

void MShape::load(std::string pathToFile){
    this->shapes = std::vector<SubShape *>();
    std::cerr << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

    std::cerr << "[Begin loading of new mesh]" << std::endl;

    this->scene = importer.ReadFile(pathToFile, aiProcess_Triangulate | aiProcess_FlipUVs);
    if (!scene) {
        fprintf(stderr, "%s", importer.GetErrorString());
        getchar();
        return;
    }

    aiNode * rootNode = scene->mRootNode;
    this->loadNode(rootNode);
    std::cerr << "[End]" << std::endl;

    std::cerr << "++++++++++++++++++++++++++++++++++++++++++++" << std::endl;

}


void MShape::loadNode(aiNode * node)
{
    for(int i=0; i<node->mNumMeshes; ++i)
    {
        this->loadMesh(node->mMeshes[i]);
    }

    for(int i=0; i<node->mNumChildren; ++i)
    {
        std::cerr << "Load node Children " << i+1 << "/" << node->mNumChildren<< " node n0: " << node->mChildren[i] << std::endl;
        this->loadNode(node->mChildren[i]);
    }
}


void MShape::loadMesh(unsigned int i)
{
    aiMesh * vMesh = this->scene->mMeshes[i];
    SubShape sb = SubShape(vMesh, "../../res/TextureGrid.jpg");
    std::cerr << "Mesh n°" << i << " want to be loaded" << std::endl;

}
