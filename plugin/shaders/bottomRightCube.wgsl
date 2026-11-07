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

    //Our off color will be red and our clicked color will be green
    let wave = sin(u.time * 5.0 * u.pressed);
    let baseColor = vec3f(0.05, (0.3 * wave) + 0.3, 0.04);

   return vec4f(baseColor, 1.0);
}