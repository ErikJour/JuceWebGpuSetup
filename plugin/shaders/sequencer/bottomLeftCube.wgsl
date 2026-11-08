//===================================
//Plane Shader
//===================================
fn vertexBottomLeftCube (position: ptr<function, vec3f>) -> vec4f {

    let wave = sin(u.time);
    position.y -= 0.1;
    position.x -= 0.35;
    return vec4f(*position, 1.0);
}

fn fragmentBottomLeftCube (in: VertexOutput) -> vec4f {

    if (u.tick == 3) { return animatedGreen; }
    else             { return animatedBlack; }

}