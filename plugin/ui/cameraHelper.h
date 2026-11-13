//
// Created by Erik Jourgensen on 11/12/26.
//

//
// Created by Erik Jourgensen on 6/26/26.
//
#pragma once
#include <cstring>
#include <cmath>

inline void buildLookAt(float out[16],
                        const float ex, float ey, float ez,
                        float tx, float ty, float tz)
{
    float fx = tx-ex, fy = ty-ey, fz = tz-ez;
    const float fl = sqrtf(fx*fx + fy*fy + fz*fz);
    fx/=fl; fy/=fl; fz/=fl;

    // right = cross(worldUp=(0,1,0), forward)
    float rx = -fz, ry = 0.0f, rz = fx;
    const float rl = sqrtf(rx*rx + ry*ry + rz*rz);
    rx/=rl; ry/=rl; rz/=rl;

    // corrected up = cross(forward, right)
    const float ux = ry*fz - rz*fy;
    const float uy = rz*fx - rx*fz;
    const float uz  = rx*fy - ry*fx;

    // column-major: out[col*4+row]
    out[0]=rx;   out[4]=ux;   out[8] =-fx;  out[12]=-(rx*ex+ry*ey+rz*ez);
    out[1]=ry;   out[5]=uy;   out[9] =-fy;  out[13]=-(ux*ex+uy*ey+uz*ez);
    out[2]=rz;   out[6]=uz;   out[10]=-fz;  out[14]=  fx*ex+fy*ey+fz*ez;
    out[3]=0.0f; out[7]=0.0f; out[11]=0.0f; out[15]=1.0f;
}
// Right-handed perspective, Z maps to [0, 1] (WebGPU NDC)
inline void buildPerspective(float out[16], float fovY, float aspect, float zNear, float zFar)
{
    const float f = 1.0f / tanf(fovY * 0.5f);
    std::memset(out, 0, 64);
    out[0]  = f / aspect;
    out[5]  = f;
    out[10] = zFar / (zNear - zFar);
    out[11] = -1.0f;
    out[14] = (zNear * zFar) / (zNear - zFar);
}

inline void mulMat4(float out[16], const float a[16], const float b[16])
{
    for (int col = 0; col < 4; ++col)
        for (int row = 0; row < 4; ++row) {
            float v = 0.0f;
            for (int k = 0; k < 4; ++k)
                v += a[k*4+row] * b[col*4+k];
            out[col*4+row] = v;
        }
}

struct CrossPlatformHelpers {
    static void buildInvLookAt(float* out,float ex, float ey, float ez, float tx, float ty, float tz,
                               float upx = 0.0f, float upy = 1.0f, float upz = 0.0f)
    {
        float fx = tx - ex, fy = ty - ey, fz = tz - ez;
        const float fl = 1.0f / sqrtf(fx*fx + fy*fy + fz*fz);
        fx *= fl; fy *= fl; fz *= fl;

        float rx = fy*upz - fz*upy;
        float ry = fz*upx - fx*upz;
        float rz = fx*upy - fy*upx;
        const float rl = 1.0f / sqrtf(rx*rx + ry*ry + rz*rz);
        rx *= rl; ry *= rl; rz *= rl;

        const float ux = ry*fz - rz*fy;
        const float uy = rz*fx - rx*fz;
        const float uz = rx*fy - ry*fx;

        out[0]  = rx;   out[1]  = ry;   out[2]  = rz;   out[3]  = 0.0f;
        out[4]  = ux;   out[5]  = uy;   out[6]  = uz;   out[7]  = 0.0f;
        out[8]  = -fx;  out[9]  = -fy;  out[10] = -fz;  out[11] = 0.0f;
        out[12] = ex;   out[13] = ey;   out[14] = ez;   out[15] = 1.0f;
    }

    static void buildInvPerspective(float* out,
                                    float fovY, float aspect,
                                    float nearZ, float farZ)
    {
        const float t  = tanf(fovY * 0.5f);
        const float A  = farZ / (nearZ - farZ);
        const float B  = (farZ * nearZ) / (nearZ - farZ);

        for (int i = 0; i < 16; ++i) out[i] = 0.0f;

        out[0]  = aspect * t;
        out[5]  = t;
        out[11] = 1.0f / B;
        out[14] = -1.0f;
        out[15] = A / B;
    }

    static void makeModelMatrix(float* m, float angle, float tx, float ty, float tz)
    {
        const float c = std::cos(angle);
        const float s = std::sin(angle);
        m[0] =  c;  m[1] = 0;  m[2]  = -s; m[3]  = 0;
        m[4] =  0;  m[5] = 1;  m[6]  =  0; m[7]  = 0;
        m[8] =  s;  m[9] = 0;  m[10] =  c; m[11] = 0;
        m[12] = tx; m[13] = ty; m[14] = tz; m[15] = 1;
    }

};


