//
//  main.c
//  HeapAllocator
//
//  Created by Luca Pascarella on 5/17/18.
//  Copyright © 2018 Luca Pascarella. All rights reserved.
//

#include "main.h"
#include "MyAlloc.h"

int main(int argc, const char * argv[]) {
    char *p1, *p2, *p3, *p4, *p5, *p6;
    
        //cout << "Begin program!\n";
    #if defined USE_PADDING_BYTES
        printf("Metadata block size: %lu bytes. Padding: %lu bytes\r\n", sizeof(METADATA_T), PADDING_BYTES_SIZE);
    #else
        printf("Metadata block size: %lu bytes. No Padding\r\n", sizeof(METADATA_T));
    #endif
        //printf("Size: %d\n", METADATA_T_ALIGNED);
    
        p1 = (char*) myMalloc(1021);
        p2 = (char*) myMalloc(148);
        myPrintFreelist();
    
        //    p1 = (char*) myMalloc(7);
        //    p2 = (char*) myMalloc(50);
    
        p3 = (char*) myMalloc(401);
        myFree(p2);
        p4 = (char*) myMalloc(123);
        p5 = (char*) myMalloc(5321);
    
        //    myFree(p4);
        myPrintFreelist();
        //    p2 = (char*) myMalloc(2);
        //    //myFree(p5);
        //    myPrintFreelist();
        //    myFree(p2);
        myFree(p3);
        myFree(p4);
        myFree(p1);
        myFree(p5);
        myPrintFreelist();
    
        //cout << "\nEnd program!\n";
        return 0;
}
