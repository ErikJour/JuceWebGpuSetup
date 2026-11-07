@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {

    //We call different fragment functions depending on the material.
    switch u.materialId {
             case MAT_ONOFFBUTTON:  { return fragmentOnOff(in);         }
             case MAT_PLANE:        { return fragmentPlane(in);         }
             default:               { return vec4f(1.0, 0.0, 1.0, 1.0); }
        }

}