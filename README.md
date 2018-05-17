# HeapAllocator
Portable Heap Allocator

This custom version of memory allocator has beed created to handle the DDR2 RAM of a PIC32MZ2048EFH064.
It has been tested only on this microcontroller.

The cache CACHE_LINE_SIZE (set to 16 bytes) respects the cache line size of the PIC32MZ2048EFH064.
