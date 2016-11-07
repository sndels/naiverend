#ifndef MESHPARSER_H
#define MESHPARSER_H

#include <string>
#include "mesh.hpp"

void parseOBJ(const std::string& obj, Mesh& mesh);
void parseLayerFile(const std::string& headerFile, Mesh& mesh);

#endif
