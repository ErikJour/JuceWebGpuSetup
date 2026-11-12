
fn projectPerspective(worldPos: vec3f) -> vec4f { return u.viewProjMatrix * vec4f(worldPos, 1.0); }

//==============
//Vertex Shader
//==============
@vertex
fn vs_main(in: VertexInput) -> VertexOutput {

    var out      : VertexOutput;
    var position = in.position;

    //Material-dependant vertex functions.
    switch u.materialId {
            case MAT_ONOFFBUTTON:  { out.clipPos = vertexOnOff        (&position);    }
            case TOPLEFTCUBE:      { out.clipPos = vertexTopLeftCube  (&position);    }
            case TOPCENTERCUBE:    { out.clipPos = vertexTopCenterCube (&position);   }
            case TOPRIGHTCUBE:     { out.clipPos = vertexTopRightCube (&position);    }
            case BOTTOMLEFTCUBE:   { out.clipPos = vertexBottomLeftCube (&position);  }
            case BOTTOMCENTERCUBE: { out.clipPos = vertexBottomCenterCube (&position);}
            case BOTTOMRIGHTCUBE:  { out.clipPos = vertexBottomRightCube (&position); }
            default:               { out.clipPos = projectPerspective (position);     }
        }

    out.color    = in.color;
    out.worldPos = position;
    out.normal   = in.normal;

    return out;
}

