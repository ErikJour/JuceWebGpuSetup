//===================================
//Plane Shader
//===================================
fn vertexTopRightCube (position: ptr<function, vec3f>) -> vec4f {

    let wave = sin(u.time);
    position.y += 0.25;
    position.x += 0.35;
    return vec4f(*position, 1.0);
}

fn fragmentTopRightCube (in: VertexOutput) -> vec4f {

     if (u.tick == 2) { return vec4f(0.35, 0.6, 0.2, 1.0); }
     else { return vec4f(0.0, 0.0, 0.0, 1.0); }
}