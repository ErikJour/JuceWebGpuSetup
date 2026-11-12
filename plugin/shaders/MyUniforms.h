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
    uint32_t tick;
    float    modelMatrix[16];
    float    viewProjMatrix[16];
    float    aspectRatio;
    float    projMatrix[16];
    float    cameraPosition[3];
    float    pad[4];
};

//static_assert(sizeof(MyUniforms) % 16              == 0);
//static_assert(offsetof(MyUniforms, modelMatrix)    == 112);
//static_assert(offsetof(MyUniforms, materialId)     == 312);
static_assert(offsetof(MyUniforms, modelMatrix)    % 16 == 0);
static_assert(offsetof(MyUniforms, viewProjMatrix) % 16 == 0);

static constexpr float kIdentity[16] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
};

static_assert(sizeof(MyUniforms) % 4 == 0);
