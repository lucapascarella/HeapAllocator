# MyAlloc
MyAlloc is an optimized and portable heap memory allocator designed to fit devices with small memory footprint.

## Description
This project implements a custom and optimized version of a dynamic memory allocator designed to fit into PIC32MZ2064DAH176 microcontroller and provide simplified access to the internal DDR2 RAM memory.
The project is developed on MAC OS and tested with a PIC32MZ2064DAH176 real environment.

However, when used with PIC32MZ DA microcontrollers the user must care about cachable addresses. For example, the DMA can only access to uncached addresses causing memory inconsistencies when the user code access to the same space through cached addresses.  
The _CACHE_LINE_SIZE_ (set to 16 bytes) respects the cache line alignment of PIC32MZ2064DAH176. Therefore, the user code can call `SYS_DEVCON_DataCacheClean(dst, size);` and `SYS_DEVCON_DataCacheInvalidate(dst, size);` to clean and invalidate the cache, respectively.

## Usage examples
The dynamic memory management acts as the standard one providing the functionalities of the `<stdlib.h>`. A memory release must follow any dynamic memory allocation. Incorrect handling of the allocated memory can create soon or far a memory leak with a fail in allocating new space.

### Dynamic memory allocation
The following code requests, at least, 35 bytes in the heap section. In case of success, the returned pointer differs to NULL and points to the assigned space.

```C
char *p;
if ((p = (char*) myMalloc(35)) != NULL) {
    // Here you can safetly use p
}
```

### Dynamic memory release
To release a dynamic allocated memory just call `myFree()` such as in the `<stdlib.h>`.
```C
if (p != NULL) {
    myFree(p);
}
```

## License
Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. You may obtain a copy of the License at
 
[https://www.apache.org/licenses/LICENSE-2.0](https://www.apache.org/licenses/LICENSE-2.0)
 
Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License for the specific language governing permissions and limitations under the License.

### References
Sponsor: [LP Systems](https://lpsystems.eu/)
Author: [Luca Pascarella](https://lucapascarella.com/)

