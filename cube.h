#pragma once

#include "draw.h"
#include <array>

class Cube {
public:
    array<Vertex<double>, 4> points;
    Vertex<double> center;
    Vertex<double> u;
    double w = 0.0;

    Cube() = default;

    Cube(const vector<Vertex<double>> &vec, const Vertex<double> &u, double w = 0) : u(u), w(w) {
        for (int i = 0; i < points.size(); i++)
            points[i] = vec[i];

        center = Vertex<double>(0, 0, 0);
        for (auto &p: points)
            center += p;

        center /= 4;
    }

    void draw(const Color &color) const {
        draw_line(points[0], points[1], color);
        draw_line(points[1], points[2], color);
        draw_line(points[2], points[3], color);
        draw_line(points[3], points[0], color);
    }

    void move(double dt) {
        for (auto &p: points)
            p += u * dt;
        center += u * dt;
    }

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