#ifndef __EBO_HPP__
#define __EBO_HPP__

#include "glad/gl.h"
#include <vector>


class EBO {
public:
    GLuint id;

    EBO(std::vector<GLuint>& indices);

    void bind();

    void unbind();

    void del();

};


#endif
