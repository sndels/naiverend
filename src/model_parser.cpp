// parseVolumeCube and lerpIntersection are based on Paul Bourke's implementation found at
// http://paulbourke.net/geometry/polygonise/

#include "model_parser.hpp"

#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include "mc_tables.hpp"

using glm::vec3;
using glm::vec2;
using glm::u32vec3;
using std::cerr;

namespace {

    std::vector<std::vector<std::vector<int32_t> > > lastLayer;
    std::vector<std::vector<int32_t> > lastRow;
    std::pair<int32_t, int32_t> lastCube;

    struct IsoVert
    {
        vec3 pos;
        int16_t value;
    };

    // Generates positions from [-1,-1] to [1,1] with value 0
    std::vector<IsoVert> genEmptyLayer(const uint32_t& resX, const uint32_t& resY,
                                       const float& aspX, const float& aspY,
                                       const float& posZ)
    {
        std::vector<IsoVert> layerVerts;
        float scaleX = 2.f / resX;
        float scaleY = 2.f / resY;
        for (auto j = 0u; j < resY; ++j) {
            for(auto i = 0u; i < resX; ++i) {
                layerVerts.push_back({vec3((((i % resX)) * scaleX - 1.f) / aspX,
                                           -(j * scaleY - 1.f) / aspY,
                                           posZ), 0});
            }
        }
        return layerVerts;
    }

    // Reads file with set parameters and ties read values to positions from [-1,-1] to [1,1]
    // Currently takes mac-ordered files
    std::vector<IsoVert> readVolumeLayer(std::string filename, const uint32_t& resX, const uint32_t& resY,
                                         const float& aspX, const float& aspY, const float& posZ)
    {
        std::ifstream in(filename, std::ios::binary);

        if (!in) {
            cerr << "Error reading layer with filename " << filename << std::endl;
            return std::vector<IsoVert>();
        }

        std::vector<IsoVert> layerVerts;
        int16_t val;
        uint32_t counter = 0;
        float scaleX = 2.f / resX;
        float scaleY = 2.f / resY;
        while (in.read(reinterpret_cast<char *>(&val), sizeof(val))) {
            // Bytes are in mac-order
            uint8_t* valbytes = reinterpret_cast<uint8_t*>(&val);
            std::reverse(valbytes, valbytes + sizeof(val));
            layerVerts.push_back({vec3((((counter % resX)) * scaleX - 1.f) / aspX,
                                       -(counter / resX * scaleY - 1.f) / aspY,
                                       posZ), val});
            ++counter;
        }
        if (counter != resX * resY) {
            cerr << "Error reading layer with filename " << filename << std::endl;
            return std::vector<IsoVert>();
        }

        return layerVerts;
    }

    // Lerps intersection position on cube edge between v1, v2
    int32_t lerpIntersection(const IsoVert& v1, const IsoVert& v2, const int& treshold, std::vector<Vertex>& verts)
    {
        verts.push_back({v1.pos + float(treshold - v1.value) * (v2.pos - v1.pos) / float(v2.value - v1.value),
                         vec3(0.f, 0.f, 0.f)});
        return verts.size() - 1;
    }

    // Pushes triangles in the given cube to verts and faces
    void parseVolumeCube(std::vector<IsoVert>& corners, const uint32_t& x, const uint32_t& y, const int& treshold,
                         std::vector<Vertex>& verts, std::vector<u32vec3>& faces)
    {
        // Get index for cube variation
        uint8_t cubeI = 0;
        if (corners[0].value >= treshold) cubeI |= 1;
        if (corners[1].value >= treshold) cubeI |= 2;
        if (corners[2].value >= treshold) cubeI |= 4;
        if (corners[3].value >= treshold) cubeI |= 8;
        if (corners[4].value >= treshold) cubeI |= 16;
        if (corners[5].value >= treshold) cubeI |= 32;
        if (corners[6].value >= treshold) cubeI |= 64;
        if (corners[7].value >= treshold) cubeI |= 128;

        if (cubeI == 0) return;

        // Lerp edge intersections
        int edges = EDGE_TABLE[cubeI];
        std::vector<int32_t> intersections(12, -1);

        // Get intersections from previous layer (no checking for zerolayer as these wont evaluate true)
        if(edges & 1) intersections[0] = lastLayer[y][x][0];
        if(edges & 2) intersections[1] = lastLayer[y][x][1];
        if(edges & 4) intersections[2] = lastLayer[y][x][2];
        if(edges & 8) intersections[3] = lastLayer[y][x][3];

        // Get intersections from previous row if possible
        if (y != 0) {
            if(edges & 16) intersections[4] = lastRow[x][1];
            if(edges & 256) intersections[8] = lastRow[x][0];
            if(edges & 512) intersections[9] = lastRow[x][2];
        }
        else {
            if(edges & 16) intersections[4] = lerpIntersection(corners[4], corners[5], treshold, verts);
            if(edges & 256) intersections[8] = lerpIntersection(corners[0], corners[4], treshold, verts);
            if(edges & 512) intersections[9] = lerpIntersection(corners[1], corners[5], treshold, verts);
        }

        // Get intersections from previous cube if possible
        if (x != 0) {
            if(edges & 128) intersections[7] = lastCube.second;
            if(edges & 2048) intersections[11] = lastCube.first;
        }
        else {
            if(edges & 128) intersections[7] = lerpIntersection(corners[7], corners[4], treshold, verts);
            if(edges & 2048) intersections[11] = lerpIntersection(corners[3], corners[7], treshold, verts);
        }

        // Lerp new intersections
        if(edges & 32) intersections[5] = lerpIntersection(corners[5], corners[6], treshold, verts);
        if(edges & 64) intersections[6] = lerpIntersection(corners[6], corners[7], treshold, verts);
        if(edges & 1024) intersections[10] = lerpIntersection(corners[2], corners[6], treshold, verts);

        const int* mFaces = TRI_TABLE[cubeI];
        for (int i = 0; TRI_TABLE[cubeI][i] != -1; i += 3) {
            int32_t v0 = intersections[mFaces[i]];
            int32_t v1 = intersections[mFaces[i+2]];
            int32_t v2 = intersections[mFaces[i+1]];
            vec3 normal = normalize(cross(verts[v2].pos - verts[v0].pos, verts[v1].pos - verts[v0].pos));
            verts[v0].normal += normal;
            verts[v1].normal += normal;
            verts[v2].normal += normal;
            faces.push_back(u32vec3(v0, v1, v2));
        }

        // Update precalculated indexes
        lastLayer[y][x][0] = intersections[4];
        lastLayer[y][x][1] = intersections[5];
        lastLayer[y][x][2] = intersections[6];
        lastLayer[y][x][3] = intersections[7];

        lastRow[x][0] = intersections[11];
        lastRow[x][1] = intersections[6];
        lastRow[x][2] = intersections[10];

        lastCube.first = intersections[10];
        lastCube.second = intersections[5];
    }
}

void parseOBJ(const std::string& obj, Model& model)
{
    std::string matFile = "";

    std::vector<Vertex> verts;
    std::vector<vec3> positions;
    std::vector<vec3> normals;
    std::vector<vec2> texturepositions;
    std::vector<u32vec3> faces;

    std::ifstream in(obj);
    std::string lineType;
    while (in >> lineType) {
        if (lineType == "mtllib") {
            in >> matFile;
        } else if (lineType == "v") {// Parse vertice positions
            vec3 v;
            in >> v.x;
            in >> v.y;
            in >> v.z;
            positions.push_back(v);
        } else if (lineType == "vn") {// Parse normal coordinates
            vec3 n;
            in >> n.x;
            in >> n.y;
            in >> n.z;
            normals.push_back(n);
        } else if (lineType == "vt") {// Parse texture coordinates
            vec2 t;
            in >> t.x;
            in >> t.y;
            t.y = 1 - t.y;// Reverse y-coordinates as stb_image loads top first
            texturepositions.push_back(t);
        } else if (lineType == "f") {// Parse faces, differing indices for v/t/n not supported
            u32vec3 f;
            uint32_t nil;
            in >> f[2];
            while (in.peek() == '/'){
                in.ignore();
                in >> nil;
            }
            in >> f[1];
            while (in.peek() == '/'){
                in.ignore();
                in >> nil;
            }
            in >> f[0];
            while (in.peek() == '/'){
                in.ignore();
                in >> nil;
            }
            f -= u32vec3(1u,1u,1u);
            faces.push_back(f);
        }
    }
    if (normals.size() == 0) {
        normals.resize(positions.size());
        for (auto& f : faces) {
            vec3 n = cross(positions[f[2]] - positions[f[0]], positions[f[1]] - positions[f[0]]);
            normals[f[0]] += n;
            normals[f[1]] += n;
            normals[f[2]] += n;
        }
        for (auto& n : normals) n = normalize(n);
    }

    if (texturepositions.size() == 0) {
        for (auto i = 0u; i < positions.size(); i++) {
            verts.push_back({positions[i], normals[i], vec2(0.f)});
        }
    } else {
        for (auto i = 0u; i < positions.size(); i++) {
            verts.push_back({positions[i], normals[i], texturepositions[i]});
        }
    }

    std::shared_ptr<Material> mat = std::make_shared<Material>();
    if (matFile != "") {
    	int pathEnd = obj.find_last_of("/\\");
    	std::string path = obj.substr(0, pathEnd + 1);
        parseMTL(path, matFile, *mat);
    }
    model.update(verts, faces, mat);
}

void parseMTL(const std::string& path, const std::string& matFile, Material& mat)
{
    std::ifstream in(path + matFile);
    std::string lineType;
    while (in >> lineType) {
        if (lineType == "#") {
            getline(in, lineType);
        } else if (lineType == "newmtl") {
            in >> mat.materialStr_;
        } else if (lineType == "map_Kd") {// Load diffuse texture
            std::string texFile;
            in >> texFile;
            mat.loadDfTex(path + texFile);
        } else if (lineType == "map_norm") {// Load normal map
            std::string normalFile;
            in >> normalFile;
            mat.loadNormalMap(path + normalFile);
        } else if (lineType == "Ks") {
            vec3 ks;
            in >> ks.x;
            in >> ks.y;
            in >> ks.z;
            mat.specularCol3f_ = ks;
        } else if (lineType == "Ns") {
            float ns;
            in >> ns;
            mat.specularExp1f_ = ns;
        }
    }
}

void parseLayerData(const std::string& headerFile, Model& model)
{
	int pathEnd = headerFile.find_last_of("/\\");
	std::string path = headerFile.substr(0, pathEnd + 1);

    std::ifstream header(headerFile);

    std::string objType;
    std::string objName;
    std::string layerFile;
    uint32_t treshold = 0;
    uint32_t layers = 0;
    uint32_t resX = 0;
    uint32_t resY = 0;
    float aspX = 0;
    float aspY = 0;
    float aspZ = 0;

    // Parse header
    while (header >> objType) {
        if (objType == "name")
            header >> objName;
        else if (objType == "filename")
            header >> layerFile;
        else if (objType == "treshold")
            header >> treshold;
        else if (objType == "layers")
            header >> layers;
        else if (objType == "resX")
            header >> resX;
        else if (objType == "resY")
            header >> resY;
        else if (objType == "aspX")
            header >> aspX;
        else if (objType == "aspY")
            header >> aspY;
        else if (objType == "aspZ")
            header >> aspZ;
    }
    if (objName.empty() || layerFile.empty() || layers == 0 || resX == 0 || resY == 0 || aspX == 0 || aspY == 0 || aspZ == 0)
        return;

    // Init an empty layer and vectors for precalculated data
    std::vector<IsoVert> layer1 = genEmptyLayer(resX, resY, aspX, aspY, -1.f / aspZ);
    lastLayer = std::vector<std::vector<std::vector<int32_t> > >(resY, std::vector<std::vector<int32_t> >(resX, std::vector<int32_t>(4, -1)));
    lastRow = std::vector<std::vector<int32_t> >(resX, std::vector<int32_t>(4, -1));

    // Parse cubes
    float scaleZ = 2.f / (layers + 2);
    std::vector<Vertex> verts;
    std::vector<u32vec3> faces;
    for (uint32_t l = 1; l <= layers + 1; ++l) {
        std::vector<IsoVert> layer2;
        if (l <= layers)
            layer2 = readVolumeLayer(path + layerFile + std::to_string(l), resX, resY,
                                     aspX, aspY, (l * scaleZ - 1.f) / aspZ);
        else
            layer2 = genEmptyLayer(resX, resY, aspX, aspY, 1.f / aspZ);

        if (layer2.empty()) return;

        // Go through the layer
        for (uint32_t j = 0; j < resY - 1; ++j) {
            uint32_t offset = j * resX;
            for (uint32_t i = 0; i < resX - 1; ++i) {
                // Push cube verts to vector
                std::vector<IsoVert> cVerts = {layer1[offset + i],
                                               layer1[offset + i + 1],
                                               layer1[offset + resX + i + 1],
                                               layer1[offset + resX + i],
                                               layer2[offset + i],
                                               layer2[offset + i + 1],
                                               layer2[offset + resX + i + 1],
                                               layer2[offset + resX + i]};
                parseVolumeCube(cVerts, i, j, treshold, verts, faces);
            }
        }
        layer1 = layer2;
    }

    for (auto& v : verts) v.normal = normalize(v.normal);

    lastLayer.clear();
    lastRow.clear();

    std::shared_ptr<Material> mat = std::make_shared<Material>();
    model.update(verts, faces, mat);
}
