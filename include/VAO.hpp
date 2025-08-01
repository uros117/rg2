#ifndef __VAO_HPP__
#define __VAO_HPP__
#include "VBO.hpp"


class VAO {
public:
    GLuint id;

    VAO();

    void bind() const;

    void unbind() const;

    void del();

};

#endif