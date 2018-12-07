//
//  testMyAlloc.cpp
//  TestMyAlloc
//
//  Created by Luca Pascarella on 12/7/18.
//  Copyright © 2018 Luca Pascarella. All rights reserved.
//

#include "catch.hpp"
#include "MyAlloc.h"

TEST_CASE("Testing MyAlloc") {

    char *p1;

    p1 = (char*) myMalloc(123);
    CHECK(p1 != NULL);
  
}
