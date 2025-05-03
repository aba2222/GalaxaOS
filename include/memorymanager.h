#ifndef __MEMORYMANAGER_H
#define __MEMORYMANAGER_H

#include <stdint.h>

namespace myos {
    struct MemoryChunk {
        MemoryChunk* next;
        MemoryChunk* prev;
        bool allocated;
        size_t size;
    };

    class MemoryManager {
    public:
        MemoryManager(size_t start, size_t size);
        ~MemoryManager();

        void* malloc(size_t size);
        void free(void* ptr);

        static MemoryManager* activeMemoryManager;
    private:
        MemoryChunk* first;
    };
    void *memcpy(void * _dest, void *_src, size_t _n);
}

void* operator new(size_t size);
void* operator new[](size_t size);

void* operator new(size_t size, void* ptr);
void* operator new[](size_t size, void* ptr);

void operator delete(void* ptr);
void operator delete(void* ptr, size_t);
void operator delete[](void* ptr);
void operator delete[](void* ptr, size_t);

#endif