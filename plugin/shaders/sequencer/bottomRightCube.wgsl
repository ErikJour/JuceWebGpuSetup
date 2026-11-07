//===================================
//Plane Shader
//===================================
fn vertexBottomRightCube (position: ptr<function, vec3f>) -> vec4f {

    let wave = sin(u.time);
    position.y -= 0.1;
    position.x += 0.35;
    return vec4f(*position, 1.0);
}

fn fragmentBottomRightCube (in: VertexOutput) -> vec4f {

    if (u.tick == 5) { return vec4f(0.35, 0.6, 0.2, 1.0); }
    else { return vec4f(0.0, 0.0, 0.0, 1.0); }
}