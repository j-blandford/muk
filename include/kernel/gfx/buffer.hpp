#pragma once

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include <std.hpp>
#include <kernel/multiboot.hpp>

namespace Graphics {
    class RGB {
    public:
        uint8_t r;
        uint8_t g;
        uint8_t b;

        RGB(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) { }
        RGB(uint32_t hex) {
            (*this).r = hex >> 16;
            (*this).g = hex >> 8;
            (*this).b = hex;
        }

        RGB() : r(0xFF), g(0xFF), b(0xFF) { } // default to pure white if values aren't specified
        ~RGB() { }
    };

    class RGBA : public RGB {
    public:
        uint8_t a;

        RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : RGB(r,g,b), a(a) { }
        RGBA(uint32_t hex) {
            (*this).r = hex >> 16;
            (*this).g = hex >> 8;
            (*this).b = hex;

            (*this).a = 0xFF; // this constructor only supports 32-bit colour, no transparency
        }
        RGBA() : RGB(0xFFFFFF), a(0xFF) { } // default to pure white if values aren't specified

        ~RGBA() { }
    };

    class Vector2 {
    public:
        uint32_t x;
        uint32_t y;

        Vector2() {
            (*this).x = 0;
            (*this).y = 0;
        }    
        Vector2(uint32_t x, uint32_t y ) {
            (*this).x = x;
            (*this).y = y;
        }
    };

    enum class Update {
        DELTA_REFRESH = 0,
        FULL_REFRESH = 1
    };

    /**
    *  This is a wrapper class for the front and back buffers.
    */    
    class ScreenBuffer {
        uint8_t* address;
        RGB* pixels; // todo! dont need to worry about the pitch then
        size_t width, height, depth;
        size_t pitch;
    public:
        ScreenBuffer() { }
        ScreenBuffer(uint8_t* memory_addr, size_t buffer_width, size_t buffer_height, size_t buffer_depth)
        : address(memory_addr)
        , width(buffer_width)
        , height(buffer_height)
        , depth(buffer_depth)
        , pitch(height*depth) { }

        ~ScreenBuffer() { }

        uint8_t* GrabPointer() const {
            return address;
        }

        void CopyFrom(ScreenBuffer& buffer, Update update_method);
        void Copy(uint8_t dest_idx, uint8_t* source, size_t size);
        void SetPixel(size_t x, size_t y, RGB col);
    };

    extern ScreenBuffer front_buffer;
    extern ScreenBuffer back_buffer;
    extern bool* dirty_buffer; // dirty buffer
    
    extern size_t frame_width;
    extern size_t frame_height;
    extern size_t frame_depth;
    extern size_t frame_pitch;

    void Setup(multiboot_info_t* mboot);
    void UpdateBuffers(Update update_method = Update::FULL_REFRESH);
}

void update_buffer();

// Font drawing and display functions
void drawchar_transparent( unsigned char c, int x, int y, Graphics::RGB fgcolor);