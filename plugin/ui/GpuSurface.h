//
// Created by Erik Jourgensen on 10/28/2026
//

#pragma once
#include "webgpu/webgpu.h"

//==========================================================================
//Platform implementations: GpuSurface.mm (macOS), GpuSurface.cpp (Windows)
//view is an NSView* on macOS and an HWND on Windows
//==========================================================================
struct NativeSurface {
    WGPUSurface surface = nullptr;
    void*       view    = nullptr;
};

NativeSurface createNativeSurface(WGPUInstance instance, double contentsScale);
