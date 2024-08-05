#pragma once

#include "cube.h"
#include "color_settings.h"
#include <list>
#include <random>

///@brief Класс, предназначенный для запуска и контроля кубов
class CubeLauncher {
    int cube_limit = 4; ///< Количество кубиков одновременно на экране
    double bonus_part{}; ///< Доля бонусных кубов
    double freeze_part{}; ///< Доля замораживающих кубов
    double time = 0.0;
    double T{}; ///< Период запуска

    std::uniform_real_distribution<double> type_generator;
    std::uniform_real_distribution<double> speed_generator;
    std::uniform_real_distribution<double> angular_speed_generator;
    std::uniform_real_distribution<double> place_generator;
    std::uniform_real_distribution<double> target_x_generator;
    std::uniform_real_distribution<double> target_y_generator;
    std::uniform_int_distribution<int> size_generator;
    std::uniform_int_distribution<int> wall_generator;
    std::default_random_engine re;
    std::default_random_engine re_cube_type;

public:
    list <Cube> cubes; ///< Текущие кубы

    CubeLauncher() = default;

    /// @param cube_limit Количество кубиков одновременно на экране
    /// @param bonus_part Доля бонусных кубов
    /// @param freeze_part Доля замораживающих кубов
    /// @param speed_min нижняя граница скорости кубов
    /// @param speed_max верхняя граница скорости кубов
    /// @param w_min нижняя граница угловой скорости кубов
    /// @param w_max верхняя граница угловой скорости кубов
    /// @param size_min нижняя граница размера кубов
    /// @param size_max верхняя границы размера кубов
    CubeLauncher(int cube_limit, double bonus_part, double freeze_part, double T,
                 double speed_min, double speed_max, double w_min, double w_max,
                 int size_min, int size_max) : cube_limit(cube_limit), T(T),
                                               bonus_part(bonus_part), freeze_part(freeze_part) {
        if (bonus_part < 0 || bonus_part > 1)
            throw runtime_error("Part of bonus cubes must be between 0 and 1");
        if (freeze_part < 0 || freeze_part > 1)
            throw runtime_error("Part of freeze cubes must be between 0 and 1");
        if (T <= 0)
            throw runtime_error("Period of cube generation must be greater then zero");
        if (speed_min <= 0)
            throw runtime_error("The minimum speed must be greater then zero");
        if (speed_max < speed_min)
            throw runtime_error("The maximum speed must be greater than the minimum");
        if (w_min <= 0)
            throw runtime_error("The minimum angular speed must be greater then zero");
        if (w_max < w_min)
            throw runtime_error("The maximum angular speed must be greater than the minimum");
        if (size_min <= 0)
            throw runtime_error("The minimum size must be greater then zero");
        if (size_max < size_min)
            throw runtime_error("The maximum size must be greater than the minimum");

        std::random_device rd;
        re = default_random_engine(rd());
        re_cube_type = default_random_engine(rd());

        type_generator = std::uniform_real_distribution<double>(0, 1);
        speed_generator = std::uniform_real_distribution<double>(speed_min, speed_max);
        angular_speed_generator = std::uniform_real_distribution<double>(w_min, w_max);
        place_generator = std::uniform_real_distribution<double>(0, 1);
        target_x_generator = std::uniform_real_distribution<double>(0.3 * SCREEN_WIDTH, 0.7 * SCREEN_WIDTH);
        target_y_generator = std::uniform_real_distribution<double>(0.3 * SCREEN_HEIGHT, 0.7 * SCREEN_HEIGHT);
        size_generator = std::uniform_int_distribution<int>(size_min, size_max);
        wall_generator = std::uniform_int_distribution<int>(0, 3);
    }

private:

    /// @brief Проверка наличия куба на изображении
    bool check_cube_in_image(const vector<Vertex<double>> &points) {
        for (auto &p: points)
            if (!is_point_in_image(to_int_point(p)))
                return false;

        return true;
    }

public:

    /// @brief Двигает все кубы и удаляет вылетевшие за границу
    void move(double dt) {
        auto it = cubes.begin();
        while (it != cubes.end()) {
            it->move(dt);
            it->rotate(dt);
            while (it != cubes.end() && !check_cube_in_image(it->points)) {
                auto cur = it;
                ++it;
                cubes.erase(cur);
            }
            ++it;
        }
    }

    /// @brief Отрисовка кубов
    void draw() {
        for (auto &cube: cubes) {
            switch (cube.type) {
                case Projectile:
                    cube.fill(projectile_color, true);
                    break;
                case Bonus:
                    cube.fill(bonus_color, true);
                    break;
                case Freeze:
                    cube.fill(freeze_color, true);
                    break;
            }
        }
    }

    /// @brief Ускорить кубы в alpha раз
    void up_speed(double alpha) {
        speed_generator = std::uniform_real_distribution<double>(alpha * speed_generator.min(), alpha * speed_generator.max());
    }

    /// @brief Увеличить время появления кубов в alpha раз
    void up_T(double alpha) {
        T *= alpha;
    }

    /// @brief Генерация куба
    void generate(double dt) {
        time -= dt;
        if (time > 0 || cubes.size() >= cube_limit)
            return;
        time = T;

        int wall = wall_generator(re);
        double place = place_generator(re);
        double size = size_generator(re);
        Vertex<double> from;
        double shift = sqrt(2) * size;
        switch (wall) {
            case 0: // верхняя
                from = {place * SCREEN_WIDTH, shift};
                break;
            case 1: // правая
                from = {SCREEN_WIDTH - shift, place * SCREEN_HEIGHT};
                break;
            case 2: // нижняя
                from = {place * SCREEN_WIDTH, SCREEN_HEIGHT - shift};
                break;
            case 3: // левая
                from = {shift, place * SCREEN_HEIGHT};
                break;
        }

        double speed = speed_generator(re);
        Vertex<double> target(target_x_generator(re), target_y_generator(re));
        Vertex<double> velocity = target - from;
        velocity = velocity * (speed / velocity.mod());
        double w = angular_speed_generator(re);

        double type_val = type_generator(re_cube_type);
        CubeType type = CubeType::Projectile;
        if (type_val < bonus_part) {
            type = CubeType::Bonus;
        } else if (bonus_part + freeze_part > type_val) {
            type = CubeType::Freeze;
        }
        cubes.emplace_back(from, size, velocity, w, type);
    }

    ~CubeLauncher() = default;
};