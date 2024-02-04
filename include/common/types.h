#ifndef __MYOS__COMMON__TYPES_H
#define __MYOS__COMMON__TYPES_H

namespace myos {
    namespace common {
        typedef char int8_t;
        typedef unsigned char uint8_t;

        typedef short int16_t;
        typedef unsigned short uint16_t;

        typedef int int32_t;
        typedef unsigned int uint32_t;

        typedef long long int64_t;
        typedef unsigned long long uint64_t;

        typedef const char* string;
        typedef uint32_t size_t;

        typedef struct multiboot_info {
            uint32_t flags;
            uint32_t mem_low;
            uint32_t mem_high;
            uint32_t boot_dev;
            uint32_t cmdline;
            uint32_t module_count;
            uint32_t module_addr;
            uint32_t syms[4];
            uint32_t mmap_len;
            uint32_t mmap_addr;
            uint32_t drives_length;
            uint32_t drives_addr;
            uint32_t config_table;
            uint32_t boot_loader_name;
            uint32_t apm_table;
            uint32_t vbe_control_info;
            uint32_t vbe_mode_info;
            uint16_t vbe_mode;
            uint16_t vbe_interface_seg;
            uint16_t vbe_interface_off;
            uint16_t vbe_interface_len;
        } __attribute__((__packed__)) multiboot_info_t;

        struct vbe_mode_info_structure {
            uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
            uint8_t window_a;			// deprecated
            uint8_t window_b;			// deprecated
            uint16_t granularity;		// deprecated; used while calculating bank numbers
            uint16_t window_size;
            uint16_t segment_a;
            uint16_t segment_b;
            uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
            uint16_t pitch;			// number of bytes per horizontal line
            uint16_t width;			// width in pixels
            uint16_t height;			// height in pixels
            uint8_t w_char;			// unused...
            uint8_t y_char;			// ...
            uint8_t planes;
            uint8_t bpp;			// bits per pixel in this mode
            uint8_t banks;			// deprecated; total number of banks in this mode
            uint8_t memory_model;
            uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
            uint8_t image_pages;
            uint8_t reserved0;
        
            uint8_t red_mask;
            uint8_t red_position;
            uint8_t green_mask;
            uint8_t green_position;
            uint8_t blue_mask;
            uint8_t blue_position;
            uint8_t reserved_mask;
            uint8_t reserved_position;
            uint8_t direct_color_attributes;
        
            uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
            uint32_t off_screen_mem_off;
            uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
            uint8_t reserved1[206];
        } __attribute__ ((packed));
    }
}


#endif