//
//  Object.h
//  JimboTrace
//
//  Created by James Bartolozzi on 9/14/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#ifndef JimboTrace_Object_h
#define JimboTrace_Object_h
#include "glm/gtx/transform.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include <vector>
#include "IntersectionPoint.h"

using namespace glm;

enum Type
{
    SPHERE,PLANE,MESH
};

// Face class used by Plane and Mesh Classes
class face {
    struct vertex
    {
        vec3 p,n;
        vertex(vec3 _p, vec3 _n) : p(_p),n(_n){}
    };
    std::vector<vertex> vertices;
    vec3 p1,p2,p3;
    face(vec3 _p1, vec3 _p2, vec3 _p3) : p1(_p1), p2(_p2), p3(_p3){}
};

// Base object class
class Object
{
private:
    virtual void CalculateBounds();
protected:
    // Bounding box min x,y,z values
    vec3 bbMin;
    // Bounding box max x,y,z values
    vec3 bbMax;
    Type ObjectType;
public:
    bool isLight;
    vec3 emittance;
    Shader *objShader;
    // Given starting point [pi] and direction vector [vi]
    // If intersected, returns true and
    // [pf] = intersection point, [n] = normal of intersection point, [t] = distance along ray
    virtual IntersectionPoint intersectShape(vec3 pi, vec3 vi);
    virtual IntersectionPoint intersectBoundingBox(vec3 pi, vec3 vi);
    virtual void transform(vec3 translate, vec3 rotate, vec3 scale);
    
};

// Implicit Sphere Class
class Sphere : public Object
{
private:
    void CalculateBounds();
protected:
    float radius;
    vec3 position;
public:
    Sphere();
    Sphere(float _radius, vec3 _position, Shader *s);
    IntersectionPoint intersectShape(vec3 pi, vec3 vi);
    IntersectionPoint intersectBoundingBox(vec3 pi, vec3 vi);
    void transform(vec3 translate, vec3 rotate, float scale);
};

// Plane Mesh class
class Plane : public Object
{
private:
    void findBounds();
protected:
    vec3 n;
    std::vector<vec3> points;
public:
    Plane();
    Plane(float w, float l, Shader *s);
    IntersectionPoint intersectShape(vec3 pi, vec3 vi);
    //IntersectionPoint intersectBoundingBox(vec3 pi, vec3 vi);
    void transform(vec3 translate, vec3 rotate, vec3 scale);
};
/*
// Obj file read class
class Mesh : public Object
{
private:
    void findBounds();
protected:
    std::vector<face> faces;
public:
    bool intersectShape(vec3 pi, vec3 vi, vec3& pf, vec3 &n, float& t);
    bool intersectBoundingBox(vec3 pi, vec3 vi, float& t);
    void transform(vec3 translate, vec3 rotate, vec3 scale);
};*/


#endif
