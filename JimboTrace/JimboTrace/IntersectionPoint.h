//
//  IntersectionPoint.h
//  JimboTrace
//
//  Created by James Bartolozzi on 9/17/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#ifndef JimboTrace_IntersectionPoint_h
#define JimboTrace_IntersectionPoint_h
#include "glm.hpp"

using namespace glm;

class IntersectionPoint
{
public:
    float tValue;
    bool didIntersect;
    int objid;
    vec3 p;
    vec3 n;
    IntersectionPoint();
    IntersectionPoint(bool dI, vec3 p, vec3 n, float t, int objid);
};


#endif
