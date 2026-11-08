//===================================
//Plane Shader
//===================================
fn vertexTopLeftCube (position: ptr<function, vec3f>) -> vec4f {

    let wave = sin(u.time);
    position.x -= 0.35;
    position.y += 0.25;
    return vec4f(*position, 1.0);
}

fn fragmentTopLeftCube (in: VertexOutput) -> vec4f {

    if (u.tick == 0) { return animatedGreen; }
    else             { return animatedBlack; }
}