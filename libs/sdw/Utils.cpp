#include <sstream>
#include "Utils.h"
#include <cassert>

std::vector<std::string> split(const std::string &line, char delimiter) {
    auto haystack = line;
    std::vector<std::string> tokens;
    size_t pos;
    while ((pos = haystack.find(delimiter)) != std::string::npos) {
        tokens.push_back(haystack.substr(0, pos));
        haystack.erase(0, pos + 1);
    }
    // Push the remaining chars onto the vector
    tokens.push_back(haystack);
    return tokens;
}

bool stringStartsWith(const std::string& s, const std::string& with) {
    return s.rfind(with, 0) == 0;
}

std::vector<float> interpolateSingleFloats(float from, float to, size_t numberOfValues) {
    if (numberOfValues < 2) throw std::runtime_error("numberOfValues must be at least 2");
    auto incr = (to - from) / float (numberOfValues - 1);
    std::vector<float> result;
    result.reserve(numberOfValues);
    for(size_t i = 0; i < numberOfValues; i++) {
        result.push_back(from + (incr * i));
    }
    return result;
}

std::vector<glm::vec3> interpolateThreeElementValues(glm::vec3 from, glm::vec3 to, size_t numberOfValues) {
    assert(numberOfValues >= 2);
    auto div = float (numberOfValues - 1);
    auto i0 = (to[0] - from[0]) / div;
    auto i1 = (to[1] - from[1]) / div;
    auto i2 = (to[2] - from[2]) / div;
    std::vector<glm::vec3> result;
    result.reserve(numberOfValues);
    for(size_t i = 0; i < numberOfValues; i++) {
        auto a = from[0] + (i0 * i);
        auto b = from[1] + (i1 * i);
        auto c = from[2] + (i2 * i);
        auto x = glm::vec3(a, b, c);
        result.push_back(x);
    }
    return result;
}
