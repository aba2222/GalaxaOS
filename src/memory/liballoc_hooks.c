#include "memorymanager.h"
#include "common/types.h"

using namespace myos;
using namespace myos::common;

#ifdef __cplusplus
extern "C" {
#endif

/** This function is supposed to lock the memory data structures. It
 * could be as simple as disabling interrupts or acquiring a spinlock.
 * It's up to you to decide. 
 *
 * \return 0 if the lock was acquired successfully. Anything else is
 * failure.
 */
int liballoc_lock() {

}

/** This function unlocks what was previously locked by the liballoc_lock
 * function.  If it disabled interrupts, it enables interrupts. If it
 * had acquiried a spinlock, it releases the spinlock. etc.
 *
 * \return 0 if the lock was successfully released.
 */
int liballoc_unlock() {

}

/** This is the hook into the local system which allocates pages. It
 * accepts an integer parameter which is the number of pages
 * required.  The page size was set up in the liballoc_init function.
 *
 * \return NULL if the pages were not allocated.
 * \return A pointer to the allocated memory.
 */
void* liballoc_alloc(size_t size) {
    if(MemoryManager::activeMemoryManager != NULL) {
        return MemoryManager::activeMemoryManager->malloc(size);
    } else {
        return NULL;
    }
}

/** This frees previously allocated memory. The void* parameter passed
 * to the function is the exact same value returned from a previous
 * liballoc_alloc call.
 *
 * The integer value is the number of pages to free.
 *
 * \return 0 if the memory was successfully freed.
 */
int liballoc_free(void* ptr, size_t size) {
    if (MemoryManager::activeMemoryManager != nullptr) {
        MemoryManager::activeMemoryManager->free(ptr);
        return 0;
    }
    return 1;
}

#ifdef __cplusplus
}
#endif