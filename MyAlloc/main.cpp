//
//  main.cpp
//  MyAlloc
//
//  Created by Luca Pascarella on 12/7/18.
//  Copyright © 2018 Luca Pascarella. All rights reserved.
//

#include <iostream>
#include "MyAlloc.h"

int main(int argc, const char * argv[]) {
    char *p1;
    
    p1 = (char*) myMalloc(1021);
    size_t size = MyAlloc_GetRequestedSize(p1);
    printf("Size: %ld\n\r", size);
    
    return 0;
}
