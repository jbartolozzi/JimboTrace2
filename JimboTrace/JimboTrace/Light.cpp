//
//  Light.cpp
//  JimboTrace
//
//  Created by James Bartolozzi on 10/1/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#include "Light.h"

Light::Light(vec3 _color, vec3 _position, float _radius)
{
    color = _color;
    position = _position;
    radius = _radius;
}