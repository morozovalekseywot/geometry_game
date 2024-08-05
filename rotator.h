#pragma once

#include "circle.h"
#include "mathematics.h"

/// Вращатель кругов
class Rotator {
    Vertex<double> center; ///< Центр вращения
    double R; ///< Большой радиус
    double r; ///< Радиус круга
    double w; ///< Угловая скорость вращения кругов
    vector<Circle> circles; ///< Круги
    bool forward = true; ///< Направление вращения

public:

    Rotator() = default;

    Rotator(const Vertex<double> &center, double R, double r, double w, size_t count) : center(center),
                                                                                        R(R), r(r), w(w) {
        if (count < 1) {
            throw runtime_error("Count of circles must be greater then zero");
        }

        circles.resize(count);
        double interval = 2 * M_PI / count;
        for (int i = 0; i < count; i++) {
            double phi = interval * i;

            double x = R * cos(phi);
            double y = R * sin(phi);

            circles[i] = Circle(center + Vertex<double>(x, y), r);
        }
    }

    /// @brief Изменить направление движения кругов
    void change_direction() {
        forward = !forward;
    }

    /// @brief Вращать круги
    void rotate(double dt) {
        double phi = w * dt;
        if (!forward) {
            phi = -phi;
        }
        double cos_phi = cos(phi), sin_phi = sin(phi);
        for (auto &circle: circles) {
            auto vec = circle.center - center;
            double x = vec.x * cos_phi - vec.y * sin_phi;
            double y = vec.x * sin_phi + vec.y * cos_phi;

            circle.center = center + Vertex<double>(x, y);
        }
    }

    /// @brief Отрисовка кругов
    void draw(const Color &color) const {
        for (auto &circle: circles)
            circle.fill(color);
    }

    const vector<Circle> &get_circles() const {
        return circles;
    }

    /// @brief Ускорение вращения в alpha раз
    void up_w(double alpha) {
        w *= alpha;
    }

    ~Rotator() = default;
};
