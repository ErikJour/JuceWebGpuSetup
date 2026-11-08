//================
//On Off Button
//================
fn vertexOnOff (position: ptr<function, vec3f>) -> vec4f {

    //Place the on / off button in the bottom left of the window
    let positionShift   = 0.75;
    position.x -= positionShift;
    position.y -= positionShift;

    return vec4f(*position, 1.0);
}

fn fragmentOnOff (in: VertexOutput) -> vec4f {


    //Switch based on pressed value. Erik note -> this a bool.
    if (u.pressed > 0) { return animatedGreen; }
    else               { return animatedRed;   }
}