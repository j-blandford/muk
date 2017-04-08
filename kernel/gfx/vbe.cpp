#include <kernel/gfx/vbe.hpp>
#include <kernel/gfx/surface.hpp>
#include <kernel/gfx/terminus.hpp>
#include <kernel/cpu.hpp>

unsigned int frame_width, frame_height, frame_depth, frame_pitch;
uint8_t     *fb_loc;
uint8_t     *bb_loc; // back-buffer

bool         *dirty_lines; // our "dirty" buffer

void putpx(size_t x, size_t y, uint32_t color)
{
    RGBA col(color);

    if(x>=frame_width || y>=frame_height)
        return;
    unsigned where = x*(frame_depth/8) + y*frame_pitch;
    bb_loc[where + 0] = col.b;
    bb_loc[where + 1] = col.g;
    bb_loc[where + 2] = col.r;
}

void setpx(size_t x, size_t y, RGBA col)
{
    if(x>=frame_width || y>=frame_height)
        return;
        
    unsigned where = x*(frame_depth/8) + y*frame_pitch;
    bb_loc[where + 0] = col.b;
    bb_loc[where + 1] = col.g;
    bb_loc[where + 2] = col.r;
}

void fill_circle(const uint32_t x, const uint32_t y, uint16_t radius, RGBA color)
{
    while(radius > 0)
    {
        unsigned xm=0;
        int delta=1-2*radius, error=0, ym=radius;
        while(1)
        {
            setpx(x+xm, y+ym, color);
            setpx(x-xm, y+ym, color);
            setpx(x+xm, y-ym, color);
            setpx(x-xm, y-ym, color);
            setpx(x+xm-1, y+ym, color);
            setpx(x-xm+1, y+ym, color);
            setpx(x+xm-1, y-ym, color);
            setpx(x-xm+1, y-ym, color);
            error = 2 * (delta + ym) - 1;
            if ((delta < 0) && (error <= 0) && ym != 0)
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
            if(ym == 0) break;
            ym--;
        }
        radius--;
    }
}

void init_fbe(multiboot_info_t* mboot) {
    
    frame_width = mboot->framebuffer_width;
    frame_height = mboot->framebuffer_height;
    frame_depth = mboot->framebuffer_bpp;

    frame_pitch = mboot->framebuffer_pitch;

    // We have to remap the physical framebuffer address to a virtual address
    // because we are using pagetables
    uintptr_t fb_addr_base = 0xD0000000;
    uint32_t fb_phys_loc = (uint32_t)mboot->framebuffer_addr;
    size_t fb_size = frame_height*frame_pitch;

    for(size_t pg_num = 0; pg_num*PAGE_SIZE < fb_size; pg_num++) {
        map_vaddr_page(fb_addr_base + pg_num*PAGE_SIZE, fb_phys_loc + pg_num*PAGE_SIZE);
    }

    fb_loc = reinterpret_cast<uint8_t*>(fb_addr_base);
    bb_loc = (uint8_t*)kmalloc(frame_height*frame_pitch);

    dirty_lines = (bool*)kmalloc(sizeof(bool)*frame_height);
    memset(dirty_lines, false, sizeof(bool)*frame_height);
}

void test_surfaces() {
    // for(size_t i = 0; i < screen_surfaces.size(); i++) {
    //     terminal_printf("Surface %d -> pointer=%x\n", i, screen_surfaces[i]->buff_loc );

    //     for(size_t row = 0; row < 10; row++) {
    //         uint32_t address = row*screen_surfaces[i]->s_pitch;
    //         terminal_printf("(0,%d) : (%d %d %d)\n", row, screen_surfaces[i]->buff_loc[address],
    //             screen_surfaces[i]->buff_loc[address+1],
    //             screen_surfaces[i]->buff_loc[address+2] );
    //     }
    // }
}

void drawchar_transparent(unsigned char c, int x, int y, RGBA fgcolor) {
	size_t cx,cy;
	int mask[8]={1,2,4,8,16,32,64,128}; // essentially 1 << 1, 1<<2, 1<<3, 1<<4...

    uint32_t x_coord;
    uint32_t y_coord;

	uint8_t * glyph = font_terminus[c];
 
	for(cy=0;cy<16;cy++){
		for(cx=0;cx<8;cx++){
			if(glyph[cy]&mask[7-cx]) {
                x_coord = x+cx;
                y_coord = y+cy-12;

                screen_surfaces[SURF_SCREEN]->setPixel(x_coord, y_coord, fgcolor);
                *(dirty_lines+sizeof(bool)*(y_coord)) = true; // update the dirty buffer for this y-value
            }
		}
	}
}

void update_buffer(bool fullRefresh) {
    screen_surfaces[SURF_SCREEN]->apply(true);

    // Copy back buffer to front buffer where the "dirty" buffer is 1
    // we are currently marking one line dirty and updating the line
    // this could be improved by creating little pixels of ~25x25 for the dirty buffer
    for(size_t y = 0; y < frame_height; y++ ) {
        if(fullRefresh || dirty_lines[y]) {
            memcpy(&(fb_loc[y*frame_pitch]), bb_loc + y*frame_pitch, frame_pitch);
        }
    }

    // reset the dirty buffer
    memset(dirty_lines, false, sizeof(bool)*frame_height);
}