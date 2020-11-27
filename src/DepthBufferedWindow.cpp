#include "DepthBufferedWindow.h"

void DepthBufferedWindow::setPixelColour(int32_t x, int32_t y, double invDepth, uint32_t colour) {
    if (x >= width || y >= height || x < 0 || y < 0) return;
    if (invDepth > getValueAt(x, y)) {
        window.setPixelColour(x, y, colour);
        setValueAt(x, y, invDepth);
    }
}

DepthBufferedWindow::DepthBufferedWindow(DrawingWindow &window) : window(window), width(window.width), height(window.height) {
    buffer.resize(width * height, 0.0);
}

double DepthBufferedWindow::getValueAt(size_t x, size_t y) const {
    return buffer[(y * width) + x];
}

void DepthBufferedWindow::setValueAt(size_t x, size_t y, double value) {
    buffer[(y * width) + x] = value;
}

size_t DepthBufferedWindow::getHeight() const {
    return height;
}

size_t DepthBufferedWindow::getWidth() const {
    return width;
}

DrawingWindow &DepthBufferedWindow::getWindow() const {
    return window;
}
