#pragma once

#include "draw.h"

/// @brief Круг
class Circle {
public:
    double r{}; ///< Радиус круга
    Vertex<double> center; ///< Центр круга
    Vertex<double> u; ///< Скорость круга

    Circle() = default;

    Circle(const Vertex<double> &center, double r, const Vertex<double> &u = {0, 0}) : u(u), center(center), r(r) {}

    /// @brief Движение круга
    void move(double dt) {
        center += u * dt;
    }

private:

    void draw_part(const Vertex<double> &point, const Color &color) const {
        set_pixel(center.x + point.x, center.y + point.y, color);
        set_pixel(center.x - point.x, center.y + point.y, color);
        set_pixel(center.x + point.x, center.y - point.y, color);
        set_pixel(center.x - point.x, center.y - point.y, color);
        set_pixel(center.x + point.y, center.y + point.x, color);
        set_pixel(center.x - point.y, center.y + point.x, color);
        set_pixel(center.x + point.y, center.y - point.x, color);
        set_pixel(center.x - point.y, center.y - point.x, color);
    }

public:

    /// @brief Отрисовка границ круга
    void draw(const Color &color) const {
        int x = 0, y = r;
        int d = 3 - 2 * r;
        draw_part(Vertex<double>(x, y), color);
        while (y >= x) {
            x++;
            if (d > 0) {
                y--;
                d = d + 4 * (x - y) + 10;
            } else
                d = d + 4 * x + 6;
            draw_part(Vertex<double>(x, y), color);
        }
    }

    /// @brief Функция, которая с помощью одной или нескольких кривых Безье 3-го порядка строит дугу окружности.
    /// @param color цвет отрисовки
    /// @param phi1, phi2 значение двух углов, которые задают радиус-вектора от центра окружности до
    /// крайних точек дуги. Дуга строится против часовой стрелки.
    void draw_with_bezier(const Color &color, double phi1 = 0, double phi2 = 2 * M_PI) const {
        double step = M_PI / 4;
        while (phi1 < phi2) {
            double R = r / sin(M_PI / 2 - step / 2);
            double F = 4.0 / 3 / (1 + 1 / cos(step / 4));
            while (phi1 + step <= phi2 + 1e-2) {
                Vertex<double> p1 = center + Vertex{r * cos(phi1), r * sin(phi1)};
                Vertex<double> p4 = center + Vertex{r * cos(phi1 + step), r * sin(phi1 + step)};
                Vertex<double> pt = center + Vertex{R * cos(phi1 + step / 2), R * sin(phi1 + step / 2)};
                Vertex<double> p2 = p1 + (pt - p1) * F;
                Vertex<double> p3 = p4 + (pt - p4) * F;
                draw_bezier_curve({p1, p2, p3, p4}, color);
                phi1 += step;
            }
            step = phi2 - phi1;
        }
    }

    /// @brief Заливка круга
    void fill(const Color &color) const {
        draw_with_bezier(color);
        fill_figure(to_int_point(center), color);
    }

    /// @brief Отрисовка границы круга прерывистой линией
    void draw_segment_line(const Color &color, int count) const {
        double delta = 2 * M_PI / count;
        for (int i = 0; i < count; i++) {
            double phi = delta * i;
            draw_with_bezier(color, phi, phi + delta / 2);
        }
    }

    ~Circle() = default;
};