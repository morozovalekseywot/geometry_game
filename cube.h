#pragma once

#include <array>
#include "draw.h"

enum CubeType {
    Projectile, ///< Убивающий куб
    Bonus, ///< Куб, увеличивающий очки
    Freeze ///< Замораживающий куб
};

///@brief Куб
class Cube {
public:
    vector<Vertex<double>> points; ///< Точки куба
    Vertex<double> center; ///< Центр куба
    Vertex<double> u; ///< Вектор скорости куба
    double w = 0.0; ///< Угловая скорость куба
    CubeType type; ///< Тип куба

    Cube() = default;

    Cube(const vector<Vertex<double>> &vec, const Vertex<double> &u,
         double w = 0, CubeType type = CubeType::Projectile) : points(vec), u(u), w(w), type(type) {
        center = Vertex<double>(0, 0, 0);
        for (auto &p: points)
            center += p;

        center /= 4;
    }

    Cube(const Vertex<double> &center, double size, const Vertex<double> &u,
         double w = 0, CubeType type = CubeType::Projectile) : center(center), u(u), w(w), type(type) {
        points = {{center.x - size / 2, center.y - size / 2},
                  {center.x - size / 2, center.y + size / 2},
                  {center.x + size / 2, center.y + size / 2},
                  {center.x + size / 2, center.y - size / 2}};
    }

    /// @brief Отрисовка границ куба
    void draw(const Color &color, bool skip_miss = false) const {
        int n = points.size();
        for (int i = 0; i < n; i++) {
            draw_line(points[i], points[circle_idx(i + 1, n)], color, skip_miss);
        }
    }

    /// @brief Заливка куба
    void fill(const Color &color, bool skip_miss = false) const {
        draw(color, skip_miss);

        fill_figure(to_int_point(center), color);
    }

    /// @brief Движение куба
    void move(double dt) {
        for (auto &p: points)
            p += u * dt;
        center += u * dt;
    }

    /// @brief Вращение куба
    void rotate(double dt) {
        double phi = w * dt;
        double cos_phi = cos(phi), sin_phi = sin(phi);
        for (auto &p: points) {
            auto vec = p - center;
            double x = vec.x * cos_phi - vec.y * sin_phi;
            double y = vec.x * sin_phi + vec.y * cos_phi;

            p = center + Vertex<double>(x, y);
        }
    }

    ~Cube() = default;
};