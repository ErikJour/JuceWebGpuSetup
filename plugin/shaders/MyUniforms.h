//
// Created by Erik Jourgensen on 7/27/26.
//

//==================================================================
//We will use these once we have some objects and interactions setup
//==================================================================
#pragma once
#include <cstdint>

static constexpr uint32_t MAT_ONOFFBUTTON  = 0;

struct MyUniforms {
    float    time;
    uint32_t materialId;
    float    pressed;
    float    buttonX;
};

static_assert(sizeof(MyUniforms) % 4 == 0);
