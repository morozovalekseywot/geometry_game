#pragma once

#include <vector>
#include <ostream>
#include <stack>
#include "vertex.h"
#include "mathematics.h"

using namespace std;

/// @brief Цвет пикселя
struct Color
{
    unsigned char r, g, b;

    Color() = default;

    Color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}

    friend ostream &operator<<(ostream &os, const Color &color) {
        os << "r: " << color.r << " g: " << color.g << " b: " << color.b;
        return os;
    }

    bool operator==(const Color &rhs) const {
        return r == rhs.r &&
               g == rhs.g &&
               b == rhs.b;
    }

    bool operator!=(const Color &rhs) const {
        return !(rhs == *this);
    }

    ~Color() = default;
};
