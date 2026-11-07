//
// Created by Erik Jourgensen on 11/6/26.
//

#ifndef JUCEWEBGPUSETUP_PLANEGEOMETRY_H
#define JUCEWEBGPUSETUP_PLANEGEOMETRY_H
#include <cstdint>
#include <vector>

struct PlaneVertex {

    //Position
    float  x,  y,  z;
    //Normal
    float nX, nY, nZ;
    //Color
    float  r,  g,  b;

};

using planeIndex = uint16_t;

class PlaneGeometry {
public:
    static void buildPlane(std::vector<PlaneVertex>& vertices,
                           std::vector<planeIndex>& indices,
                           int width,
                           int height,
                           float shrinkY,
                           int widthSegments,
                           int heightSegments);

};



#endif //JUCEWEBGPUSETUP_PLANEGEOMETRY_H
