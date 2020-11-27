#ifndef DEPTHBUFFEREDWINDOW_H
#define DEPTHBUFFEREDWINDOW_H

#include "DrawingWindow.h"

class DepthBufferedWindow {
public:
    explicit DepthBufferedWindow(DrawingWindow &window);
    void setPixelColour(int32_t x, int32_t y, double invDepth, uint32_t colour);
    [[nodiscard]] size_t getHeight() const;
    [[nodiscard]] size_t getWidth() const;
    [[nodiscard]] DrawingWindow &getWindow() const;
private:
    DrawingWindow &window;
    size_t height;
    size_t width;
    std::vector<double> buffer;
    [[nodiscard]] double getValueAt(size_t x, size_t y) const;
    void setValueAt(size_t x, size_t y, double value);
};

#endif //DEPTHBUFFEREDWINDOW_H
