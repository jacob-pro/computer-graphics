# Invoking the CMake build from the command line is a two step process, first, *generate* a build by running the following:
#
#   cmake -Bbuild -H. -DCMAKE_BUILD_TYPE=Release
# 
# Where `Release` can be replaced with `Debug`, which contains gdb and address sanitiser definitions already written for you.
# Once a build is created, proceed with *compilation*:
# 
#   cmake --build build --target RedNoise --config Release # optionally, for parallel build, append -j $(nproc)
#
# This creates the executable in the build directory. You only need to *generate* a build if you modify the CMakeList.txt file.
# For any other changes to the source code, simply recompile.


cmake_minimum_required(VERSION 2.8)
project(CG2020)
option(BUILD_TESTS "Enable tests" OFF)

set(CMAKE_CXX_STANDARD 17)

# Note, we do this for glm because it's a header only library and because we shipped it with the project
# normally you would use find_package(<package_name>) for libraries with actual objects
set(GLM_INCLUDE_DIRS libs/glm-0.9.7.2)

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})
include_directories(libs/sdw)

set(SOURCES
        libs/sdw/CanvasPoint.cpp
        libs/sdw/CanvasTriangle.cpp
        libs/sdw/Colour.cpp
        libs/sdw/DrawingWindow.cpp
        libs/sdw/ModelTriangle.cpp
        libs/sdw/RayTriangleIntersection.cpp
        libs/sdw/TextureMap.cpp
        libs/sdw/TexturePoint.cpp
        libs/sdw/Utils.cpp
        src/AnimationDriver.cpp
        src/Camera.cpp
        src/DepthBufferedWindow.cpp
        src/LightingConfiguration.cpp
        src/LineSegment.cpp
        src/ObjFile.cpp
        src/RasterisedRenderer.cpp
        src/RaytracedRenderer.cpp
        src/Renderer.cpp
        src/WireframeRenderer.cpp
        src/main.cpp)

add_executable(CG2020 ${SOURCES})

if (MSVC)
    target_compile_options(CG2020
            PUBLIC
            /W3
            /Zc:wchar_t
            )
    set(DEBUG_OPTIONS /MTd)
    set(RELEASE_OPTIONS /MT /GF /Gy /O2 /fp:fast)
    if (NOT DEFINED SDL2_LIBRARIES)
        set(SDL2_LIBRARIES SDL2::SDL2 SDL2::SDL2main)
    endif()
else ()
    target_compile_options(CG2020
        PUBLIC
        -std=c++17
        -Wall
        -Wextra
        -Wcast-align
        -Wfatal-errors
        -Werror=return-type
        -Wno-unused-parameter
        -Wno-unused-variable
        -Wno-ignored-attributes)

    set(DEBUG_OPTIONS -O2 -fno-omit-frame-pointer -g)
    set(RELEASE_OPTIONS -O3 -march=native -mtune=native)
    target_link_libraries(CG2020 PUBLIC $<$<CONFIG:Debug>:-Wl,-lasan>)

endif()


target_compile_options(CG2020 PUBLIC "$<$<CONFIG:RelWithDebInfo>:${RELEASE_OPTIONS}>")
target_compile_options(CG2020 PUBLIC "$<$<CONFIG:Release>:${RELEASE_OPTIONS}>")
target_compile_options(CG2020 PUBLIC "$<$<CONFIG:Debug>:${DEBUG_OPTIONS}>")
 
target_link_libraries(CG2020 PRIVATE ${SDL2_LIBRARIES})


if(BUILD_TESTS)
    enable_testing()
    add_executable(tests
            tests/main.cpp
            tests/UtilsTest.cpp
            tests/LightingConfigurationTest.cpp
            ${SOURCES}
            )
    find_package(Catch2 REQUIRED)
    target_link_libraries(tests PRIVATE Catch2::Catch2)
    target_link_libraries(tests PRIVATE ${SDL2_LIBRARIES})
    add_test (NAME Tests COMMAND tests)
endif()
