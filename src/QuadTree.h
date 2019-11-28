#include <iostream>
#include <memory> // for std::unique_ptr
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include <cmath>
#include <random>
#include <omp.h>

#include "PerlinNoise.hpp"

struct XYZ
{
    float x;
    float y;
    float z;

    XYZ() : x(0), y(0), z(0) {}
    XYZ(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    XYZ(const XYZ& other): x(other.x), y(other.y), z(other.z) {}
    XYZ& operator=(const XYZ& other);
    XYZ operator+(const XYZ other);

    XYZ operator-(const XYZ other);
    XYZ& operator+=(const XYZ & other);
    XYZ& operator-=(const XYZ & other);


};

struct AABB{
    XYZ center;
    float halfSize;

    AABB() : center(XYZ(0, 0, 0)), halfSize(0.f) {}
    AABB(XYZ _center, float _halfSize) : center(_center), halfSize(_halfSize) {}
    AABB(const AABB& other);
    AABB& operator=(const AABB& other);

    bool containsPoint(XYZ point);
};

struct QuadTree{
    const static int MAX_HEIGHT = 15;
    std::unique_ptr<QuadTree> subTrees[4];

    AABB boundary;
    bool isLeaf;
    int height;

    std::unique_ptr<int> resource;

    QuadTree() : boundary(AABB()), isLeaf(true), height(0) {}
    QuadTree(AABB _boundary) : boundary(_boundary), isLeaf(true), height(0) {}
    QuadTree(AABB _boundary, int _height) : boundary(_boundary), isLeaf(true), height(_height) {}
    QuadTree(const QuadTree& other);
    QuadTree& operator =(const QuadTree& other);

    void subdivise();
    void insert(XYZ point);
    int maxHeight();
    void draw(GLuint _matrixID, glm::mat4 _MV, GLuint _VAO_ID);
    void deleteObj(){
        if(isLeaf){
            for(int i=0; i<4; ++i){
                subTrees[0].reset();
            }
            return;
        }
        for(int i=0; i<4; ++i)
            subTrees[i].get()->deleteObj();
    }
};


QuadTree::QuadTree(const QuadTree& other){
    boundary = other.boundary;
    isLeaf = other.isLeaf;
    //TODO: ressource
}

QuadTree& QuadTree::operator =(const QuadTree& other){
    if(this != &other){
        boundary = other.boundary;
        isLeaf = other.isLeaf;
    }
    return *this;
}


AABB::AABB(const AABB& other){
    center = other.center;
    halfSize = other.halfSize;
}

AABB& AABB::operator=(const AABB& other){
    if(this != &other){
        center = other.center;
        halfSize = other.halfSize;
    }
    return *this;
}

bool AABB::containsPoint(XYZ point){
    float scale = 20;
    float x_size_squared = std::pow(center.x - point.x, 2);
    float y_size_squared = std::pow(center.y - point.y, 2);
    float z_size_squared = std::pow(center.z - point.z, 2);

    float dist = std::sqrt(x_size_squared + y_size_squared + z_size_squared/50);
    bool contains = dist < halfSize*scale;
}


XYZ& XYZ::operator =(const XYZ& other){
    if(this != &other){
        x = other.x;
        y = other.y;
        z = other.z;
    }
    return *this;
}

XYZ XYZ::operator-(const XYZ other){
    return XYZ(x-other.x, y-other.y, z-other.z);
}

XYZ& XYZ::operator +=(const XYZ & other){
    x += other.x;
    y += other.y;
    z += other.z;
    return *this;
}

XYZ& XYZ::operator -=(const XYZ & other){
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return *this;
}

XYZ XYZ::operator+(const XYZ other){
    return XYZ(x+other.x, y+other.y, z+other.z);
}


void QuadTree::subdivise(){
    if(height == MAX_HEIGHT){
        return;
    }

    isLeaf = false;

    XYZ center = boundary.center;
    float z = center.z;
    float next_halfsize = boundary.halfSize/2;
    XYZ nw = center + XYZ(-next_halfsize, next_halfsize, z);
    XYZ ne = center + XYZ(next_halfsize, next_halfsize, z);
    XYZ sw = center + XYZ(-next_halfsize, -next_halfsize, z);
    XYZ se = center + XYZ(next_halfsize, -next_halfsize, z);

    //std::cout << "Center:\t" << center.x << ", " << center.y << ", " << i_ << std::endl;

    subTrees[0] = std::unique_ptr<QuadTree>(new QuadTree(AABB(nw, next_halfsize), height+1));
    subTrees[1] = std::unique_ptr<QuadTree>(new QuadTree(AABB(ne, next_halfsize), height+1));
    subTrees[2] = std::unique_ptr<QuadTree>(new QuadTree(AABB(sw, next_halfsize), height+1));
    subTrees[3] = std::unique_ptr<QuadTree>(new QuadTree(AABB(se, next_halfsize), height+1));
}

void QuadTree::insert(XYZ point){
    // If point not in Quad -> out
    if(!boundary.containsPoint(point))
        return;

    // If we can't go further -> out
    if(height == MAX_HEIGHT && isLeaf)
        return;

    // If Quad a leaf, subdivise and call recursively
    if(isLeaf)
        this->subdivise();

    // Now we are sure that this Quad have childs
    #pragma omp parallel for
    for(int i=0; i<4; ++i){
        subTrees[i].get()->insert(point);
    }
}

int QuadTree::maxHeight(){
    // if leaf return value
    if(isLeaf)
        return height;

    // else
    int _height = -1;
    for(int i=0; i<4; ++i){
        int t_height = subTrees[i].get()->height;
        if(t_height > _height)
            _height = t_height;
    }

    return _height;
}

float getZsphere(float x, float y, float radius){
    return std::sqrt(std::pow(radius, 2) - std::pow(y, 2) - std::pow(x, 2));
}

float noiseGenerator(float x, float y){
    //const siv::PerlinNoise noisifier(1);
    return noise(x/30, y/30)*5;

}

void QuadTree::draw(GLuint _matrixID, glm::mat4 _MV, GLuint _VAO_ID){
    if(!isLeaf){

        #pragma omp parallel for
        for(int i=0; i<4; ++i){
            subTrees[i].get()->draw(_matrixID, _MV, _VAO_ID);
        }
        return;
    }

    glBindVertexArray(_VAO_ID);


    // An array of 3 vectors which represents 3 vertices
    //float radius = 70.71f;
    //float half = boundary.halfSize;
    /*
    float mp = getZsphere(boundary.center.x-half, boundary.center.y+half, radius);
    float mm = getZsphere(boundary.center.x-half, boundary.center.y-half, radius);
    float pm = getZsphere(boundary.center.x+half, boundary.center.y-half, radius);
    float pp = getZsphere(boundary.center.x+half, boundary.center.y+half, radius);
*/

    /*
    float mp = noiseGenerator(boundary.center.x-half, boundary.center.y+half);
    float mm = noiseGenerator(boundary.center.x-half, boundary.center.y-half);
    float pm = noiseGenerator(boundary.center.x+half, boundary.center.y-half);
    float pp = noiseGenerator(boundary.center.x+half, boundary.center.y+half);
*/
    const GLfloat g_vertex_buffer_data[] = {
        -1.f, +1.f, 0,
        +1.f, -1.f, 0,
        +1.f,  +1.f, 0,

        -1.f, +1.f, 0,
        -1.f, -1.f, 0,
        +1.f, -1.f, 0,
    };

    // This will identify our vertex buffer
    GLuint vertexbuffer;

    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_DYNAMIC_DRAW);


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0,(void*)0);

    glm::mat4 modelMatrix(1.0);
    XYZ center = boundary.center;
    float scale = boundary.halfSize;

    modelMatrix = glm::translate(modelMatrix, glm::vec3(center.x, center.y, center.z));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, 1));
    glm::mat4 MVP = _MV * modelMatrix;
    glUniformMatrix4fv(_matrixID, 1, GL_FALSE, &MVP[0][0]);


    glDrawArrays(GL_TRIANGLES, 0, 6); // Starting from vertex 0; 3 vertices total -> 1 triangle

    glDisableVertexAttribArray(0);
    glBindVertexArray(0);

    glDeleteBuffers(1, &vertexbuffer);
}
