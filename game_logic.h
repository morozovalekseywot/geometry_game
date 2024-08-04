#pragma once

#include "cube_launcher.h"
#include "rotator.h"

class GameLogic
{
    Rotator rotator;
    CubeLauncher cube_launcher;
    int score = 0;

    double time = 0;
    bool is_freeze = false;

public:

    GameLogic() = default;

    GameLogic(const Rotator &rotator, const CubeLauncher &cube_launcher) : rotator(rotator),
                                                                           cube_launcher(cube_launcher) {}


private:
    bool is_intersects(const Cube &cube, const Circle &circle) const {
        bool check_close = false;
        for (auto &p: cube.points) {
            if ((circle.center - p).mod() < 1.5 * circle.r) {
                check_close = true;
                break;
            }
        }
        if (!check_close) {
            return false;
        }

        int n = cube.points.size();
        for (int i = 0; i < n; i++) {
            const Vertex<double> &p1 = cube.points[i];
            const Vertex<double> &p2 = cube.points[circle_idx(i + 1, n)];

            double c1 = p1.x;
            double c = p2.x - p1.x;
            double c3 = p1.y;
            double d = p2.y - p1.y;

            // (ct - a)^2 + (dt - b)^2 = r^2
            double a = circle.center.x - c1;
            double b = circle.center.y - c3;

            // At^2 - 2Bt + C = 0
            double A = c * c + d * d;
            double B = a * c + d * b;
            double C = a * a + b * b - circle.r * circle.r;

            double D = B * B - A * C;
            if (D < 0) {
                continue;
            }

            double sqr = sqrt(D);
            double t1 = (B + sqr) / A;
            double t2 = (B - sqr) / A;
            if ((0 <= t1 && t1 <= 1) || (0 <= t2 && t2 <= 1)) {
                return true;
            }
        }

        return false;
    }

    vector<bool> find_intersections() const {
        auto &circles = rotator.get_circles();
        vector<bool> res;
        res.reserve(circles.size());
        for (auto &cube: cube_launcher.cubes) {
            bool intersects = false;
            for (auto &circle: circles) {
                if (is_intersects(cube, circle)) {
                    intersects = true;
                    break;
                }
            }
            res.push_back(intersects);
        }

        return res;
    }

public:

    void actions(double dt) {
        time -= dt;
        if(is_freeze && time <= 0)
            is_freeze = false;

        if(!is_freeze)
            rotator.rotate(dt);

        cube_launcher.move(dt);
        cube_launcher.generate(dt);
    }

    void draw() {
        if(is_freeze)
            rotator.draw(freeze_color);
        else
            rotator.draw(circle_color);

        cube_launcher.draw();
    }

    bool update_score() {
        auto res = find_intersections();
        auto it = cube_launcher.cubes.begin();
        for(int i = 0; i < res.size(); i++){
            if(!res[i]) {
                it++;
                continue;
            }

            switch (it->type) {
                case Projectile:
                    return false;
                case Bonus:
                    score++;
                    break;
                case Freeze:
                    time = freeze_time;
                    is_freeze = true;
                    break;
            }

            auto cur = it;
            it++;
            cube_launcher.cubes.erase(cur);
        }

        return true;
    }

    void change_direction() {
        if(time <= 0) {
            rotator.change_direction();
            time = wait_after_press;
        }
    }

    int get_score() {
        return score;
    }
};
