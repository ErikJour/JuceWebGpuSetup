//===============
//On Off Button
//===============
fn fragmentOnOff (in: VertexOutput) -> vec4f {

    let baseColor = vec3f(0.5, 0.07, 0.04);
    let onColor   = vec3f(0.1, 0.45, 0.09);

    if (u.pressed > 0) {return vec4f(onColor, 1.0);}
    else { return vec4f(baseColor, 1.0); }
}

fn vertexOnOff (position: ptr<function, vec3f>) -> vec4f {

    let shift   = 0.75;

    position.x -= shift;
    position.y -= shift;

    return vec4f(*position, 0.5);
}