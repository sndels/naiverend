#ifndef MESHPARSER_H
#define MESHPARSER_H

#include <string>
#include "mesh.hpp"

void parseOBJ(const std::string& obj, Mesh& mesh);

#endif