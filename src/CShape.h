#pragma once
//#include <GL/glew.h>
#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION

struct Vertex {
    glm::vec3 Vertices;
    glm::vec2 UVs;
    glm::vec3 Normals;
};

class CShape {
public:
    CShape();
    ~CShape() = default;

    void init(const char * pathobj, const char * path);

    void setup();
    void draw();
    void FreeMemory();

    glm::mat4 getModel();
    void translateX(float x);
    void rotateZ(float x);
    void rotateY(float y);
    void scale(float s);

private:
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint UVO;
    GLuint TextureID;

    GLfloat * vertices;
    GLuint * indices;
    GLfloat * uv;
    const char * path;

    int mNumElements;

    glm::mat4 model = glm::mat4(1.0f);
};
