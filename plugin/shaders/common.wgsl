struct VertexInput {
    @location(0) position: vec3f,
    @location(1) normal:   vec3f,
    @location(2) color:    vec3f
}

struct VertexOutput {
    @builtin(position) clipPos:  vec4f,
    @location(0)       color:    vec3f,
    @location(1)       worldPos: vec3f,
    @location(2)       normal:   vec3f
}