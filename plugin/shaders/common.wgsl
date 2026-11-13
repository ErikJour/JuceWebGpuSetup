//Material list
const MAT_ONOFFBUTTON  : u32 = 0u;
const TOPLEFTCUBE      : u32 = 1u;
const TOPCENTERCUBE    : u32 = 2u;
const TOPRIGHTCUBE     : u32 = 3u;
const BOTTOMLEFTCUBE   : u32 = 4u;
const BOTTOMCENTERCUBE : u32 = 5u;
const BOTTOMRIGHTCUBE  : u32 = 6u;

//Some colors
const animatedGreen = vec4(0.35, 0.67, 0.2, 1.0);
const animatedBlack = vec4(0.0, 0.0, 0.0, 1.0);
const animatedRed   = vec4(0.5, 0.07, 0.04, 1.0);

struct Uniforms {
    time:           f32,
    materialId:     u32,
    pressed:        f32,
    tick:           u32,
    modelMatrix:    mat4x4f,
    viewProjMatrix: mat4x4f,
    aspectRatio:    f32,
    projMatrix:     mat4x4f,
    cameraPosition: vec3f
}

@group(0) @binding(0) var<uniform> u: Uniforms;

struct VertexInput {
    @location(0) position: vec3f,
    @location(1) normal  : vec3f,
    @location(2) color   : vec3f
}

struct VertexOutput {
    //Clip space, used by the rasterizer
    @builtin(position) clipPos   : vec4f,
    @location(0)       color     : vec3f,
    @location(1)       worldPos  : vec3f,
    @location(2)       normal    : vec3f,
    @location(3)       objectPos : vec3f
}