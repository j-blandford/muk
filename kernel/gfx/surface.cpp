#include <kernel/gfx/surface.hpp>
#include <kernel/gfx/vbe.hpp>
#include <kernel/proc/thread.hpp>
#include <kernel/proc/scheduler.hpp>
#include <kernel/cpu.hpp>

std::vector<Surface*> screen_surfaces;

Surface::Surface(Vector2 pos, Vector2 dim) {
    this->pos = pos;
    this->dim = dim;

    this->z_index = 1; // 1 is just above the lowest terminal buffer

    float pitchRatio = (float)frame_pitch / (float)frame_width;
    this->s_pitch = pitchRatio*dim.x; 

    this->buff_loc = (uint8_t*)kmalloc(dim.y*this->s_pitch);

    this->dirty_buffer = (bool*)kmalloc(dim.y*sizeof(bool));
    memset(this->dirty_buffer, false, dim.y*sizeof(bool));
}

void Surface::setPixel(uint32_t x, uint32_t y, RGB color) {
    if(x>=this->dim.x || y>=this->dim.y)
        return;

    unsigned int where = x*(frame_depth/8) + y*this->s_pitch;
    
    this->buff_loc[where] = color.b;
    this->buff_loc[where + 1] = color.g;
    this->buff_loc[where + 2] = color.r;

    this->dirty_buffer[y] = true;
}

void Surface::drawCircle(uint32_t x, uint32_t y, uint16_t radius, RGB color) {
    unsigned int xm=0;
    int delta=1-2*radius, error=0, ym=radius;

    while(true)
    {
        this->setPixel(x+xm, y+ym, color);
        this->setPixel(x-xm, y+ym, color);
        this->setPixel(x+xm, y-ym, color);
        this->setPixel(x-xm, y-ym, color);

        error = 2 * (delta + ym) - 1;
        if ((delta < 0) && (error <= 0) && ym !=0)
        {
            delta += 2 * ++xm + 1;
            continue;
        }

        error = 2 * (delta - xm) - 1;
        if ((delta > 0) && (error > 0) && ym != 0)
        {
            delta += 1 - 2 * --ym;
            continue;
        }
        xm++;

        delta += 2 * (xm - ym);
        if(ym == 0) return;
        ym--;
    }
}

void Surface::apply(bool fullRefresh) {
    //
    // Apply the surface to the back buffer.
    // ------------------------------------------------------------------**
    //     PLEASE NOTE: this function doesn't actually display the Surface! 
    //              --> You need to call "update_buffer()" to display it
    // ------------------------------------------------------------------**

    for(size_t y = 0; y < this->dim.y; y++) {
        if(fullRefresh || this->dirty_buffer[y]) {
            // no SSE functions are currently implemented, so we have to copy it line by line
            size_t where = this->pos.x*(frame_depth/8) + (y+this->pos.y)*(frame_pitch); //

            memcpy(&bb_loc[where], &this->buff_loc[y*this->s_pitch], this->s_pitch);
        }
    }
}

void Surface::scrollUp(size_t num_lines) {
    for(size_t y = num_lines; y < this->dim.y - num_lines; y++) {
        size_t from_loc = this->pos.x*(frame_depth/8) + (y+this->pos.y)*(frame_pitch);
        size_t to_loc = this->pos.x*(frame_depth/8) + (y+this->pos.y-num_lines)*(frame_pitch);

        memcpy(&this->buff_loc[to_loc], &this->buff_loc[from_loc], this->s_pitch); // copy the buffer line by line
    }
}

void Surface::bringToFront() {
    z_index = 10;
}
void Surface::setZindex(uint8_t z_index) {
    this->z_index = z_index;
}

void Surface::setBackground(RGB bg_color) {
    for(size_t x = 0; x < this->dim.x; x++) {
        for(size_t y = 0; y < this->dim.y-1; y++) {
            this->setPixel(x, y, bg_color);
        }
    }
}

void init_screens() {
	screen_surfaces.push_back(new Surface(Vector2(0,0), Vector2(frame_width,frame_height)));
	
	screen_surfaces[SURF_SCREEN]->setBackground(RGB(0x2a2b31));
	screen_surfaces[SURF_SCREEN]->apply(true);

    //test_surfaces();

	// screen_surfaces.push_back(Surface(Vector2(250,250), Vector2(50,50)));
	// screen_surfaces[1].setBackground(RGBA(0xFFFFFF));
	// screen_surfaces[1].drawCircle(25, 25, 20, RGBA(0xFF0000));
	// screen_surfaces[1].apply();

}

void surface_update() {
    for(;;) {
        //Scheduler::lock();
        interrupts_disable();
        update_buffer(false);
        interrupts_enable();
        //Scheduler::yield();
    }
}

void start_display_driver(multiboot_info_t* mboot) {
    init_fbe(mboot);
    init_screens();

    update_buffer(true);  
}