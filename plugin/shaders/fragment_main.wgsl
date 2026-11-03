@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {

    switch u.materialId {
         default:         { return vec4f(1.0, 0.0, 1.0, 1.0);  }
    }
}