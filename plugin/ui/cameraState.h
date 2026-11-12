//
// Created by Erik Jourgensen on 11/12/26.
//

#ifndef ANIMATEDNOISE_CAMERASTATE_H
#define ANIMATEDNOISE_CAMERASTATE_H

struct vec2
{
    float x = 0.0f, y = 0.0f;

    vec2 operator+(vec2 o)  const       { return { x + o.x, y + o.y }; }
    vec2 operator-(vec2 o)  const       { return { x - o.x, y - o.y }; }
    vec2 operator*(float s) const       { return { x * s, y * s }; }
};

struct CameraState {
    float angleX = 0.0f;
    float posX   = 0.75f;
    float posZ   = 2.0f;

    static constexpr float eyeY        = 0.02f;  // fixed eye height
    static constexpr float kWallRadius = 1.f;  // clamp radius (floor edge is 0.95)
};

struct DragState {
    bool  active            = false;
    float startMouseX       = 0.0f;
    float startAngleX       = 0.0f;
    float sensitivity       = 0.005f;
    float scrollSensitivity = 0.075f;
    float turnSensitivity   = 5.0f;
};

#endif //ANIMATEDNOISE_CAMERASTATE_H
