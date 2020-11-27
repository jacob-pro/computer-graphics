#include "LineSegment.h"

LineSegment::LineSegment(CanvasPoint start, CanvasPoint end) : LineSegment(start, end, Colour()) {}

LineSegment::LineSegment(CanvasPoint from, CanvasPoint to, Colour colour) :
    colour(std::move(colour)),
    points({from, to})
    {
    auto res = std::vector<CanvasPoint>();
    float xDiff = to.x - from.x;
    float yDiff = to.y - from.y;
    float numberOfSteps = std::max(abs(xDiff), abs(yDiff));
    res.reserve((size_t) numberOfSteps);
    float xStepSize = xDiff/numberOfSteps;
    float yStepSize = yDiff/numberOfSteps;
    for (size_t i = 0; i < numberOfSteps; i++) {
        float x = round(from.x + (xStepSize*i));
        float y = round(from.y + (yStepSize*i));
        res.emplace_back(x, y);
    }
    pixels = res;
}

void LineSegment::draw(DrawingWindow &window) const {
    for(const auto& p: this->pixels) {
        if (p.x >= 0 && p.x < window.width && p.y >= 0 && p.y < window.height) {
            // size_t is unsigned
            window.setPixelColour((size_t) p.x, (size_t) p.y, this->colour.sdlValue());
        }
    }
}

const Colour &LineSegment::getColour() const {
    return colour;
}

void LineSegment::setColour(const Colour &c) {
    LineSegment::colour = c;
}

const std::array<CanvasPoint, 2> &LineSegment::getPoints() const {
    return points;
}

const std::vector<CanvasPoint> &LineSegment::getPixels() const {
    return pixels;
}

std::map<int32_t , std::vector<int32_t>> LineSegment::mapYsToXs() const {
    auto m = std::map<int32_t, std::vector<int32_t>>();
    for(const auto& pixel: this->pixels) {
        m[(int32_t) pixel.y].push_back((int32_t) pixel.x);
    }
    return m;
}

