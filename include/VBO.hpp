#ifndef __VBO_HPP__
#define __VBO_HPP__
#include <vector>
#include <iostream>
#include "glad/gl.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"



struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 uv;

    inline friend std::ostream& operator<<(std::ostream& os, const Vertex& v) {
        os << "Vertex {" 
        << glm::to_string(v.position) 
        << glm::to_string(v.normal) 
        << glm::to_string(v.color) 
        << glm::to_string(v.uv) << "}";
        return os;
    };
};

struct UIVertex {
    glm::vec2 position;
    glm::vec3 color;
    glm::vec2 uv;

    inline friend std::ostream& operator<<(std::ostream& os, const UIVertex& v) {
        os << "UIVertex {" 
        << glm::to_string(v.position) 
        << glm::to_string(v.color) 
        << glm::to_string(v.uv) << "}";
        return os;
    };
};


template <class V> class VBO {
public:
    GLuint id;

    VBO(std::vector<V>& vertices) {
        glGenBuffers(1, &id);
        glBindBuffer(GL_ARRAY_BUFFER, id);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(V), vertices.data(), GL_STATIC_DRAW);
    }

    void linkAttrib(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
        this->bind();
        glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
        glEnableVertexAttribArray(layout);
        this->unbind();
    }

    void linkAttributes() {
    }

    void linkVBO(VBO& vbo, GLuint layout) {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }


    void bind() {
        glBindBuffer(GL_ARRAY_BUFFER, id);
    }


    void unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }


    void del() {
        glDeleteBuffers(1, &id);
    }

};



#endif