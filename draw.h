#pragma once

#include <vector>
#include <ostream>
#include "vertex.h"

using namespace std;

struct Color {
    unsigned char r, g, b;

    Color() = default;

    friend ostream &operator<<(ostream &os, const Color &color) {
        os << "r: " << color.r << " g: " << color.g << " b: " << color.b;
        return os;
    }

    Color(unsigned char r, unsigned char g, unsigned char b) : r(r), g(g), b(b) {}
};

void set_pixel(int x, int y, const Color &col) {
    uint32_t p = col.r;
    p <<= 8;
    p |= col.g;
    p <<= 8;
    p |= col.b;

    buffer[y][x] = p;
}

Color get_pixel(int x, int y) {
    uint32_t p = buffer[y][x];
    Color color{};
    color.b = p & 0xFF;
    color.g = (p >> 8) & 0xFF;
    color.r = (p >> 16) & 0xFF;

    return color;
}

void set_pixel(const Vertex<int> &v, const Color &color) {
    set_pixel(v.x, v.y, color);
}

void set_pixel(double x, double y, const Color &col) {
    set_pixel(int(round(x)), int(round(y)), col);
}

void set_pixel(const Vertex<double> &v, const Color &color) {
    set_pixel(v.x, v.y, color);
}

void draw_line(int x1, int y1, int x2, int y2, const Color &col) {
    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }

    const int delta_x = 2 * abs(x2 - x1), delta_y = 2 * abs(y2 - y1);
    const int step_x = x1 < x2 ? 1 : -1, step_y = y1 < y2 ? 1 : -1;
    int error = delta_x - delta_y;
    while (x1 != x2 && y1 != y2) {
        set_pixel(x1, y1, col);
        if (error > -delta_y) {
            error -= delta_y;
            x1 += step_x;
        }
        if (error < delta_x) {
            error += delta_x;
            y1 += step_y;
        }
    }
    while (x1 != x2) {
        set_pixel(x1, y2, col);
        x1 += step_x;
    }
    while (y1 != y2) {
        set_pixel(x2, y1, col);
        y1 += step_y;
    }
    set_pixel(x2, y2, col);
}

void draw_line(const Vertex<int> &from, const Vertex<int> &to, const Color &color) {
    draw_line(from.x, from.y, to.x, to.y, color);
}

void draw_line(const Vertex<double> &from, const Vertex<double> &to, const Color &color) {
    draw_line(round_to_int(from.x), round_to_int(from.y), round_to_int(to.x), round_to_int(to.y), color);
}

vector<int> get_comb_coeffs(int n) {
    if (n == 1)
        return {1};
    vector<int> coeffs(n);
    coeffs[0] = 1;
    coeffs.back() = 1;
    for (int i = 1; i < n / 2; i++) {
        coeffs[i] = coeffs[i - 1] * (n - i) / i;
        coeffs[n - i - 1] = coeffs[i];
    }
    if (n % 2 != 0) {
        coeffs[n / 2] = coeffs[n / 2 - 1] * (n - n / 2) / (n / 2);
    }

    return coeffs;
}

void draw_bezier_curve(const vector<Vertex<double>> &init_points, const Color &color) {
    size_t n = init_points.size();
    auto coeffs = get_comb_coeffs(n);

    // sum(coeffs[i] * (1 - t) ^ (n - i) * t ^ i * points[i])
    Vertex<int> last = to_int_point(init_points[0]);
    for (double t = 0.0; t <= 1.0; t += 0.01) {
        Vertex<double> p = {0.0, 0.0};
        for (size_t i = 0; i < n; i++) {
            p += init_points[i] * (coeffs[i] * pow((1 - t), n - i - 1) * pow(t, i));
        }

        Vertex<int> cur = to_int_point(p);
        if ((cur - last).mod2() > 3) {
            draw_line(last.x, last.y, cur.x, cur.y, color);
            last = cur;
        }
    }

    draw_line(last, to_int_point(init_points.back()), color);
}

void draw_bezier_curve(const vector<Vertex<int>> &init_points, const Color &color) {
    vector<Vertex<double>> points(init_points.size());
    for (size_t i = 0; i < init_points.size(); ++i) {
        points[i] = to_double_point(init_points[i]);
    }

    draw_bezier_curve(points, color);
}
