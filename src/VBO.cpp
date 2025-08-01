#include <vector>
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include "VBO.hpp"

template <> void VBO<Vertex>::linkAttributes() {
    linkAttrib(0, 3, GL_FLOAT, sizeof(Vertex), (void*) 0);// position
    linkAttrib(1, 3, GL_FLOAT, sizeof(Vertex), (void*) (3 * sizeof(float)));// normal
    linkAttrib(2, 3, GL_FLOAT, sizeof(Vertex), (void*) (6 * sizeof(float)));// color
    linkAttrib(3, 2, GL_FLOAT, sizeof(Vertex), (void*) (9 * sizeof(float)));// uv
};

template <> void VBO<UIVertex>::linkAttributes() {
    linkAttrib(0, 3, GL_FLOAT, sizeof(UIVertex), (void*) 0);// position
    linkAttrib(1, 3, GL_FLOAT, sizeof(UIVertex), (void*) (2 * sizeof(float)));// normal
    linkAttrib(2, 3, GL_FLOAT, sizeof(UIVertex), (void*) (5 * sizeof(float)));// color
};