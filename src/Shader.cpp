#include "Shader.hpp"
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    char * vert_src, * frag_src;
    try
    {
        std::ifstream vert(vertexPath);
        std::string vert_buffer((std::istreambuf_iterator<char>(vert)),
                    std::istreambuf_iterator<char>());
        vert_src = new char [vert_buffer.length()+1];
        std::strcpy (vert_src, vert_buffer.c_str());
        vert.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << "ERROR: Failed to load vertex shader (" << vertexPath << "). " << e.what() << '\n';
    }
    
    
    try
    {
        std::ifstream frag(fragmentPath);
        std::string frag_buffer((std::istreambuf_iterator<char>(frag)),
                    std::istreambuf_iterator<char>());
        frag_src = new char [frag_buffer.length()+1];
        std::strcpy (frag_src, frag_buffer.c_str());
        frag.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << "ERROR: Failed to load vertex shader (" << fragmentPath << "). " << e.what() << '\n';
    }

    int status;
    // Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vert_src, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    if(!status) {
        char err[1024];
        glGetShaderInfoLog(vertexShader, 1024, NULL, err);
        std::cout << "\033[31mERROR:\033[0m failed to compile vertex shader: " << err << std::endl;
    }

	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &frag_src, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
    if(!status) {
        char err[1024];
        glGetShaderInfoLog(fragmentShader, 1024, NULL, err);
        std::cout << "\033[31mERROR:\033[0m failed to compile fragment shader: " << err << std::endl;
    }

	// Create Shader Program Object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);
    glGetShaderiv(shaderProgram, GL_LINK_STATUS, &status);
    if(!status) {
        char err[1024];
        glGetShaderInfoLog(shaderProgram, 1024, NULL, err);
        std::cout << "\033[31mERROR:\033[0m failed to link shader: " << err << std::endl;
    }

    
    

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

    this->ID = shaderProgram;
}

void Shader::use() {
	glUseProgram(this->ID);
}

void Shader::unbind() {
	glUseProgram(0);
}

void Shader::setBool(const std::string& name, bool val) {
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)val);
}
void Shader::setInt(const std::string& name, int val) {
	glUniform1i(glGetUniformLocation(this->ID, name.c_str()), val);
}
void Shader::setFloat(const std::string& name, float val) {
	glUniform1f(glGetUniformLocation(this->ID, name.c_str()), val);
}
void Shader::setMat4x4(const std::string& name, glm::mat4 val) {
	glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

Shader::~Shader() {
	glDeleteProgram(this->ID);
}
