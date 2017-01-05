#ifndef MESHPARSER_H
#define MESHPARSER_H

#include <string>
#include "model.hpp"

void parseOBJ(const std::string& obj, Model& model);
void parseMTL(const std::string& path, const std::string& matFile, Material& mat);
void parseLayerData(const std::string& headerFile, Model& model);

#endif
