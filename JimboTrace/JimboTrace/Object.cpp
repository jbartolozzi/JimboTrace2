//
//  Object.cpp
//  JimboTrace
//
//  Created by James Bartolozzi on 9/14/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#include "Object.h"

/*#############################################################
 #                                                            #
 #                  BASE OBJECT CLASS                         #
 #                                                            #
 ############################################################## */

void Object::CalculateBounds()
{
    
}

IntersectionPoint Object::intersectShape(vec3 pi, vec3 vi)
{
    IntersectionPoint output;
    return output;
}

IntersectionPoint Object::intersectBoundingBox(vec3 pi, vec3 vi)
{
    IntersectionPoint output;
    return output;
}

void Object::transform(vec3 translate, vec3 rotate, vec3 scale)
{
}

/*#############################################################
 #                                                            #
 #                       SPHERE CLASS                         #
 #                                                            #
 ############################################################## */

Sphere::Sphere()
{
    emittance = vec3(0);
    objShader = new Shader();
    isLight = false;
    radius = 1.0f;
    position = vec3(0,0,0);
    CalculateBounds();
}

Sphere::Sphere(float _radius, vec3 _position, Shader *s): radius(_radius),position(_position)
{
    CalculateBounds();
    isLight = false;
    objShader = s;
}

void Sphere::CalculateBounds()
{
    bbMin = vec3(position[0] - radius,position[1] - radius,position[2] - radius);
    bbMax = vec3(position[0] + radius,position[1] + radius,position[2] + radius);
}

IntersectionPoint Sphere::intersectShape(vec3 pi, vec3 vi)
{
    float discriminant,t1,t2,t;
    bool hit = false;
    vec3 pf,n;
    // a =  dx^2 + dy^2 + dz^2
    float a = vi.x*vi.x+vi.y*vi.y+vi.z*vi.z;
    // p1 = pi, p3 = sphere position
    // b = 2(dx*(x1-x3) + dy*(y1-y3) + dz*(z1-z3))
    float b = 2.f*(vi.x*(pi.x-position.x) + vi.y*(pi.y-position.y) + vi.z*(pi.z-position.z));
    //
    float c = position[0]*position[0] + position[1]*position[1] + position[2]*position[2] + pi[0]*pi[0] + pi[1]*pi[1] + pi[2]*pi[2]
              - 2.f*(position[0]*pi[0] + position[1]*pi[1] + position[2]*pi[2]) - radius*radius;
    discriminant = b * b - 4.f * a * c;
    if (discriminant <= 0.001f && discriminant >= 0.0001)
    {
        t = -b/(2.f*a);
        if (t > 0)
            hit = true;
    }
    else if (discriminant > 0.01f)
    {
        t1 = (-b - sqrt(b*b - 4.f*a*c)) / 2.f*a;
        t2 = (-b + sqrt(b*b - 4.f*a*c)) / 2.f*a;
        t = min(t1,t2);
        if (t > 0)
            hit = true;
    }
    pf = pi + t*vi;
    n = normalize(pf-position);
    IntersectionPoint output = IntersectionPoint(hit,pf,n,t,-1);
    return output;
}

IntersectionPoint Sphere::intersectBoundingBox(vec3 pi, vec3 vi)
{
    IntersectionPoint output;
    return output;
}

void Sphere::transform(vec3 translate, vec3 rotate, float scale)
{
    position += translate;
    radius *= scale;
    CalculateBounds();
}

Plane::Plane()
{
    objShader = new Shader();
    n = vec3(0,1,0);
    points.push_back(vec3(-1,-2,1));
    points.push_back(vec3(1,-2,1));
    points.push_back(vec3(-1,-2,-1));
    points.push_back(vec3(1,2,-1));
    isLight = false;
}

Plane::Plane(float w, float l, Shader *s)
{
    n = vec3(0,1,0);
    points.push_back(vec3(-w,0,l));
    points.push_back(vec3(w,0,l));
    points.push_back(vec3(w,0,-l));
    points.push_back(vec3(-w,0,-l));
    isLight = false;
    objShader = s;
}

IntersectionPoint Plane::intersectShape(vec3 pi, vec3 vi)
{
    vec3 E1,E2,V0,V1,V2,O,T,Q,P;
    bool hit = false;
    float outT = 100000;
    for (int i = 0; i < 2; i++)
    {
        V0 = points[0];
        V1 = points[i+1];
        V2 = points[i+2];
        E1 = V1 - V0;
        E2 = V2 - V0;
        T = pi - V0;
        P = cross(vi, E2);
        Q = cross(T,E1);
        
        vec3 result = (1.f / (dot(P, E1))) * vec3(dot(Q, E2),dot(P,T),dot(Q,vi));
        if (result.y >= 0.f && result.z >= 0.f && result.y + result.z <= 1.f && result.x < outT && result.x > 0.f)
        {
            hit = true;
            outT = result.x;
        }
    }
    IntersectionPoint output;
    if (hit)
    {
        output = IntersectionPoint(true, pi+outT*vi, n, outT,0);
    }
    return output;
}

void Plane::transform(vec3 translate, vec3 rotate, vec3 scale)
{
    // translation * rotation * scale
    mat4 trans = glm::gtx::transform::translate(translate.x, translate.y, translate.z);
    mat4 rotx = glm::gtx::transform::rotate(rotate.x, 1.f, 0.f, 0.f);
    mat4 roty = glm::gtx::transform::rotate(rotate.y, 0.f, 1.f, 0.f);
    mat4 rotz = glm::gtx::transform::rotate(rotate.z, 0.f, 0.f, 1.f);
    mat4 rot = rotx*roty*rotz;
    mat4 sca = glm::gtx::transform::scale(scale.x, scale.y, scale.z);
    mat4 result = trans * rot * sca;
    for (int i = 0; i < points.size(); i++)
    {
        vec4 out = result * vec4(points[i],1.f);
        points[i] = vec3(out.x,out.y,out.z);
    }
    vec4 newN = result * vec4(n,0.f);
    n = vec3(newN);//normalize(vec3(newN.x,newN.y,newN.z));
}