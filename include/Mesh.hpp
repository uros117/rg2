#ifndef __MESH_HPP__
#define __MESH_HPP__
#include <vector>
#include "glad/gl.h"
#include "VAO.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Texture.hpp"


class Mesh {
public:
    std::vector <Vertex> vertices;
    std::vector <GLuint> indices;
    std::vector <Texture> textures;
    VAO vao;

    Mesh(std::vector <Vertex> vertices, std::vector <GLuint> indices/*, std::vector <Texture> textures*/);
    static Mesh Quad(float width, float height, glm::vec3 color);

    void draw(Shader& shader, Camera& camera) const;

};

#endif