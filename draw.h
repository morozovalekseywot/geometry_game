#pragma once

#include <vector>
#include <ostream>
#include <stack>
#include "vertex.h"
#include "color.h"
#include "mathematics.h"
#include "color_settings.h"

using namespace std;

bool is_point_in_image(int x, int y) {
    return 0 <= x && x < SCREEN_WIDTH && 0 <= y && y < SCREEN_HEIGHT;
}

bool is_point_in_image(const Vertex<int> &v) {
    return 0 <= v.x && v.x < SCREEN_WIDTH && 0 <= v.y && v.y < SCREEN_HEIGHT;
}

void set_pixel(int x, int y, const Color &col, bool skip_miss = false) {
    if (skip_miss && !is_point_in_image(x, y)) {
        return;
    }

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

void set_pixel(const Vertex<int> &v, const Color &color, bool skip_miss = false) {
    set_pixel(v.x, v.y, color, skip_miss);
}

Color get_pixel(const Vertex<int> &v) {
    return get_pixel(v.x, v.y);
}

void set_pixel(double x, double y, const Color &col, bool skip_miss = false) {
    set_pixel(int(round(x)), int(round(y)), col, skip_miss);
}

void set_pixel(const Vertex<double> &v, const Color &color, bool skip_miss = false) {
    set_pixel(v.x, v.y, color, skip_miss);
}

/// @brief Отрисовка отрезка алгоритмом Брезенхема
void draw_line(int x1, int y1, int x2, int y2, const Color &col, bool skip_miss = false) {
    if (x1 > x2) {
        swap(x1, x2);
        swap(y1, y2);
    }

    const int delta_x = 2 * abs(x2 - x1), delta_y = 2 * abs(y2 - y1);
    const int step_x = x1 < x2 ? 1 : -1, step_y = y1 < y2 ? 1 : -1;
    int error = delta_x - delta_y;
    while (x1 != x2 && y1 != y2) {
        set_pixel(x1, y1, col, skip_miss);
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
        set_pixel(x1, y2, col, skip_miss);
        x1 += step_x;
    }
    while (y1 != y2) {
        set_pixel(x2, y1, col, skip_miss);
        y1 += step_y;
    }
    set_pixel(x2, y2, col, skip_miss);
}

void draw_line(const Vertex<int> &from, const Vertex<int> &to, const Color &color, bool skip_miss = false) {
    draw_line(from.x, from.y, to.x, to.y, color, skip_miss);
}

void draw_line(const Vertex<double> &from, const Vertex<double> &to, const Color &color, bool skip_miss = false) {
    draw_line(round_to_int(from.x), round_to_int(from.y),
              round_to_int(to.x), round_to_int(to.y),
              color, skip_miss);
}

/// @brief Отрисовка кривой Безье
void draw_bezier_curve(const vector<Vertex<double>> &init_points, const Color &color, bool skip_miss = false) {
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
            draw_line(last, cur, color, skip_miss);
            last = cur;
        }
    }

    draw_line(last, to_int_point(init_points.back()), color, skip_miss);
}

void draw_bezier_curve(const vector<Vertex<int>> &init_points, const Color &color, bool skip_miss = false) {
    vector<Vertex<double>> points(init_points.size());
    for (size_t i = 0; i < init_points.size(); ++i) {
        points[i] = to_double_point(init_points[i]);
    }

    draw_bezier_curve(points, color, skip_miss);
}

/// @brief Заливка фигуры
/// @param seed - начальная точка
/// @param new_color - цвет закраски
/// @param stop_color - цвет, за который нельзя выходить
void fill_figure(const Vertex<int> &seed, const Color &new_color, const Color &stop_color = bounds_color) {
    static const int dx[4] = {0, 1, 0, -1}; // смещения для получения координат 4-х соседей
    static const int dy[4] = {-1, 0, 1, 0};
    std::stack<Vertex<int>> stack;

    if(is_point_in_image(seed)){
        stack.push(seed);
    } else {
        for(int i = 0; i < 4; i++){
            Vertex<int> next(seed.x + dx[i], seed.y + dy[i]);
            if (is_point_in_image(next)) {
                Color currentColor = get_pixel(next);
                if (!(currentColor == stop_color) && !(currentColor == new_color)) {
                    stack.push(next);
                    break;
                }
            }
        }
    }

    while (!stack.empty()) {
        Vertex<int> v = stack.top();
        stack.pop();
        set_pixel(v, new_color, true);
        for (int i = 0; i < 4; i++) {
            Vertex<int> next(v.x + dx[i], v.y + dy[i]);
            if (is_point_in_image(next)) {
                Color currentColor = get_pixel(next);
                if (!(currentColor == stop_color) && !(currentColor == new_color)) {
                    stack.push(next);
                }
            }
        }
    }
}

void draw_bounds() {
    for (int y = 0; y < bounds_size; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            set_pixel(x, y, bounds_color);
            set_pixel(x, SCREEN_HEIGHT - y - 1, bounds_color);
        }
    }

    for (int x = 0; x < bounds_size; x++) {
        for (int y = bounds_size; y < SCREEN_HEIGHT - bounds_size; y++) {
            set_pixel(x, y, bounds_color);
            set_pixel(SCREEN_WIDTH - x - 1, y, bounds_color);
        }
    }
}
