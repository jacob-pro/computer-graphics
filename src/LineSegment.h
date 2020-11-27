#ifndef LINESEGMENT_H
#define LINESEGMENT_H

#include <vector>
#include <array>
#include "CanvasPoint.h"
#include "Colour.h"
#include "DrawingWindow.h"
#include <map>

class LineSegment {

public:
    LineSegment(CanvasPoint start, CanvasPoint end);

    LineSegment(CanvasPoint start, CanvasPoint end, Colour colour);

    [[nodiscard]] const Colour &getColour() const;
    void setColour(const Colour &colour);

    [[nodiscard]] const std::array<CanvasPoint, 2> &getPoints() const;

    [[nodiscard]] const std::vector<CanvasPoint> &getPixels() const;

    void draw(DrawingWindow &window) const;

    // A map of y-coords (rows) to a list of the x-coords in this row
    [[nodiscard]] std::map<int32_t, std::vector<int32_t>> mapYsToXs() const;

private:
    std::array<CanvasPoint, 2> points;
    std::vector<CanvasPoint> pixels;
    Colour colour;
};

#endif //LINESEGMENT_H
