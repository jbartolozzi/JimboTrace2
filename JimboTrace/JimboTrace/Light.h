//
//  Light.h
//  JimboTrace
//
//  Created by James Bartolozzi on 10/1/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#ifndef __JimboTrace__Light__
#define __JimboTrace__Light__
#include <stdio.h>
#include "glm.hpp"

using namespace glm;

class Light
{
public:
    vec3 color;
    vec3 position;
    float radius;
    Light(vec3 _color, vec3 _position, float _radius);
};

#endif /* defined(__JimboTrace__Light__) */
