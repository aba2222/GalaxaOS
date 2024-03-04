#ifndef __MEMORYMANAGER_H
#define __MEMORYMANAGER_H

#include "common/types.h"

namespace myos {
    struct MemoryChunk {
        MemoryChunk* next;
        MemoryChunk* prev;
        bool allocated;
        common::size_t size;
    };

    class MemoryManager {
    public:
        MemoryManager(common::size_t start, common::size_t size);
        ~MemoryManager();

        void* malloc(common::size_t size);
        void free(void* ptr);

        static MemoryManager* activeMemoryManager;
    private:
        MemoryChunk* first;
    };
    void *memcpy(void * _dest, void *_src, common::uint32_t _n);
}

void* operator new(myos::common::size_t size);
void* operator new[](myos::common::size_t size);

void* operator new(myos::common::size_t size, void* ptr);
void* operator new[](myos::common::size_t size, void* ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);


#endif