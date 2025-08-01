#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__
#include <string>
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "Shader.hpp"
#include "VAO.hpp"
#include "GLFW/glfw3.h"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "glm/gtx/quaternion.hpp"

class Entity {
public:
    std::string name;

    glm::vec3 position = glm::vec3();
    glm::quat rotation = glm::quat();
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    
    Mesh mesh;
    
    Entity(std::string name, Mesh& mesh);
    
    void setPosition(const glm::vec3 _position);
    glm::vec3& getPosition();

    void setRotation(const glm::quat _rotation);
    glm::quat& getRotation();

    void setScale(const glm::vec3 _scale);
    glm::vec3& getScale();

    glm::mat4 getMatrix() const;

    void draw(Shader& shader, Camera& camera) const;

    void draw(Shader& shader, Camera& camera, Camera& sun_camera) const;
    
};

#endif