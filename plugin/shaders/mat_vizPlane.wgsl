//===================================
//Plane Shader
//===================================
fn vertexPlane (position: ptr<function, vec3f>) -> vec4f {
//    let wave = sin(u.time);
    return vec4f(*position, 1.0);
}

fn fragmentPlane (in: VertexOutput) -> vec4f {
     //Our off color will be red and our clicked color will be green
        let baseColor = vec3f(0.5, 0.07, 0.04);
        let onColor   = vec3f(0.1, 0.45, 0.09);

        //Switch based on pressed value. Erik note -> this a bool.
        if (u.pressed > 0) { return vec4f(onColor, 1.0);   }
        else               { return vec4f(baseColor, 1.0); }
}