#ifndef OBJFILE_H
#define OBJFILE_H

#include <string>
#include <vector>
#include <ModelTriangle.h>

std::vector<ModelTriangle> loadObj(const std::string &filename,
                                   float scale = 1.0,
                                   const Colour& default_colour = Colour());

#endif //OBJFILE_H
