//================
//On Off Button
//================
fn vertexOnOff (position: ptr<function, vec3f>) -> vec4f {

    //Added model matrix in here
    let worldPosition = u.modelMatrix * vec4f(*position, 1.0);
    *position              = worldPosition.xyz;
    return (projectPerspective(worldPosition.xyz));
}

fn fragmentOnOff (in: VertexOutput) -> vec4f {
    let normal      = normalize(in.normal);
    let baseColor   = vec3f(0.7, 0.65, 0.5);

    return vec4f(baseColor, 1.0);

}