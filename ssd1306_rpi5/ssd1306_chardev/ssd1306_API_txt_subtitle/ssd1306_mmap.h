#ifndef _SSD1306_MMAP_H_
#define _SSD1306_MMAP_H_


#define OLED_CLEAR _IO('o', 0)
#define OLED_CONTRAST _IOW('o', 1, int)
#define OLED_CLEAR_FB _IO('o', 2)
#define OLED_MMAP_DRAW _IO('o', 3)
#define OLED_FB_SIZE 4096 //framebuffer size = one page size

#endif
