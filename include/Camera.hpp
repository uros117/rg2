#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class Camera {
public:
    glm::vec3 position = glm::vec3();
    glm::quat rotation = glm::quat();
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 16.0f / 10.0f, 0.1f, 30.0f);

    Camera();

    glm::mat4 getMatrix();
};

#endif