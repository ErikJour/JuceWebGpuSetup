const MAT_ONOFFBUTTON  : u32 = 0u;
const TOPLEFTCUBE      : u32 = 1u;
const TOPCENTERCUBE    : u32 = 2u;
const TOPRIGHTCUBE     : u32 = 3u;
const BOTTOMLEFTCUBE   : u32 = 4u;
const BOTTOMCENTERCUBE : u32 = 5u;
const BOTTOMRIGHTCUBE  : u32 = 6u;

struct Uniforms {
    time:       f32,
    materialId: u32,
    pressed:    f32,
}

@group(0) @binding(0) var<uniform> u: Uniforms;

struct VertexInput {
    @location(0) position: vec3f,
    @location(1) normal  : vec3f,
    @location(2) color   : vec3f
}

struct VertexOutput {
    @builtin(position) clipPos : vec4f,
    @location(0)       color   : vec3f,
    @location(1)       worldPos: vec3f,
    @location(2)       normal  : vec3f
}