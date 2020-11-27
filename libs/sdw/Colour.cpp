#include "Colour.h"
#include <utility>

Colour::Colour() = default;
Colour::Colour(int r, int g, int b) : red(r), green(g), blue(b) {}
Colour::Colour(std::string n, int r, int g, int b) :
        name(std::move(n)),
        red(r), green(g), blue(b) {}

uint32_t Colour::sdlValue() const {
    return (255u << 24u) + ((uint16_t)(red * brightness) << 16u) + ((uint16_t)(green * brightness) << 8u) + (uint16_t)(blue * brightness);
}

std::ostream &operator<<(std::ostream &os, const Colour &colour) {
    os << colour.name << " ["
       << colour.red << ", "
       << colour.green << ", "
       << colour.blue << "]";
    return os;
}
