#pragma once

#include "draw.h"

class Circle
{
public:
    double r{};
    Vertex<double> center;
    Vertex<double> u;

    Circle() = default;

    Circle(const Vertex<double> &center, double r, const Vertex<double> &u = {0, 0}) : u(u), center(center), r(r) {}

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

    /// Функция, которая с помощью одной или нескольких кривых Безье 3-го
    /// порядка строит дугу окружности. Функция получает в качестве параметров координаты
    /// центра окружности, радиус окружности и значения двух углов, которые задают радиус-вектора от центра окружности до
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

    void fill(const Color &color) const {
        draw_with_bezier(color);
        fill_figure(to_int_point(center), color);
    }

    ~Circle() = default;
};