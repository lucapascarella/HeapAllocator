/** **************************************************************************
 @Company
 LP Systems https://lpsystems.eu
 
 @File Name
 MyAlloc.c
 
 @Author
 Luca Pascarella https://lucapascarella.com
 
 @Summary
 Implementation of a custom memory allocator.
 
 @Description
 This file contains the implementation of a custom memory allocator designed for low-end MCUs.
 In the header file the user can select between two allocation strategies USE_FISRT_FIT and USE_BEST_FIT.
 Moreover CACHE_LINE_SIZE enables a padding before the structure allocation to prevent cache memory allignment invalidation.
 
 @License
 Copyright (C) 2016 LP Systems
 
 Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
 in compliance with the License. You may obtain a copy of the License at
 
 https://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software distributed under the License
 is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 or implied. See the License for the specific language governing permissions and limitations under
 the License.
 ************************************************************************** */


#include "MyAlloc.h"

static char heap[MAX_HEAP_SIZE];
static MY_ALLOC myAlloc;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Internal Functions                                                */
/* ************************************************************************** */
/* ************************************************************************** */


/**
 @Function
 void myMalloc_Initialization (void)
 
 @Summary
 Initialization function used to prepare the linked list.
 
 @Description
 This function assigns the linked list to the new heap area
 and initialize the head of the linked list.
 
 @Precondition
 None.
 
 @Parameters
 None.
 
 @Remarks
 To use this functions you must declare the start heap address.
 Use #define HEAP_START_ADDRESS to specify a physical address.
 */
static void myMalloc_Initialization(void) {
    // Assign the head of the linked list to the destination heap
    myAlloc.heapStartAddress = (size_t)(&heap);
    myAlloc.heapEndAddress = (size_t)(&heap) + ALIGN(MAX_HEAP_SIZE);
    myAlloc.heapSize = ALIGN(MAX_HEAP_SIZE);
    myAlloc.requests = 0;
    myAlloc.blocklist = (METADATA_T*) heap;
    // Initialize chain fields
    myAlloc.blocklist->next = NULL;
    myAlloc.blocklist->prev = NULL;
    myAlloc.blocklist->size = 0;
    myAlloc.blocklist->free = true; // Define the initial memory status, all free
    
    // Debug info
#ifdef MY_ALLOC_PRINT_DEBUG_INFO
    printf("Heap start address: %p 0x%lX\r\n", myAlloc.blocklist, myAlloc.heapStartAddress);
    printf("Heap end address: 0x%lX\r\n", myAlloc.heapEndAddress);
    printf("Heap size: %lu bytes\r\n", myAlloc.heapSize);
    
    printf("Heap header size: %lu bytes\r\n", sizeof(METADATA_T));
#endif
}

/**
 @Function
 static size_t getBlockSize(METADATA_T* block).
 
 @Summary
 Returns the block size.
 
 @Description
 This function calculates and returns the block size.
 The length is in bytes.
 
 @Precondition
 myMalloc_Initialization() must be called.
 
 @Parameters
 @param block Is the referred block which length is calculated.
 
 @Returns
 Return the length in bytes of the given block.
 */
static size_t getBlockSize(METADATA_T* block) {
    size_t size;
    
    if (block->next == NULL) {
        // No more blocks after this in the chain
        if (block->prev == NULL)
            size = myAlloc.heapSize - METADATA_T_ALIGNED; // Head (only) block
        else
            size = ((size_t)(myAlloc.heapEndAddress) - (size_t) (block)) - METADATA_T_ALIGNED;
    } else {
        // More blocks after this
        size = ((size_t) (block->next) - (size_t) (block)) - METADATA_T_ALIGNED;
    }
    return size;
}

#if defined USE_FIRST_FIT
static METADATA_T* algorithmFirstFit(METADATA_T* current, size_t length) {
    // First-fit implementation. Start from the first block
    // Increment block until we find one that is free and large enough to fit numbytes
    while (current && !(current->free && getBlockSize(current) >= length))
        current = current->next;
    return current;
}
#elif defined USE_BEST_FIT
static METADATA_T* algorithmBestFit(METADATA_T* current, size_t length) {
    // Best-fit implementation. Find the smallest block by inspecting the size of all blocks
    METADATA_T* smallest = NULL;
    size_t block_size, size = myAlloc.heapSize;
    while (current) {
        if (current->free) {
            block_size = getBlockSize(current);
            if (block_size >= length && block_size < size) {
                size = block_size;
                smallest = current;
            }
        }
        current = current->next;
    }
    return smallest;
}
#endif

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Public Functions                                                  */
/* ************************************************************************** */
/* ************************************************************************** */


/**
 @Function
 void* myMalloc(size_t size)
 
 @Summary
 Returns a pointer to a new memory allocated block.
 
 @Description
 This function returns a void pointer to a new memory allocated block of size size.
 
 @Precondition
 None.
 
 @Parameters
 @param size Is the minimum size of the allocated block.
 
 @Returns
 Returns a not NULL pointer if the funciont successes.
 Returns a NULL pointer if the function fails.
 */
void* myMalloc(size_t size) {
    
    size_t length;
    METADATA_T* current;
    
    // Initialize pointer chain
    if (myAlloc.blocklist == NULL)
        myMalloc_Initialization();
    
    // Check required space is bigger than zero
    if (size <= 0)
        return NULL;
    
    // Round up requested bytes to be compatible with word processor allignment
    // This is not used for cache lines boundaries (see padding bytes instead)
    length = ALIGN(size);
    
    // Free space research algorithm
#if defined USE_FIRST_FIT
    current = algorithmFirstFit(myAlloc.blocklist, length);
#elif defined USE_BEST_FIT
    current = algorithmBestFit(myAlloc.blocklist, length);
#endif
    
    // Check that current is a valid METADATA_T* pointer, space may be over
    if (current == NULL)
        return NULL;
    
    // Block found. Mark it as allocated
    current->free = false;
    current->size = (uint32_t)size;
    
    // Check if block size is large enough to split
    if (getBlockSize(current) >= (length + METADATA_T_ALIGNED)) {
        // Create a new free block in current's extra space
        METADATA_T* newblock = (METADATA_T*) (((char*) (current)) + length + METADATA_T_ALIGNED);
        newblock->free = true;
        newblock->prev = current;
        newblock->next = current->next;
        if (newblock->next)
            newblock->next->prev = newblock;
        // Refine current block's data
        current->next = newblock;
    }
    
    // Return a pointer to the beginning of the newly allocated block
    void *rtn = (void*) ((METADATA_T*) (((char*) (current)) + METADATA_T_ALIGNED));
    myAlloc.requests += 1;
    
    return rtn;
}

/**
 @Function
 void myFree(void* ptr)
 
 @Summary
 Release a previous allocated memory.
 
 @Description
 This function releases a previous allocated piece of dynamic memory.
 
 @Precondition
 MyMalloc must be called and returns successully.
 
 @Parameters
 @param ptr Is the pointer to the block to release.
 
 */
void myFree(void* ptr) {
    
    // Sanity check before continue
    if (ptr == NULL)
        return;
    
    // Find the block that we want to free from the pointer parameter (pointer math)
    METADATA_T* block_to_free = ((METADATA_T*) ptr) - 1;
    
    // Return if invalid block
    if (block_to_free == NULL) {
        //printf("Error block at %p not found\n", ptr);
        return;
    }
    
    // Free current block
    block_to_free->free = true;
    block_to_free->size = 0;
    
    // Coalesce after each free
    METADATA_T* previous_block = block_to_free->prev;
    METADATA_T* next_block = block_to_free->next;
    
    if (previous_block && previous_block->free) {
        // Combine previous, current, and next blocks
        if (next_block && next_block->free) {
            // Combine previous and next block
            previous_block->next = next_block->next;
            if (next_block->next)
                next_block->next->prev = previous_block;
        } else {
            // Combine previous and current blocks
            previous_block->next = next_block;
            if (next_block)
                next_block->prev = previous_block;
        }
    } else if (next_block && next_block->free) {
        // Combine current and next blocks
        block_to_free->next = next_block->next;
        if (next_block->next)
            next_block->next->prev = block_to_free;
    }
    myAlloc.requests -= 1;
}

/**
 @Function
 void MyAlloc_GetRequestedSize(void* ptr)
 
 @Summary
 Return the size of the requested memory.
 
 @Description
 This function returns the size of the requested memory.
 
 @Precondition
 MyMalloc must be called and returns successully.
 
 @Parameters
 A valid pointer to the beginning of the heap section
 
 */
size_t MyAlloc_GetRequestedSize(void* ptr) {
    // Sanity check before continue
    if (ptr == NULL)
        return 0;
    
    // Find associated METADATA_T block
    METADATA_T* block = ((METADATA_T*) ptr) - 1;
    
    return block->size;
}

/**
 @Function
 void MyAlloc_PrintFreelist(void)
 
 @Summary
 Print a summary of the heap status.
 
 @Description
 This function is available in debug mode to print a summary of the dynamic memory allocation.
 
 @Precondition
 MyMalloc must be called and returns successully.
 
 @Parameters
 None.
 
 */
#ifdef MY_ALLOC_PRINT_DEBUG_INFO
void MyAlloc_PrintFreelist(void) {
    long totalRequired =0, totalAssigned=0, totalTotal =0;
    METADATA_T *blocklist_head = myAlloc.blocklist;
    
    int i = 0;
    printf("   |                  Blocks addresses                 |        |                Space                \r\n");
    printf(" # |   Prev block   |     Current     |   Next block   | Status |  Required  |  Assigned  |   Total   \r\n");
    printf("---+----------------+-----------------+----------------+--------+------------+------------+------------\r\n");
    while (blocklist_head != NULL) {
        size_t space = getBlockSize(blocklist_head);
        size_t total;
        if ((char*) (blocklist_head->next) > (char*) (blocklist_head))
            total = ((char*) (blocklist_head->next) - (char*) (blocklist_head));
        else
            total = ((size_t)(myAlloc.blocklist) - (size_t)(blocklist_head)) + myAlloc.heapSize ;
        
        printf("%2d | %14p | %15p | %14p |  %s  | %10d | %10lu | %10lu\r\n", i++, blocklist_head->prev,
               blocklist_head, blocklist_head->next,
               blocklist_head->free ? "free" : "used", blocklist_head->size,space,  total);
        totalRequired +=blocklist_head->size;
        if (blocklist_head->free == false)
            totalAssigned += space;
        totalTotal += total;
        blocklist_head = blocklist_head->next;
    }
    printf("---+----------------+-----------------+----------------+--------+------------+------------+------------\r\n");
    printf("   |                |                 |                |        | %10ld | %10lu | %10lu\r\n", totalRequired, totalAssigned, totalTotal);
    //printf("--+----------------+-----------------+----------------+--------+------------+------------+-----------\r\n");
    printf("\r\n");
}
#endif
