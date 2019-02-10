//
//  main.cpp
//  MyAlloc
//
//  Created by Luca Pascarella on 12/7/18.
//  Copyright © 2018 Luca Pascarella. All rights reserved.
//

#include <iostream>
#include "MyAlloc.h"

static void printSize(void *ptr) {
    size_t req = MyAlloc_GetRequestedSize(ptr);
    size_t assign = MyAlloc_GetAssignedSize(ptr);
    size_t tot = MyAlloc_GetTotalSize(ptr);
    printf("Size: %ld => %ld (%ld)\r\n", req, assign, tot);
}

int main(int argc, const char * argv[]) {
    char *p1, *p2, *p3;
    
    p1 = (char*) myMalloc(35);
    printSize(p1);
 
    p2 = (char*) myMalloc(36);
    printSize(p2);

    p3 = (char*) myMalloc(37);
    printSize(p3);
    
    printf("All free/full space: %ld/%ld\r\n", MyAlloc_GetFreeNonLinearSpace(), MyAlloc_GetFullNonLinearSpace());
    
    myFree(p1);
    myFree(p2);
    myFree(p3);
    
    printf("All free/full space: %ld/%ld\r\n", MyAlloc_GetFreeNonLinearSpace(), MyAlloc_GetFullNonLinearSpace());
    
    MyAlloc_PrintFreelist();
    
    return 0;
}
