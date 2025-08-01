#include "ObjLoader.hpp"
#include <iostream>
#include "Mesh.hpp"
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <tuple>
#include <map>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"


Mesh ObjLoader::load(const char *file_path)
{
    std::ifstream file(file_path);
    std::string line;
    

    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;
    std::map<std::tuple<int, int, int>, std::pair<Vertex, int>> vert_map;

    std::vector<glm::vec<3, std::tuple<int, int, int>>> faces;

    int i = 0;
    
    while(!file.eof()) {
        std::getline(file, line);

        std::stringstream line_stream(line);

        std::string c;
        line_stream >> c;

        if(!line_stream || !file) throw std::invalid_argument("Error occured while reading file!");// Error occured while reading the file

        float x, y, z;
        if(c == "v") {
            line_stream >> x >> y >> z;

            if(!line_stream || !file) throw std::invalid_argument("Error occured while reading a vertex coordinate!");   
            vertices.push_back(glm::vec3(x, y, z));
        } else if(c == "vt") {
            line_stream >> x >> y;

            if(!line_stream || !file) throw std::invalid_argument("Error occured while reading a texture coordinate!");   
            uvs.push_back(glm::vec2(x, y));
        } else if(c == "vn") {
            line_stream >> x >> y >> z;

            if(!line_stream || !file) throw std::invalid_argument("Error occured while reading a normal vector!");   
            normals.push_back(glm::vec3(x, y, z));
        } else if (c == "f") {
            int x1 = 0.0f, y1 = 0.0f, z1 = 0.0f, x2 = 0.0f, y2 = 0.0f, z2 = 0.0f, x3 = 0.0f, y3 = 0.0f, z3 = 0.0f;

            line_stream >> x1;
            line_stream.ignore();
            line_stream >> y1;
            line_stream.ignore();
            line_stream >> z1;
            //line_stream.ignore();

            line_stream >> x2;
            line_stream.ignore();
            line_stream >> y2;
            line_stream.ignore();
            line_stream >> z2;
            //line_stream.ignore();

            line_stream >> x3;
            line_stream.ignore();
            line_stream >> y3;
            line_stream.ignore();
            line_stream >> z3;

            x1--; y1--; z1--; x2--; y2--; z2--; x3--; y3--; z3--;

            if(!vert_map.count({x1, y1, z1})) {
                vert_map[{x1, y1, z1}] = {{vertices[x1], normals[z1], {1.0f, 1.0f, 1.0f}, uvs[y1]}, i};
                i++;
            }
            if(!vert_map.count({x2, y2, z2})) {
                vert_map[{x2, y2, z2}] = {{vertices[x2], normals[z2], {1.0f, 1.0f, 1.0f}, uvs[y2]}, i};
                i++;
            }
            if(!vert_map.count({x3, y3, z3})) {
                vert_map[{x3, y3, z3}] = {{vertices[x3], normals[z3], {1.0f, 1.0f, 1.0f}, uvs[y3]}, i};
                i++;
            }

            faces.push_back(glm::vec<3, std::tuple<int, int, int>>({{x1, y1, z1}, {x2, y2, z2}, {x3, y3, z3}}));
        }
    }

    std::vector<Vertex> verts;
    std::vector<GLuint> inds;
    i = 0;

    for (auto& vert: vert_map) {
        verts.push_back(vert.second.first);
        vert.second.second = i;
        i++;
    }

    for (auto& face: faces) {
        for (int i = 0; i < 3; i++)
        {
            inds.push_back(vert_map[face[i]].second);
        }
    }

    return Mesh(verts, inds);
}
