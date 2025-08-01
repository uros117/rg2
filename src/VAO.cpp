#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "VAO.hpp"

VAO::VAO() {
    glGenVertexArrays(1, &id);
}

void VAO::bind() const {
    glBindVertexArray(id);
}

void VAO::unbind() const {
    glBindVertexArray(0);
}

void VAO::del() {
    glDeleteVertexArrays(1, &id);
}