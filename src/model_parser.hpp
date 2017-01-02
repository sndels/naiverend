#ifndef MESHPARSER_H
#define MESHPARSER_H

#include <string>
#include "model.hpp"

void parseOBJ(const std::string& obj, Model& model);
void parseLayerData(const std::string& headerFile, Model& model);

#endif
