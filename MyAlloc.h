/* ************************************************************************** */
/** Descriptive File Name
 
 @Company
 Luca Pascarella
 
 @File Name
 MyAlloc.h
 
 @Summary
 Custom memory allocator.
 
 @Description
 This file is the header of a custom memory allocator designed for low-end MCU.
 In the header file the user can select between two allocation strategies USE_FISRT_FIT and USE_BEST_FIT.
 Moreover CACHE_LINE_SIZE enables a padding before the structure allocation to prevent cache memory allignment invalidation.
 
 */
/* ************************************************************************** */

#ifndef _MY_ALLOC_H    /* Guard against multiple inclusion */
#define _MY_ALLOC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
///#include "system_config.h"
///#include "system_definitions.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    
    
    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */
    
    /*
     * These constants should be manually define before use this functions.
     */
    
    // X-code definitions
#define DDR_SIZE                1024 * 10
    // End x-code definitions
    
#define EXAMPLE_CONSTANT 0
    
#define MAX_HEAP_SIZE           DDR_SIZE
    ///#define HEAP_START_ADDRESS      (0xA8000000)
    
    
    // On 32-bit machines a 4 byte is typical. 8 is used on 64-bit instead
#define WORD_SIZE               4
    
    // single word (4) or double word (8) alignment
#define ALIGNMENT               WORD_SIZE   /* typically, single word on 32-bit systems and double word on 64-bit systems */
    
    // rounds up to the nearest multiple of ALIGNMENT
#define ALIGN(size)             (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define METADATA_T_ALIGNED      (ALIGN(sizeof(METADATA_T)))
    
    //#define USE_FIRST_FIT
#define USE_BEST_FIT
#if defined USE_FIRST_FIT && defined USE_BEST_FIT
#error "Only one algorithm at time can be choosen."
#endif
    
    
    
    
    
    // A preload of dummy bytes is added into the head of each block of the linked list to prevent cache lines inconstistencies
    // When the used of the previous block flushs or invalidates the cache content there are not consequence on the current block
    //#define USE_CACHE_LINE_BYTES
#define CACHE_LINE_SIZE             16  // 16 bytes (4 words) for PIC32MZ DA
    
    typedef struct METADATA_T_TEMP {
        uint32_t free;
        struct METADATA_T_TEMP *prev;
        struct METADATA_T_TEMP *next;
    } METADATA_T_TEMP;
    
#if defined USE_CACHE_LINE_BYTES
#define MEATA_SIZE                  sizeof(METADATA_T_TEMP)
#define USE_PADDING_BYTES
#define PADDING_BYTES_SIZE          CACHE_LINE_SIZE - MEATA_SIZE
#endif
    
    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************
    
    /*
     * This structure is used to hold block meta-data linked list.
     * There are two pointers, next and previous structures in the list
     * A boolean type indicates, if the block is free (true) or used (false)
     */
    typedef struct METADATA_T {
#if defined USE_PADDING_BYTES
        uint8_t dummy[PADDING_BYTES_SIZE];
#endif
        struct {
            uint32_t size : 31; // Max size 2 GBytes
            uint32_t free : 1;
        };
        struct METADATA_T *prev;
        struct METADATA_T *next;
    } METADATA_T;
    
    typedef struct {
        METADATA_T* blocklist;
        size_t heapStartAddress;
        size_t heapEndAddress;
        size_t heapSize;
        size_t requests;
    } MY_ALLOC;
    
    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************
    
    static void myMalloc_Initialization(void);
    static size_t getBlockSize(METADATA_T* block);
    static METADATA_T* algorithmFirstFit(METADATA_T* current, size_t length);
    static METADATA_T* algorithmBestFit(METADATA_T* current, size_t length);
    
    
    void* myMalloc(size_t length);
    void myFree(void* ptr);
    void myPrintFreelist(void);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _MY_ALLOC_H */

/* *****************************************************************************
 End of File
 */
