#define GLFW_INCLUDE_NONE
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <assimp/scene.h>
#include <string>

class SubShape{
public:
    SubShape(aiMesh * mesh, std::string texturePath);
    void draw();
    void denit();

private:
    void initTexture();
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint UVO;
    GLuint TextureID;

    int numVertices;
    int numIndices;
    int numUV;
    int numFaces;

    aiVector3D * vertices;
    GLuint * indices;
    GLfloat * uv;

    glm::mat4 model = glm::mat4(1.0f);
    std::string path;
};