#include "Entity.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include <iostream>

#include "VAO.hpp"

Entity::Entity(std::string name, Mesh& mesh) : name(name), mesh(mesh) {

}

glm::mat4 Entity::getMatrix() const {
    // glm::mat4 trans = glm::mat4(1.0f);
    // // trans = glm::toMat4(rotation) * trans;
    // trans = glm::translate(trans, position);
    // // trans = glm::scale(trans, scale);  
    // return trans;

    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot = glm::toMat4(rotation); 
    glm::mat4 rot_inv = glm::inverse(rot); 
    trans = glm::translate(trans, position) * rot_inv;
    trans = glm::scale(trans, scale);  
    return trans;
}

void Entity::setPosition(const glm::vec3 _position) {
    this->position = _position;
}

glm::vec3& Entity::getPosition() {
    return position;
}

void Entity::setRotation(const glm::quat _rotation) {
    this->rotation = _rotation;
}

glm::quat& Entity::getRotation() {
    return rotation;
}

void Entity::setScale(const glm::vec3 _scale) {
    this->scale = _scale;
}

glm::vec3& Entity::getScale() {
    return scale;
}

void Entity::draw(Shader& shader, Camera& camera) const {
    // set the transformation matrix
    shader.use();
    shader.setMat4x4("model", getMatrix());
    mesh.draw(shader, camera);

}

void Entity::draw(Shader& shader, Camera& camera, Camera& sun_camera) const {
    // set the transformation matrix
    shader.use();
    shader.setMat4x4("model", getMatrix());
    shader.setMat4x4("lightSpaceMatrix", sun_camera.projection * sun_camera.getMatrix());
    glm::vec3 light_dir = glm::vec3(0.0f, 0.0f, 1.0f) * sun_camera.rotation;
	glUniform3f(glGetUniformLocation(shader.ID, "light_dir"), light_dir[0], light_dir[1], light_dir[2]);
    mesh.draw(shader, camera);
}