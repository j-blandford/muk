#include <kernel/gfx/surface.hpp>
#include <kernel/gfx/buffer.hpp>
#include <kernel/proc/thread.hpp>
#include <kernel/proc/message.hpp>
#include <kernel/proc/scheduler.hpp>
#include <kernel/cpu.hpp>
#include <kernel/tty.hpp>

std::vector<BaseSurface*> screen_surfaces;
bool surfaces_setup = false;

size_t VGA_WIDTH = 80;
size_t VGA_HEIGHT = 25;

BaseSurface::BaseSurface(Graphics::Vector2 pos, Graphics::Vector2 dim) {
    this->pos = pos;
    this->dim = dim;

    this->z_index = 1; // 1 is just above the lowest terminal buffer

    float pitchRatio = (float)Graphics::frame_pitch / (float)Graphics::frame_width;
    this->s_pitch = pitchRatio*dim.x; 

    this->buff_loc = (uint8_t*)vmalloc(dim.y*this->s_pitch);

    this->dirty_buffer = (bool*)vmalloc(dim.y*sizeof(bool));
    memset(this->dirty_buffer, false, dim.y*sizeof(bool));
}

void BaseSurface::setPixel(uint32_t x, uint32_t y, Graphics::RGB color) {
    if(x>=this->dim.x || y>=this->dim.y) return;

    unsigned int where = x*(Graphics::frame_depth/8) + y*this->s_pitch;
    
    this->buff_loc[where] = color.b;
    this->buff_loc[where + 1] = color.g;
    this->buff_loc[where + 2] = color.r;

    this->dirty_buffer[y] = true;
}

void BaseSurface::apply(bool full_refresh) {
    //
    // Apply the surface to the back buffer.
    // ------------------------------------------------------------------**
    //     PLEASE NOTE: this function doesn't actually display the Surface! 
    //              --> You need to call "update_buffer()" to display it
    // ------------------------------------------------------------------**

    if(full_refresh) {
        // just copy the whole buffer if we need to do a full surface refresh

        // todo: make this work for window sizes that aren't 100% width/height
        memcpy(Graphics::back_buffer.GrabPointer(), this->buff_loc, this->s_pitch*this->dim.y);
    } 
    else {
        // only update the incremental changes to the backbuffer
        for(size_t y = 0; y < this->dim.y; y++) {
            if(this->dirty_buffer[y]) {
                size_t where = this->pos.x*(Graphics::frame_depth/8) + (y+this->pos.y)*(this->s_pitch);

                memcpy(&Graphics::back_buffer.GrabPointer()[where], &this->buff_loc[y*this->s_pitch], this->s_pitch);
            }
        }
    }
}

void BaseSurface::scrollUp(size_t num_lines) {
    Process::Locker<Process::SpinlockMutex> str_print_locker(print_mutex, -1); // -1 => lock ALL threads
    Scheduler::lock();
    // now copy!
    size_t from_loc = num_lines*(this->s_pitch);
    size_t size_cpy = this->s_pitch * (VGA_HEIGHT)*num_lines;
    memcpy(&this->buff_loc[0], &this->buff_loc[from_loc], size_cpy);
    Scheduler::unlock();
}

void BaseSurface::bringToFront() {
    z_index = 10;
}
void BaseSurface::setZindex(uint8_t z_index) {
    this->z_index = z_index;
}

void BaseSurface::setBackground(Graphics::RGB bg_color) {
    for(size_t x = 0; x < this->dim.x; x++) {
        for(size_t y = 0; y < this->dim.y-1; y++) {
            this->setPixel(x, y, bg_color);
        }
    }
}

void init_screens(multiboot_info_t* mboot) {
	screen_surfaces.push_back(new BaseSurface(Graphics::Vector2(0,0), Graphics::Vector2(mboot->framebuffer_width,mboot->framebuffer_height)));
    
    VGA_WIDTH = mboot->framebuffer_width / X_FONTWIDTH;
    VGA_HEIGHT = mboot->framebuffer_height / (Y_FONTWIDTH) - 1;

	//screen_surfaces[0]->setBackground(Graphics::RGB(0x2a2b31));
	screen_surfaces[0]->apply(true);
}

void surface_update() {
    Process::Message msg = Process::kMessageNull;

    // {
	// 	using namespace Process;
	// 	Locker<SpinlockMutex> messaging_locker(messaging_mutex, Scheduler::threadId());
	// 	Process::listen(3);
	// }

    for(;;) {

        // while((msg = Process::postbox.pop(3)) != Process::kMessageNull) {
        //     if((int)msg.data == MSG_SCROLLUP) {
        //         bcprintf("[GFX] Scrolling UP!\n");

        //     //    screen_surfaces[0]->scrollUp(1);
        //     }
        // }

        interrupts_disable();
        Scheduler::lock();
        
        Graphics::UpdateBuffers(Graphics::Update::DELTA_REFRESH);  
        
        Scheduler::unlock();
        interrupts_enable();
        Scheduler::yield();
    }
}

void start_display_driver(multiboot_info_t* mboot) {
    Graphics::Setup(mboot);
    init_screens(mboot);

    Graphics::UpdateBuffers(Graphics::Update::FULL_REFRESH);  

    surfaces_setup = true;
}