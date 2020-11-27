#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

std::vector<std::string> split(const std::string &line, char delimiter);

bool stringStartsWith(const std::string& s, const std::string& with);

std::vector<float> interpolateSingleFloats(float from, float to, size_t numberOfValues);

std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, size_t numberOfValues);
