#include "Mesh.hpp"
#include "VBO.hpp"
#include "EBO.hpp"
#include <string>
#include <vector>

Mesh::Mesh(std::vector <Vertex> vertices, std::vector <GLuint> indices/*, std::vector <Texture> textures*/) : vertices(vertices), indices(indices) {
    vao.bind();

    VBO<Vertex> vbo(vertices);
    EBO ebo(indices);

    // TODO: Uraditi na pametniji nacin, mozda reflection
    vbo.linkAttributes();

    vao.unbind();
    vbo.unbind();
    ebo.unbind();
}

Mesh Mesh::Quad(float width, float height, glm::vec3 color) {
    std::vector<Vertex> vertecies = {
        (Vertex){
            {0.0f, 0.0f, 0.0f}, // position
            {0.0f, 0.0f, 1.0f}, // normal
            color,
            {0.0f, 0.0f}  // uv
        },
        (Vertex){
            {width, 0.0f, 0.0f}, // position
            {0.0f, 0.0f, 1.0f}, // normal
            color,
            {1.0f, 0.0f}  // uv
        },
        (Vertex){
            {0.0f, height, 0.0f}, // position
            {0.0f, 0.0f, 1.0f}, // normal
            color,
            {0.0f, 1.0f}  // uv
        },
        (Vertex){
            {width, height, 0.0f}, // position
            {0.0f, 0.0f, 1.0f}, // normal
            color,
            {1.0f, 1.0f}  // uv
        }
    };

    std::vector<GLuint> indices = {
        0, 3, 2,
        0, 1, 3,
    };
    return Mesh(vertecies, indices);
}

std::vector<PointLight> lights = {
    {
        {-5.0f, 4.0f, 2.0f},
        1.0f,
        0.05f,
        0.05f,
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f}
    },
    {
        {0.0f, 2.5f, -2.5f},
        1.0f,
        0.2f,
        0.2f,
        {0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 0.0f, 0.0f}
    }
    };

void Mesh::draw(Shader& shader, Camera& camera) const {
    shader.use();
    shader.setMat4x4("projection", camera.projection);
    shader.setMat4x4("view", camera.getMatrix());
	glUniform3f(glGetUniformLocation(shader.ID, "view_position"), camera.position[0], camera.position[1], camera.position[2]);

    int i = 0;
    for (const Texture& tex: textures)
    {
        tex.bind();
        tex.texUnit(shader, tex.type);
        i++;
    }

    

    for (int i = 0; i < lights.size(); i++) {
	    glUniform3f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].position").c_str()), lights[i].position[0], lights[i].position[1], lights[i].position[2]);

	    glUniform1f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].constant").c_str()), lights[i].constant);
	    glUniform1f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].linear").c_str()), lights[i].linear);
	    glUniform1f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].quadratic").c_str()), lights[i].quadratic);
        
	    glUniform3f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].ambient").c_str()), lights[i].ambient[0], lights[i].ambient[1], lights[i].ambient[2]);
	    glUniform3f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].diffuse").c_str()), lights[i].diffuse[0], lights[i].diffuse[1], lights[i].diffuse[2]);
	    glUniform3f(glGetUniformLocation(shader.ID, ("lights[" + std::to_string(i) + "].specular").c_str()), lights[i].specular[0], lights[i].specular[1], lights[i].specular[2]);
    }
    
    vao.bind();
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);
    vao.unbind();
    // shader.unbind();
}
