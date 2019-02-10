/** **************************************************************************
 @Company
 LP Systems https://lpsystems.eu
 
 @File Name
 MyAlloc.h
 
 @Author
 Luca Pascarella https://lucapascarella.com
 
 @Summary
 Header of a custom memory allocator.
 
 @Description
 This file is the header of a custom memory allocator designed for low-end MCUs.
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

#ifndef _MY_ALLOC_H    /* Guard against multiple inclusion */
#define _MY_ALLOC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdio.h>
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
    
#define MY_ALLOC_PRINT_DEBUG_INFO // Do not use in production phase
    
#define DDR_SIZE                1024 * 1
    //#define DDR_SIZE                32 * 1024 * 1024 // PIC32 DA has 32 MBytes
    
    
#define MAX_HEAP_SIZE           DDR_SIZE
    ///#define HEAP_START_ADDRESS      (0xA8000000) // PIC32 DA
    
    
    // On 32-bit machines 4 bytes are typical. 8 bytes are used on 64-bit, instead
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
    
    
    
    
    // The head of every block contains a preload with dummy bytes to prevent cache lines inconstistencies
    // In this way, a previous block may flush or invalidate the cache content without affecting the current block
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
    // Section: Public Functions
    // *****************************************************************************
    // *****************************************************************************
    
    // Basic functions
    void* myMalloc(size_t length);
    void myFree(void* ptr);
    // Advanced functions
    size_t MyAlloc_GetRequestedSize(void* ptr);
    
    // Debug functions
    void MyAlloc_PrintFreelist(void);
    size_t MyAlloc_GetFreeNonLinearSpace(void);
    size_t MyAlloc_GetFullNonLinearSpace(void);
    size_t MyAlloc_GetAssignedSize(void* ptr);
    size_t MyAlloc_GetTotalSize(void* ptr);
    
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _MY_ALLOC_H */

/* *****************************************************************************
 End of File
 */
