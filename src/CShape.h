#pragma once
//#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#ifndef GLAD_I
#define GLAD_I
#include <glad/glad.h>
#endif

#include <vector>
#include <glm/gtc/matrix_transform.hpp>

#ifndef ASSIMP_I
#define ASSIMP_I
#include <assimp/scene.h>
#endif



class CShape {
public:
    CShape();
    ~CShape() = default;

    void init(const char * pathobj, const char * path, int pIndex);

    void setup();
    void draw();
    void FreeMemory();

    glm::mat4 getModel();
    void translateX(float x);
    void rotateZ(float x);
    void rotateY(float y);
    void scale(float s);

    const aiScene * scene;
    static std::vector<CShape*> list;

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint UVO;
    GLuint TextureID;

    aiVector3D * vertices;
    GLuint * indices;
    GLfloat * uv;
    const char * path;

    int mNumElements;

    glm::mat4 model = glm::mat4(1.0f);
};
