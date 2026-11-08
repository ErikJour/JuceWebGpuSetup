//===================================
//Plane Shader
//===================================
fn vertexBottomCenterCube (position: ptr<function, vec3f>) -> vec4f {

    let wave = sin(u.time);
    position.y -= 0.1;
    return vec4f(*position, 1.0);
}

fn fragmentBottomCenterCube (in: VertexOutput) -> vec4f {

    if (u.tick == 4) { return animatedGreen; }
    else             { return animatedBlack; }

}