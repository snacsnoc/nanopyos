#include <stdint.h>

#define FRAMEBUFFER_ADDR 0x8E000000
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// Set a pixel's color in the framebuffer
void set_pixel(int x, int y, uint32_t color) {
    uint32_t* framebuffer = (uint32_t*)FRAMEBUFFER_ADDR;
    framebuffer[y * SCREEN_WIDTH + x] = color;
}

// Clear the screen with a specific color
void clear_screen(uint32_t color) {
    for (int y = 0; y < SCREEN_HEIGHT; ++y) {
        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            set_pixel(x, y, color);
        }
    }
}
