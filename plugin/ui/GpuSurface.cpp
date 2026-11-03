#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include "webgpu/webgpu.h"
#include "GpuSurface.h"

static LRESULT CALLBACK animatedWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_NCHITTEST:   return HTTRANSPARENT; //Let mouse events fall through to JUCE
        case WM_ERASEBKGND:  return 1;             //WebGPU paints every pixel
        default:             return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

static const wchar_t* registerWindowClass(HINSTANCE hInstance) {
    static const wchar_t* className = L"AnimatedWebGpuWindow";
    static bool registered          = false;
    if (!registered) {
        WNDCLASSEXW wc   = {};
        wc.cbSize        = sizeof(wc);
        wc.style         = CS_OWNDC;
        wc.lpfnWndProc   = animatedWindowProc;
        wc.hInstance     = hInstance;
        wc.lpszClassName = className;
        RegisterClassExW(&wc);
        registered       = true;
    }
    return className;
}

NativeSurface createNativeSurface(WGPUInstance instance, double /*contentsScale*/) {
  NativeSurface result                      = {};
  HMODULE hInstance                         = nullptr;
  GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                     reinterpret_cast<LPCWSTR>(&animatedWindowProc),
                     &hInstance);
  HWND hwnd                                 = CreateWindowExW(WS_EX_NOACTIVATE,
                                                              registerWindowClass(hInstance),
                                                              L"",
                                                              WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                                                              0, 0, 1, 1,
                                                              nullptr, nullptr, hInstance, nullptr);
  if (!hwnd) return result;
  WGPUSurfaceSourceWindowsHWND hwndSource   = {};
  hwndSource.chain.next                     = nullptr;
  hwndSource.chain.sType                    = WGPUSType_SurfaceSourceWindowsHWND;
  hwndSource.hinstance                      = hInstance;
  hwndSource.hwnd                           = hwnd;
  WGPUSurfaceDescriptor surfaceDesc         = {};
  surfaceDesc.nextInChain                   = &hwndSource.chain;
  result.surface                            = wgpuInstanceCreateSurface(instance, &surfaceDesc);
  result.view                               = hwnd;
  return result;
}
