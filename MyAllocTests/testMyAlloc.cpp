//
//  testMyAlloc.cpp
//  TestMyAlloc
//
//  Created by Luca Pascarella on 12/7/18.
//  Copyright © 2018 Luca Pascarella. All rights reserved.
//

#include "catch.hpp"
#include "MyAlloc.h"

TEST_CASE("Testing MyAlloc 1") {
    
    char *p1;
    
    SECTION("Auto initialization test") {
        INFO("Returned value must not be NULL") // Only appears on a FAIL
        p1 = (char*) myMalloc(123);
        REQUIRE(p1 != NULL);
        myFree(p1);
    }
    
    SECTION("Correcte size allocation") {
        INFO("Must return the asked size") // Only appears on a FAIL
        p1 = (char*) myMalloc(123);
        REQUIRE(p1 != NULL);
        size_t size = MyAlloc_GetRequestedSize(p1);
        REQUIRE(size == 123);
        myFree(p1);
    }
}

#define ALLOC_MAX       12
TEST_CASE("Testing free/full space") {
    char *p1, *p2, *p3, *p4;
    char *p[ALLOC_MAX];
    int i, space = 0;
    
    SECTION("Multiple allocation test 1") {
        INFO("Ordinary allocation failed") // Only appears on a FAIL
        for (i = 0; i < ALLOC_MAX; i++) {
            p[i] = (char*) myMalloc(i+1);
            REQUIRE(MyAlloc_GetRequestedSize(p[i]) == i+1);
            space += MyAlloc_GetTotalSize(p[i]);
        }
        REQUIRE(MyAlloc_GetFullNonLinearSpace() == space);
        for (i = 0; i < ALLOC_MAX; i++) {
            myFree(p[i]);
        }
        REQUIRE(MyAlloc_GetFreeNonLinearSpace() == MAX_HEAP_SIZE);
    }
    
    SECTION("Multiple allocation test 1") {
        INFO("Ordinary allocation failed") // Only appears on a FAIL
        for (i = 0; i < ALLOC_MAX; i++) {
            p[i] = (char*) myMalloc(i+1);
            REQUIRE(MyAlloc_GetRequestedSize(p[i]) == i+1);
            space += MyAlloc_GetTotalSize(p[i]);
        }
        REQUIRE(MyAlloc_GetFullNonLinearSpace() == space);
        for (i = ALLOC_MAX-1; i >= 0; i--)
            myFree(p[i]);
        REQUIRE(MyAlloc_GetFreeNonLinearSpace() == MAX_HEAP_SIZE);
    }
    
    SECTION("Multiple allocation test 2") {
        INFO("Random allocation") // Only appears on a FAIL
        p1 = (char*) myMalloc(15);
        REQUIRE(p1 != NULL);
        REQUIRE(MyAlloc_GetRequestedSize(p1) == 15);
        p2 = (char*) myMalloc(23);
        REQUIRE(p2 != NULL);
        REQUIRE(MyAlloc_GetRequestedSize(p2) == 23);
        myFree(p1);
        p1 = (char*) myMalloc(32);
        REQUIRE(p1 != NULL);
        REQUIRE(MyAlloc_GetRequestedSize(p1) == 32);
        p3 = (char*) myMalloc(8);
        REQUIRE(p3 != NULL);
        REQUIRE(MyAlloc_GetRequestedSize(p3) == 8);
        
        REQUIRE(MyAlloc_GetFullNonLinearSpace() == MyAlloc_GetTotalSize(p1) + MyAlloc_GetTotalSize(p2) + MyAlloc_GetTotalSize(p3));
        myFree(p1);
        myFree(p2);
        myFree(p3);
        REQUIRE(MyAlloc_GetFreeNonLinearSpace() == MAX_HEAP_SIZE);
    }
    
    SECTION("Multiple allocation test 3") {
        INFO("Random complex allocation") // Only appears on a FAIL
        p1 = (char*) myMalloc(15);
        REQUIRE(p1 != NULL);
        REQUIRE(MyAlloc_GetRequestedSize(p1) == 15);
        p2 = (char*) myMalloc(23);
        REQUIRE(p2 != NULL);
        REQUIRE(MyAlloc_GetRequestedSize(p2) == 23);
        myFree(p1);
        p1 = (char*) myMalloc(32);
        REQUIRE(p1 != NULL);
        REQUIRE(MyAlloc_GetRequestedSize(p1) == 32);
        p3 = (char*) myMalloc(8);
        REQUIRE(p3 != NULL);
        REQUIRE(MyAlloc_GetRequestedSize(p3) == 8);
        myFree(p1);
        myFree(p2);
        p4 = (char*) myMalloc(64);
        REQUIRE(p3 != NULL);
        REQUIRE(MyAlloc_GetRequestedSize(p4) == 64);
        p1 = (char*) myMalloc(17);
        REQUIRE(p1 != NULL);
        REQUIRE(MyAlloc_GetRequestedSize(p1) == 17);
        p2 = (char*) myMalloc(34);
        REQUIRE(p2 != NULL);
        REQUIRE(MyAlloc_GetRequestedSize(p2) == 34);
        myFree(p4);
        myFree(p3);
        myFree(p2);
        myFree(p1);
        REQUIRE(MyAlloc_GetFreeNonLinearSpace() == MAX_HEAP_SIZE);
    }
}

//TEST_CASE("Testing Sample Class") {
//    SampleClass sc;
//
//    sampleC();
//
//
//    SECTION("setting the str") {
//        Sample2 s = Sample2();
//        s.doSomething();
//
//        INFO("Using TestStr") // Only appears on a FAIL
//        sc.setStr("TestStr");
//        CAPTURE(sc.getStr()); // Displays this variable on a FAIL
//
//        CHECK(sc.getStr() == "TestStr");
//    }
//
//    SECTION("test bigStr") {
//        sc.setStr("TestStr");
//        REQUIRE(sc.bigStr() == "TestStr : 8");
//    }
//
//    SECTION("Test doubles") {
//        sc.setD(1);
//        CHECK(sc.getD() == 1);
//        sc.setD(1.0/3.0);
//        CHECK(sc.getD() == Approx(0.33333)); // Nice
//    }
//}
