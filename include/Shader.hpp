#ifndef SHADER_HPP
#define SHADER_HPP

#include <vector>
#include <string>
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

struct PointLight {
    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

class Shader {
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void use();
    void unbind();

    void setBool(const std::string& name, bool val);
    void setInt(const std::string& name, int val);
    void setFloat(const std::string& name, float val);
    void setMat4x4(const std::string& name, glm::mat4 val);
};

#endif