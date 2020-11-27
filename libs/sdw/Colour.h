#pragma once

#include <iostream>

struct Colour {
    std::string name;
    int red{};
    int green{};
    int blue{};
    double brightness = 1.0;
    Colour();
    Colour(int r, int g, int b);
    Colour(std::string n, int r, int g, int b);
    [[nodiscard]] uint32_t sdlValue() const;
};

std::ostream &operator<<(std::ostream &os, const Colour &colour);
