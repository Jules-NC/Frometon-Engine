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

    void showGUI();

    const glm::mat4 getModelMatrix();

private:
    Assimp::Importer importer;
    const aiScene * scene;
    void loadNode(aiNode * node);
    void loadMesh(unsigned int i);
    void loadTexture(aiMaterial * material);

    glm::mat4 modelMatrix;
    glm::vec3 position;
    float rotation;
    glm::vec3 rotationAngle;
    glm::vec3 scale;

    unsigned int numMeshes;
    std::vector<SubShape> shapes;
    std::vector<SubTexture> textures;
};
