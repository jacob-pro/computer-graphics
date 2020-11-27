#include <catch2/catch.hpp>
#include "../src/LightingConfiguration.h"

TEST_CASE( "getPointCluster" ) {
    auto lighting = LightingConfiguration(glm::vec3{0,0,30}, 0, 8, 0);
    lighting.lightPoints = 64;
    lighting.lightPointsSample = 1.0;
    auto nonRotated = lighting.getPointCluster(lighting.lightPosition);
    REQUIRE(nonRotated.size() == 64);
    REQUIRE(nonRotated[0] == glm::vec3{-8, -8, 30});
    REQUIRE(nonRotated[63] == glm::vec3{6, 6, 30});

    auto observer = glm::vec3{50, 0, 30};
    auto rotated = lighting.getPointCluster(observer);
    auto len = glm::length(lighting.lightPosition - observer);
    auto len2 = glm::length(rotated[36] - observer);
    REQUIRE(rotated.size() == 64);
    REQUIRE(len == len2);
}
