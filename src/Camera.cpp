#include "Camera.hpp"

Camera::Camera() {

}

glm::mat4 Camera::getMatrix() {
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot = glm::toMat4(rotation); 
    glm::mat4 rot_inv = glm::inverse(rot); 
    trans = glm::translate(trans, position) * rot_inv;
    trans = glm::scale(trans, scale);  
    trans = glm::inverse(trans);
    return trans;
}