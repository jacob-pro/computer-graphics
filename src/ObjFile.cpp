#include <fstream>
#include "ObjFile.h"
#include "Utils.h"
#include <cassert>
#include <optional>
#include <map>

struct Material {
    std::optional<Colour> colour;
    std::optional<TextureMap> textureMap;
    std::optional<uint32_t> illum;
};

std::map<std::string, Material> loadMtl(const std::string &filename) {
    std::ifstream inputStream(filename, std::ifstream::binary);
    if (!inputStream.is_open()) throw std::runtime_error("Failed to open mtl file");
    std::string line;
    std::map<std::string, Material> materials;
    std::string currentMaterial;

    while (std::getline(inputStream, line)) {
        auto tokens = split(line, ' ');
        if (tokens.empty() || (tokens.size() == 1 && tokens[0].empty())) continue;              // Skip empty lines
        if (stringStartsWith(tokens[0], "#")) continue; // Skip comments

        if (tokens[0] == "newmtl") {
            currentMaterial = tokens[1];
        } else if (tokens[0] == "Kd") {
            auto c = Colour((int) round(std::stof(tokens[1]) * 255.0),
                            (int) round(std::stof(tokens[2]) * 255.0),
                            (int) round(std::stof(tokens[3]) * 255.0));
            materials[currentMaterial].colour = c;
        } else if (tokens[0] == "map_Kd") {
            auto map = TextureMap(tokens[1]);
            materials[currentMaterial].textureMap = map;
        } else if (tokens[0] == "illum") {
            materials[currentMaterial].illum = std::stoi(tokens[1]);
        } else {
            std::cout << "Warning unrecognised token in mtl file" << std::endl;
        }
    }
    inputStream.close();
    return materials;
}

std::vector<ModelTriangle> loadObj(const std::string &filename, float scale, const Colour& default_colour) {
    std::ifstream inputStream(filename, std::ifstream::binary);
    if (!inputStream.is_open()) throw std::runtime_error("Failed to open obj file");
    std::string line;
    std::vector<glm::vec3> geometricVertices;      // (v)
    std::vector<glm::vec3> vertexNormals;          // (vn)
    std::vector<glm::vec2> textureVertices;        // (vt)
    std::vector<ModelTriangle> triangles;
    std::map<std::string, Material> materials;
    std::optional<Material> selectedMaterial;

    while (std::getline(inputStream, line)) {
        auto tokens = split(line, ' ');
        if (tokens.empty() || (tokens.size() == 1 && tokens[0].empty())) continue;              // Skip empty lines
        if (stringStartsWith(tokens[0], "#")) continue; // Skip comments
        if (tokens[0] == "o") continue; // Skip object grouping

        if (tokens[0] == "v") {
            if (tokens.size() != 4) throw std::runtime_error("Vertex must have three coordinates");
            auto one = -std::stof(tokens[1]) * scale;
            auto two = std::stof(tokens[2]) * scale;
            auto three = std::stof(tokens[3]) * scale;
            geometricVertices.emplace_back(one, two, three);

        } else if (tokens[0] == "vt") {
            if (tokens.size() != 3) throw std::runtime_error("Texture Vertex must have two coordinates");
            auto one = std::stof(tokens[1]);
            auto two = std::stof(tokens[2]);
            textureVertices.emplace_back(one, two);

        } else if (tokens[0] == "vn") {
            if (tokens.size() != 4) throw std::runtime_error("Vertex normal must have three coordinates");
            auto one = std::stof(tokens[1]);
            auto two = std::stof(tokens[2]);
            auto three = std::stof(tokens[3]);
            auto normalised = -glm::normalize(glm::vec3{one, two, three});
            vertexNormals.push_back(normalised);

        } else if (tokens[0] == "f") {
            if (tokens.size() != 4) throw std::runtime_error("Triangle face must have three vertices");
            auto triangle = ModelTriangle();
            for (size_t i = 0; i < 3; i++) {
                auto parts = split(tokens[i + 1], '/');
                triangle.vertices[i] = geometricVertices[std::stoi(parts[0]) - 1];
                if (parts.size() > 1 && !parts[1].empty()) {
                    auto tv = textureVertices[std::stoi(parts[1]) - 1];
                    triangle.texturePoints[i] = TexturePoint(tv[0], tv[1]);
                }
                if (parts.size() > 2 && !parts[2].empty()) {
                    auto vn = vertexNormals[std::stoi(parts[2]) - 1];
                    triangle.vertexNormals[i] = vn;
                }
            }
            if (selectedMaterial.has_value()) {
                triangle.colour = selectedMaterial->colour.has_value() ? selectedMaterial->colour.value() : default_colour;
                triangle.textureMap = selectedMaterial->textureMap;
                if (selectedMaterial->illum.has_value() && selectedMaterial->illum.value() == 3) {
                    triangle.mirror = true;
                }
            } else {
                triangle.colour = default_colour;
            }
            triangles.push_back(triangle);

        } else if (tokens[0] == "usemtl") {
            selectedMaterial = materials.at(tokens[1]);

        } else if (tokens[0] == "mtllib") {
            materials.merge(loadMtl(tokens[1]));
            if (!selectedMaterial.has_value()) {
                if (materials.count("")) {
                    selectedMaterial = materials[""];
                }
            }
        } else {
            std::cout << "Warning unrecognised token in obj file" << std::endl;
        }
    }
    inputStream.close();
    return triangles;
}
