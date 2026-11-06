//===================================
//Sphere Shader
//===================================
fn fragmentOnOff (in: VertexOutput) -> vec4f {
    let baseColor = vec3f(0.5, 0.07, 1.0);
    return vec4f(baseColor, 1.0);
}

fn vertexOnOff (position: ptr<function, vec3f>) -> vec4f {
    position.x *= 0.25;
    position.y *= 0.25;
    position.x -= 0.75;
    position.y -= 0.75;

////    position.x += u.buttonX * 0.1;
    return vec4f(*position, 1.0);

}