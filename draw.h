#pragma once

#include <vector>
#include "vertex.h"

using namespace std;

struct Color {
    unsigned char r, g, b;

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

void set_pixel(const Vertex<int> &v, const Color &color) {
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

void draw_bezier_curve(const vector<Vertex<int>> &init_points, const Color &color) {
    size_t n = init_points.size();
    auto coeffs = get_comb_coeffs(n);
    vector<Vertex<double>> points(n);
    for (size_t i = 0; i < n; ++i) {
        points[i] = to_double_point(init_points[i]);
    }

    // sum(coeffs[i] * (1 - t) ^ (n - i) * t ^ i * points[i])
    Vertex<int> last = init_points[0];
    for (double t = 0.0; t <= 1.0; t += 0.01) {
        Vertex<double> p = {0.0, 0.0};
        for (size_t i = 0; i < n; i++) {
            p += points[i] * (coeffs[i] * pow((1 - t), n - i - 1) * pow(t, i));
        }

        Vertex<int> cur = to_int_point(p);
        if ((cur - last).mod2() > 3) {
            draw_line(last.x, last.y, cur.x, cur.y, color);
            last = cur;
        }
    }
    draw_line(last, init_points.back(), color);
}


/// Функция, которая с помощью одной или нескольких кривых Безье 3-го
/// порядка строит дугу окружности. Функция получает в качестве параметров координаты
/// центра окружности, радиус окружности и значения двух углов, которые задают радиус-вектора от центра окружности до
/// крайних точек дуги. Дуга строится против часовой стрелки.
void draw_circle_with_bezier(const Vertex<int> &center, int r, double phi1, double phi2,
                             const Color &color, const Color &color2) {
    double step = M_PI / 4;
    while (phi1 < phi2) {
        double R = r / sin(M_PI / 2 - step / 2);
        double F = 4.0 / 3 / (1 + 1 / cos(step / 4));
        while (phi1 + step <= phi2 + 1e-2) {
            Vertex<int> p1 = center + Vertex{round_to_int(r * cos(phi1)), round_to_int(r * sin(phi1))};
            Vertex<int> p4 = center + Vertex{round_to_int(r * cos(phi1 + step)), round_to_int(r * sin(phi1 + step))};
            Vertex<int> pt =
                    center + Vertex{round_to_int(R * cos(phi1 + step / 2)), round_to_int(R * sin(phi1 + step / 2))};
            Vertex<int> p2 = p1 + (pt - p1) * F;
            Vertex<int> p3 = p4 + (pt - p4) * F;
            draw_bezier_curve({p1, p2, p3, p4}, color);
            phi1 += step;
            set_pixel(p1.x, p1.y, color2);
            set_pixel(p2.x, p2.y, color2);
            set_pixel(p3.x, p3.y, color2);
            set_pixel(p4.x, p4.y, color2);
        }
        step = phi2 - phi1;
    }
}
