#include "Engine.h"
#include <cstdlib>
#include <memory.h>
#include "draw.h"
#include "cube.h"

#include <cstdio>

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
Cube cube;

// initialize game data in this function
void initialize() {
    cube = Cube({{20.0, 20.0}, {40.0, 20.0}, {40.0, 40}, {20, 40}}, {20, 20}, 2 / M_PI);
}

// this function is called to update game data,
// dt - time elapsed since the previous update (in seconds)
void act(float dt) {
    if (is_key_pressed(VK_ESCAPE))
        schedule_quit_game();

    cube.move(dt);
    cube.rotate(dt);
//    if (is_key_pressed(VK_RIGHT)) {
//        line.move(1, dt);
//    } else if (is_key_pressed(VK_LEFT)) {
//        line.move(-1, dt);
//    }
}

// fill buffer in this function
// uint32_t buffer[SCREEN_HEIGHT][SCREEN_WIDTH] - is an array of 32-bit colors (8 bits per R, G, B)
void draw() {
    // clear backbuffer
    memset(buffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));

    cube.draw(Color(0, 255, 0));
//    int x = line.x, y = line.y;
//    for (int i = 0; i < line.h; i++) {
//        set_pixel(x, y + i, Color{255, 0, 0});
//    }
}

// free game data in this function
void finalize() {
}

