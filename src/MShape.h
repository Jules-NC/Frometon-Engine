#include <string>
#include <assimp/Importer.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/scene.h>           // Output data structure
#include "SubShape.h"
#include <vector>
#include "SubTexture.h"

class MShape{
public:
    MShape()=default;
    void load(std::string pathToFile);
    void draw();
    void free();

private:
    Assimp::Importer importer;
    const aiScene * scene;
    void loadNode(aiNode * node);
    void loadMesh(unsigned int i);
    void loadTexture(aiMaterial * material);

    unsigned int numMeshes;
    std::vector<SubShape> shapes;
    std::vector<SubTexture> textures;
};
