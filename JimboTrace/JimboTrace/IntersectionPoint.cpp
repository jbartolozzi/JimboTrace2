//
//  IntersectionPoint.cpp
//  JimboTrace
//
//  Created by James Bartolozzi on 9/17/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#include "IntersectionPoint.h"

IntersectionPoint::IntersectionPoint()
{
    tValue = 100000;
    didIntersect = false;
    p = vec3(0);
    n = vec3(0);
}

IntersectionPoint::IntersectionPoint(bool dI, vec3 _p, vec3 _n, float t, int _objid) : didIntersect(dI), p(_p), n(_n), tValue(t), objid(_objid)
{
}
