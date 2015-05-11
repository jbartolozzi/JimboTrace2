//
//  Camera.h
//  JimboTrace
//
//  Created by James Bartolozzi on 9/14/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#ifndef JimboTrace_Camera_h
#define JimboTrace_Camera_h

#include "glm.hpp"
#include "mtrand.h"

using namespace glm;

class Camera {
private:
	vec3 M;
	vec3 H;
	vec3 V;
protected:
public:
    // Up Vector (normalized vec3)
	vec3 up;
    // Eye position
	vec3 eye;
    //Viewing direction
	vec3 view;
    // Right vector
    vec3 right;
    // Horizontal Field of View
	float fovX;
    // Vertical Field of View
	float fovY;
    // Image width in pixels (integer)
	int imgWidth;
    // Image height in pixels (integer)
	int imgHeight;
    // Get ray direction from pixel coordinate
	vec3 getDirectionFromCoordinate(int sx, int sy, int sampX, int sampY, float seed);
    // Camera constructor
	Camera(vec3 _eye, vec3 _view, vec3 _up, float _fovy, int _imgWidth, int _imgHeight);
    // Default constructor
    Camera();
};

#endif
