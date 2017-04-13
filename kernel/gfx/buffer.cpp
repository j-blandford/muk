#include <kernel/gfx/buffer.hpp>
#include <kernel/gfx/surface.hpp>
#include <kernel/gfx/terminus.hpp>
#include <kernel/cpu.hpp>

using namespace Graphics;

size_t Graphics::frame_width = 0;
size_t Graphics::frame_height = 0;
size_t Graphics::frame_depth = 0;
size_t Graphics::frame_pitch = 0;

ScreenBuffer Graphics::front_buffer;
ScreenBuffer Graphics::back_buffer;
bool* Graphics::dirty_buffer; // our "dirty" buffer

void ScreenBuffer::SetPixel(size_t x, size_t y, Graphics::RGB col) {
    if(x>=width || y>=height)
        return;
        
    unsigned where = x*(depth/8) + y*pitch;
    address[where] = col.b;
    address[where + 1] = col.g;
    address[where + 2] = col.r;
}

void ScreenBuffer::CopyFrom(ScreenBuffer& buffer, Update update_method) {
    // Copy buffer to another buffer where the "dirty" buffer is 1
    // we are currently marking one line dirty and updating the line
    // this could be improved by creating little pixels of ~25x25 for the dirty buffer

    for(size_t y = 0; y < height; y++ ) {
        if(update_method == Update::FULL_REFRESH || dirty_buffer[y]) {
            memcpy(address + y*frame_pitch, buffer.GrabPointer() + y*frame_pitch, frame_pitch);
        }
    }
}

void ScreenBuffer::Copy(uint8_t dest, uint8_t* source, size_t size) {
    // allows us to keep the memory address for this buffer private
    memcpy(&address[dest], source, size);
}

void Graphics::Setup(multiboot_info_t* mboot) {
    frame_width = mboot->framebuffer_width;
    frame_height = mboot->framebuffer_height;
    frame_depth = mboot->framebuffer_bpp;
    frame_pitch = mboot->framebuffer_pitch;

    // We have to remap the physical framebuffer address to a virtual address
    // because we are using pagetables
    uintptr_t fb_addr_base = 0xD0000000;
    uint32_t fb_phys_loc = (uint32_t)mboot->framebuffer_addr;
    size_t fb_size = frame_height*frame_pitch;

    for(size_t pg_num = 0; pg_num*Memory::kPageSize < fb_size; pg_num++) {
        Memory::virt_manager.MapPage(fb_addr_base + pg_num*Memory::kPageSize, fb_phys_loc + pg_num*Memory::kPageSize);
    }

    front_buffer = ScreenBuffer((uint8_t*)fb_addr_base, frame_width, frame_height, frame_depth);
    back_buffer = ScreenBuffer((uint8_t*)kmalloc(frame_height*frame_pitch), frame_width, frame_height, frame_depth);
    
    dirty_buffer = (bool*)kmalloc(sizeof(bool)*frame_height);
    memset(dirty_buffer, false, sizeof(bool)*frame_height);
}

void Graphics::UpdateBuffers(Graphics::Update update_method) {
    // Apply each surface to the backbuffer
    for(auto surf : screen_surfaces) {
        surf->apply(false);
    }
    
    // Apply the backbuffer to the user's screen
    front_buffer.CopyFrom(back_buffer, update_method);

    // reset the dirty buffer
    memset(dirty_buffer, false, sizeof(bool)*frame_height);
}

void drawchar_transparent(unsigned char c, int x, int y, Graphics::RGB fgcolor) {
	size_t cx,cy;
	int mask[8]={1,2,4,8,16,32,64,128}; // essentially 1 << 1, 1<<2, 1<<3, 1<<4...

    uint32_t x_coord;
    uint32_t y_coord;

	uint8_t* glyph = font_terminus[c];
    
	for(cy=0;cy<16;cy++){
        //bcprintf("%x\n",glyph[cy]);
		for(cx=0;cx<8;cx++){
			if(glyph[cy]&mask[7-cx]) {
                x_coord = x+cx;
                y_coord = y+cy-12;

                screen_surfaces[0]->setPixel(x_coord, y_coord, fgcolor);
                *(dirty_buffer+sizeof(bool)*y_coord) = true; // update the dirty buffer for this y-value
            }
		}
	}
}