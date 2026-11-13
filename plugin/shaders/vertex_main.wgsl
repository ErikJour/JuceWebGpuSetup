
fn projectPerspective(worldPos: vec3f) -> vec4f { return u.viewProjMatrix * vec4f(worldPos, 1.0); }

//==============
//Vertex Shader
//==============
@vertex
fn vs_main(in: VertexInput) -> VertexOutput {

    var out      : VertexOutput;
    var worldPos = (u.modelMatrix * vec4f(in.position, 1.0)).xyz;

    //Material-dependant vertex functions.
    switch u.materialId {
            case MAT_ONOFFBUTTON:  { vertexOnOff(&worldPos);           }
            case TOPLEFTCUBE:      { vertexTopLeftCube(&worldPos);     }
            case TOPCENTERCUBE:    { vertexTopCenterCube(&worldPos);    }
            case TOPRIGHTCUBE:     { vertexTopRightCube(&worldPos);    }
            case BOTTOMLEFTCUBE:   { vertexBottomLeftCube(&worldPos);  }
            case BOTTOMCENTERCUBE: { vertexBottomCenterCube(&worldPos);}
            case BOTTOMRIGHTCUBE:  { vertexBottomRightCube(&worldPos); }
            default:               { }
        }

    //This gets into camera and then into clip space
    out.clipPos   = projectPerspective(worldPos);
    out.worldPos  = worldPos;
    out.objectPos = in.position;
    out.color     = in.color;
    out.normal    = in.normal;

    return out;
}

