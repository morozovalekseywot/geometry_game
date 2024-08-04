#include "Engine.h"
#include <memory.h>
#include "draw.h"
#include "cube.h"
#include "circle.h"
#include "mathematics.h"
#include "rotator.h"
#include "cube_launcher.h"
#include "game_logic.h"

//
//  You are free to modify this file
//

//  is_key_pressed(int button_vk_code) - check if a key is pressed,
//                                       use keycodes (VK_SPACE, VK_RIGHT, VK_LEFT, VK_UP, VK_DOWN, VK_RETURN)
//
//  get_cursor_x(), get_cursor_y() - get mouse cursor position
//  is_mouse_button_pressed(int button) - check if mouse button is pressed (0 - left button, 1 - right button)
//  schedule_quit_game() - quit game after act()

// (0, 0) - сверху слева

int score = 0;
GameLogic game_logic;

// initialize game data in this function
void initialize() {
    double R = 300, r = 40, w = 2 * M_PI / 5;
    int count = 2;
    Rotator rotator({SCREEN_WIDTH / 2.0, SCREEN_HEIGHT / 2.0}, R, r, w, count);

    double bonus_part = 0.3, freeze_part = 0.4, T = 3;
    double speed_min = 100, speed_max = 150;
    double w_min = 2 * M_PI / 5, w_max = 2 * M_PI / 2;
    int size_min = 20, size_max = 40;
    CubeLauncher cube_launcher(bonus_part, freeze_part, T, speed_min, speed_max, w_min, w_max, size_min, size_max);

    game_logic = GameLogic(rotator, cube_launcher);
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt) {
    if (is_key_pressed(VK_ESCAPE))
        schedule_quit_game();

    if (is_key_pressed(VK_SPACE))
        game_logic.change_direction();

    game_logic.actions(dt);
    if (!game_logic.update_score()) {
        cout << "\nYOU LOOSE!!!\n";
        schedule_quit_game();
    }

    if (game_logic.get_score() != score) {
        score = game_logic.get_score();
        cout << "Your score is: " << score << '\n';
    }
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw() {
    // clear backbuffer
    memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
    draw_bounds();

    game_logic.draw();
    draw_bounds();
}

// free game data in this function
void finalize() {
}

