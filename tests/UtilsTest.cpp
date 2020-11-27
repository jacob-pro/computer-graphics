#include <catch2/catch.hpp>
#include <iostream>
#include "Utils.h"

TEST_CASE( "interpolateSingleFloats" ) {
    auto result = interpolateSingleFloats(2.2, 8.5, 7);
    auto expected = std::vector<float> {2.2, 3.25, 4.3, 5.35, 6.4, 7.45, 8.5};
    REQUIRE_THAT(result, Catch::Matchers::Approx(expected));
}

bool vec3ApproxEqual(glm::vec3 l, glm::vec3 r) {
    for (size_t i = 0; i < 3; i++) {
        if (l[i] != Approx(r[i])) return false;
    }
    return true;
}

TEST_CASE( "interpolateThreeElementValues" ) {
    auto from = glm::vec3(1, 4, 9.2);
    auto to = glm::vec3(4, 1, 9.8);
    auto result = interpolateThreeElementValues(from, to, 4);
    auto expected = std::vector<glm::vec3> {
        glm::vec3(1, 4, 9.2),
        glm::vec3(2, 3, 9.4),
        glm::vec3(3, 2, 9.6),
        glm::vec3(4, 1, 9.8)
    };
    for (size_t i = 0; i < result.size(); i++) {
        REQUIRE(vec3ApproxEqual(result[i], expected[i]));
    }
}
