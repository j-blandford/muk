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

class ISurface {
    public:
    virtual ~ISurface() {}

    virtual void apply(bool full_refresh) = 0;
    virtual void setPixel(uint32_t x, uint32_t y, RGB color) = 0;
    virtual void bringToFront() = 0;
    virtual void setZindex(uint8_t z_index) = 0;
    virtual void setBackground(RGB bg_color) = 0;
};

class BaseSurface : public ISurface {
    uint8_t z_index;
public:
    uint8_t* buff_loc;
    bool* dirty_buffer; // per window dirty buffer for speedy updates!

    uint32_t s_pitch;
    Vector2 pos;
    Vector2 dim;

    BaseSurface(Vector2 pos, Vector2 dim);
    ~BaseSurface() {}

    void apply(bool full_refresh);
    void setPixel(uint32_t x, uint32_t y, RGB color);
    void bringToFront();
    void setZindex(uint8_t z_index);
    void setBackground(RGB bg_color);

    void scrollUp(size_t num_lines); // this needs to be inside a TTY:: class (which will embed a Surface object within it)
    void drawCircle(uint32_t x, uint32_t y, uint16_t radius, RGB color);
};

extern std::vector<BaseSurface*> screen_surfaces;

void start_display_driver(multiboot_info_t* mboot);
void surface_update();
void init_screens();