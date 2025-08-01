#ifndef __OBJLOADER_HPP__
#define __OBJLOADER_HPP__
#include "glad/gl.h"
#include "GLFW/glfw3.h"
#include <fstream>
#include <string>
#include <sstream>
#include "Mesh.hpp"


class ObjLoader {
public:

    static Mesh load(const char* file_path);
};

#endif