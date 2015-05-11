//
//  Camera.cpp
//  JimboTrace
//
//  Created by James Bartolozzi on 9/14/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#include "Camera.h"

Camera::Camera()
{
    eye = vec3(0,0,8);
    view = vec3(0,0,-1);
    up = vec3(0,1,0);
    right = cross(view,up);
    
    fovX = 45.0f;
    
    imgWidth = 1280/2;
    imgHeight = 720/2;
    //tan((M_PI * 0.5 * fovy) / 180.0);
    fovY = (float)imgHeight/(float)imgWidth * fovX;
    fovY *= M_PI/180.f;
    fovX *= M_PI/180.f;
    fovX = atan(((float)imgWidth/(float)imgHeight)*tan(fovY));
}

Camera::Camera(vec3 _eye, vec3 _view, vec3 _up, float _fovy, int _imgWidth, int _imgHeight) :
eye(_eye), view(_view), up(_up), fovY(_fovy), imgWidth(_imgWidth),imgHeight(_imgHeight)
{
	fovX = atan(((float)imgWidth/(float)imgHeight)*tan(fovY));
}

vec3 Camera::getDirectionFromCoordinate(int sx, int sy, int sampX, int sampY, float seed)
{
    /*
    float sx2 = ((float)sx /(float)imgWidth) - 0.5f;
    float sy2 = ((float)sy /(float)imgHeight) - 0.5f;
    sx2 *= (float)imgWidth/(float)imgHeight ;
    vec3 image_point = sx2 * right + up * sy2 + eye + view;
    vec3 ray_direction = image_point - eye;
    return  ray_direction;*/
    
    MTRand_closed r1,r2;
    int rand1 = (int)(r1()*10.f);
    int rand2 = (int)(r2()*10.f);
    float x0 = rand1 % sampX;
    float y0 = rand2 % sampY;
    float X = sx + (x0 - 1)/(float)sampX;
    float Y = sy + (y0 - 1)/(float)sampY;
    
    // A = C x U
    glm::vec3 A = glm::cross(view,up);
    // B = A x C
    glm::vec3 B = glm::cross(A,view);
    // M = E + C
    M = eye + view;
    // H = (A|C|tan(theta))/|A|
    H = (A * glm::length(view) * glm::tan(fovX))/(glm::length(A));
    // V = (B|C|tan(phi))/|B|
    V = (B * glm::length(view) * glm::tan(fovY))/(glm::length(B));
	glm::vec3 P = M + (2.f*((float)X/(imgWidth-1)) - 1)*H + (2.f*((float)Y/(imgHeight-1)) - 1)*V;
	return normalize(P-eye);
}