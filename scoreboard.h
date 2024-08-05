#pragma once

#include "draw.h"
#include "color_settings.h"

/// @brief Класс для отображения текущего счета
class Scoreboard {
    const int w = 40; ///< Ширина окна для одной цифры
    const int h = 80; ///< Высота окна для одной цифры
    const int h_2 = h / 2; ///< Половина высоты окна для одной цифры
    const int skip = w / 4; ///< Интервал между цифрами
    Vertex<int> left_up = {int(0.8 * SCREEN_WIDTH), bounds_size + skip + 2}; ///< Точка, откуда начинают рисоваться цифры

    vector<Vertex<int>> number_0() {
        return vector<Vertex<int>>{
                {w, 0},
                {0, 0},
                {0, h},
                {w, h},
                {w, 0},
        };
    }

    vector<Vertex<int>> number_1() {
        return vector<Vertex<int>>{
                {w / 2, h_2 - 10},
                {w,     0},
                {w,     h}
        };
    }

    vector<Vertex<int>> number_2() {
        return vector<Vertex<int>>{
                {0, 0},
                {w, 0},
                {w, h_2},
                {0, h_2},
                {0, h},
                {w, h}
        };
    }

    vector<Vertex<int>> number_3() {
        return vector<Vertex<int>>{
                {0, 0},
                {w, 0},
                {w, h_2},
                {0, h_2},
                {w, h_2},
                {w, h},
                {0, h},
        };
    }

    vector<Vertex<int>> number_4() {
        return vector<Vertex<int>>{
                {0, 0},
                {0, h_2},
                {w, h_2},
                {w, 0},
                {w, h},
        };
    }

    vector<Vertex<int>> number_5() {
        return vector<Vertex<int>>{
                {w, 0},
                {0, 0},
                {0, h_2},
                {w, h_2},
                {w, h},
                {0, h},
        };
    }

    vector<Vertex<int>> number_6() {
        return vector<Vertex<int>>{
                {w, 0},
                {0, 0},
                {0, h},
                {w, h},
                {w, h_2},
                {0, h_2},
        };
    }

    vector<Vertex<int>> number_7() {
        return vector<Vertex<int>>{
                {0,     0},
                {w,     0},
                {w / 2, h},
        };
    }

    vector<Vertex<int>> number_8() {
        return vector<Vertex<int>>{
                {w, 0},
                {0, 0},
                {0, h},
                {w, h},
                {w, 0},
                {w, h_2},
                {0, h_2},
        };
    }

    vector<Vertex<int>> number_9() {
        return vector<Vertex<int>>{
                {w, h},
                {w, 0},
                {0, 0},
                {0, h_2},
                {w, h_2},
        };
    }

public:

    Scoreboard() = default;

    /// @brief Отрисовка текущего счета
    void draw_score(int score_) {
        string score = to_string(score_);
        for (int i = 0; i < score.size(); i++) {
            int num = score[i] - '0';

            Vertex<int> shift = {left_up.x + i * (w + skip), left_up.y};
            vector<Vertex<int>> vec;
            switch (num) {
                case 0:
                    vec = number_0();
                    break;
                case 1:
                    vec = number_1();
                    break;
                case 2:
                    vec = number_2();
                    break;
                case 3:
                    vec = number_3();
                    break;
                case 4:
                    vec = number_4();
                    break;
                case 5:
                    vec = number_5();
                    break;
                case 6:
                    vec = number_6();
                    break;
                case 7:
                    vec = number_7();
                    break;
                case 8:
                    vec = number_8();
                    break;
                case 9:
                    vec = number_9();
                    break;
            }

            for (size_t j = 0; j < vec.size() - 1; j++) {
                draw_line(shift + vec[j], shift + vec[j + 1], score_color);
                draw_line(shift + Vertex{1, 0} + vec[j], shift + vec[j + 1] + Vertex{1, 0}, score_color);
                draw_line(shift + Vertex{0, 1} + vec[j], shift + vec[j + 1] + Vertex{0, 1}, score_color);
            }
        }

        // Рамка
        int n = score.size();
        Vertex<int> right_down = Vertex<int>{left_up.x + n * (w + skip), left_up.y + h + skip};
        left_up -= Vertex<int>{skip, skip};
        draw_line(left_up, {right_down.x, left_up.y}, score_color);
        draw_line({right_down.x, left_up.y}, right_down, score_color);
        draw_line(right_down, {left_up.x, right_down.y}, score_color);
        draw_line({left_up.x, right_down.y}, left_up, score_color);

        // Заливка
        for(int y = left_up.y + 1; y < right_down.y; y++){
            for(int x = left_up.x + 1; x < right_down.x; x++){
                if(get_pixel(x, y) != score_color){
                    set_pixel(x, y, score_background_color);
                }
            }
        }

        left_up += Vertex<int>{skip, skip};
    }
};
