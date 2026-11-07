
fn projectPerspective(worldPos: vec3f) -> vec4f { return vec4f(worldPos, 1.0); }

//==============
//Vertex Shader
//==============
@vertex
fn vs_main(in: VertexInput) -> VertexOutput {

    var out      : VertexOutput;
    var position = in.position;

    //Material-dependant vertex functions.
    switch u.materialId {
            case MAT_ONOFFBUTTON:  { out.clipPos = vertexOnOff        (&position); }
            case MAT_PLANE:        { out.clipPos = vertexPlane        (&position); }
            default:               { out.clipPos = projectPerspective (position);  }
        }

    out.clipPos  = projectPerspective (position);
    out.color    = in.color;
    out.worldPos = position;
    out.normal   = in.normal;

    return out;
}

