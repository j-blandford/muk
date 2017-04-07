#pragma once

#include <stdint.h>
#include <kernel/multiboot.hpp>
#include <kernel/memory/alloc.hpp>
#include <kernel/memory/heap.hpp>

#include <kernel/gfx/vga.hpp>
#include <kernel/gfx/vbe.hpp>

#include <kernel/cpu.hpp>


// This is my Surface class. It contains a small section of memory which can be blitted onto the back buffer. 
//       *** backbuffer ****
//   <--------  frame_width -->
//  |-------------------------| ^
//  |         Vec2 pos(x,y)   | |
//  |         |----------|    | |
//  |         | dim.x    |    |  frame_height
//  |         | dim.y    |    | |
//  |         |----------|    | |
//  |-------------------------| V

class Surface {
    
    uint8_t z_index;

public:
    uint8_t* buff_loc;
    bool* dirty_buffer; // per window dirty buffer for speedy updates!

    uint32_t s_pitch;
    Vector2 pos;
    Vector2 dim;

    Surface() {}
    Surface(Vector2 pos, Vector2 dim);

    ~Surface() {}

    void apply(bool fullRefresh = true); // Apply the Surface to the backbuffer

    void scrollUp(size_t num_lines); // this needs to be inside a TTY:: class (which will embed a Surface object within it)

    void bringToFront();
    void setZindex(uint8_t z_index);

    void setPixel(uint32_t x, uint32_t y, RGBA color); // Set a single pixel
    void setBackground(RGBA bg_color);
    void drawCircle(uint32_t x, uint32_t y, uint16_t radius, RGBA color);
};

extern std::vector<Surface*> screen_surfaces;
void start_display_driver(multiboot_info_t* mboot);
void surface_update();
void init_screens();