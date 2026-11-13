//================
//On Off Button
//================
fn vertexOnOff (position: ptr<function, vec3f>) {

    //Puts us in 3D mode
//    return (projectPerspective(position.xyz));
}

fn fragmentOnOff (in: VertexOutput) -> vec4f {
    let normal      = normalize(in.normal);
    let baseColor   = vec3f(0.2, 0.95, 0.5);

    return vec4f(baseColor, 1.0);

}