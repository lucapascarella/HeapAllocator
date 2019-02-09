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
    
    SECTION("test bigStr") {
        INFO("Must return the asked size") // Only appears on a FAIL
        p1 = (char*) myMalloc(123);
        REQUIRE(p1 != NULL);
        size_t size = MyAlloc_GetRequestedSize(p1);
        REQUIRE(size == 123);
        myFree(p1);
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
