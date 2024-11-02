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

            uint64_t framebuffer_addr;
            uint32_t framebuffer_pitch;
            uint32_t framebuffer_width;
            uint32_t framebuffer_height;
            uint8_t framebuffer_bpp;
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

        class String {
        public:
            // 默认构造函数
            String() : data(0), length(0) {}

            // 构造函数
            String(const char* str) {
                if (str) {
                    length = 0;
                    while (str[length] != '\0') {
                        length++;
                    }
                    data = new char[length + 1]; // +1 是为了存储 '\0'
                    for (size_t i = 0; i < length; i++) {
                        data[i] = str[i];
                    }
                    data[length] = '\0'; // 添加字符串结束符
                } else {
                    data = 0;
                    length = 0;
                }
            }

            // 拷贝构造函数
            String(const String& other) {
                length = other.length;
                if (length > 0) {
                    data = new char[length + 1];
                    for (size_t i = 0; i < length; i++) {
                        data[i] = other.data[i];
                    }
                    data[length] = '\0';
                } else {
                    data = 0;
                }
            }

            String(String&& other) noexcept {
                data = other.data;
                length = other.length;

                other.data = nullptr;
                other.length = 0;
            }

            // 赋值操作符
            String& operator=(const String& other) {
                if (this == &other) return *this; // 避免自赋值

                // 使用临时对象处理分配，防止部分失败造成内存泄漏
                char* newData = new char[other.length + 1];
                for (size_t i = 0; i < other.length; ++i) {
                    newData[i] = other.data[i];
                }
                newData[other.length] = '\0';

                delete[] data;  // 释放旧的data
                data = newData;
                length = other.length;
                return *this;
            }

            String& operator=(String&& other) noexcept {
                if (this != &other) {
                    delete[] data;

                    data = other.data;
                    length = other.length;

                    other.data = nullptr;
                    other.length = 0;
                }
                return *this;
            }

            String operator+(const String& other) const {
                String result;
                size_t newLength = length + other.length;
                char* newData = new char[newLength + 1];

                // 复制当前字符串
                for (size_t i = 0; i < length; ++i) {
                    newData[i] = data[i];
                }

                // 复制其他字符串
                for (size_t i = 0; i < other.length; ++i) {
                    newData[length + i] = other.data[i];
                }
                newData[newLength] = '\0';

                result.data = newData;
                result.length = newLength;

                return result;
            }

            // 获取字符串长度
            size_t Length() const {
                return length;
            }

            // 索引操作符
            char& operator[](size_t index) {
                return data[index];
            }

            const char& operator[](size_t index) const {
                return data[index];
            }

            // 析构函数
            ~String() {
                if (data) {
                    delete[] data;
                }
            }


            // 获取底层数据
            const char* c_str() const {
                return data ? data : "";
            }
        private:
            char* data;     // 字符串数据
            size_t length;  // 字符串长度
        };
    }
}


#endif