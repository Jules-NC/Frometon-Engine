#include <string>
#include <assimp/Importer.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/scene.h>           // Output data structure
#include "SubShape.h"
#include <vector>

class MShape{
public:
    MShape()=default;
    void load(std::string pathToFile);
    void draw();

private:
    Assimp::Importer importer;
    const aiScene * scene;
    void loadNode(aiNode * node);
    void loadMesh(unsigned int i);

    unsigned int numMeshes;
    std::vector<SubShape *> shapes;
};
