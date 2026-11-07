//
// Created by Erik Jourgensen on 7/27/26.
//

#pragma once
#include <cstdint>

static constexpr uint32_t MAT_ONOFFBUTTON  = 0;
static constexpr uint32_t TOPLEFTCUBE      = 1;
static constexpr uint32_t TOPCENTERCUBE    = 2;
static constexpr uint32_t TOPRIGHTCUBE     = 3;
static constexpr uint32_t BOTTOMLEFTCUBE   = 4;
static constexpr uint32_t BOTTOMCENTERCUBE = 5;
static constexpr uint32_t BOTTOMRIGHTCUBE  = 6;

struct MyUniforms {
    float    time;
    uint32_t materialId;
    float    pressed;
    uint32_t padding[5];
};

static_assert(sizeof(MyUniforms) % 4 == 0);
