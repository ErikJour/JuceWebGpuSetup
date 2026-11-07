@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4f {

    //Material-dependant fragment functions.
    switch u.materialId {
             case MAT_ONOFFBUTTON:  { return fragmentOnOff(in);          }
             case TOPLEFTCUBE:      { return fragmentTopLeftCube(in);    }
             case TOPCENTERCUBE:    { return fragmentTopCenterCube(in);  }
             case TOPRIGHTCUBE:     { return fragmentTopRightCube(in);   }
             case BOTTOMLEFTCUBE:       { return fragmentBottomLeftCube(in); }
             case BOTTOMCENTERCUBE: { return fragmentBottomCenterCube(in); }
             case BOTTOMRIGHTCUBE:  { return fragmentBottomRightCube(in); }
             default:               { return vec4f(1.0, 0.0, 1.0, 1.0);  }
        }

}