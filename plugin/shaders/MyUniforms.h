//
// Created by Erik Jourgensen on 7/27/26.
//

//==================================================================
//We will use these once we have some objects and interactions setup
//==================================================================
#pragma once
#include <cstdint>

struct MyUniforms {
    float    time;
    float    pressed;
    float    buttonX;
};

static_assert(sizeof(MyUniforms) % 4 == 0);
