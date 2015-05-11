#pragma once
#ifndef STUBS_H
#define STUBS_H

#include <iostream>
#include <limits>
#include <vector>
#include "primative.h"

using namespace glm;

struct intersectionPoint {
	vec3 point;
	vec3 normal;
	double tValue;
	primative *intersectionPrimative;
};
// IMPORTANT NOTE:
// Although these functions are useful for testing, they're not good to use for the
// actual raycasting. In particular, using these functions means you have to invert
// an object's transformation matrix for every ray test you run against it.
// It's much better to cache an object's inverted transformation matrix in the node,
// and then use that to transform the ray, rather than invert it in your raycast.
// Therefore, make these functions wrappers around your actual intersection functions,
// and use those functions in your raytracer.
intersectionPoint Test_RayPlaneIntersectInverse(vec3 const& P0, vec3 const& V0, mat4 const& T, mat4 const& invT, primative* prim);
// Tries to find the intersection of a ray and a sphere.
// P0 is the position from which the ray emanates; V0 is the
// direction of the ray.
// matrix is the transformation matrix of the sphere.
// This function should return the smallest positive t-value of the intersection
// (a point such that P0+t*V0 intersects the sphere), or -1 if there is no
// intersection.
//intersectionPoint Test_RaySphereIntersect(vec3 const& P0, vec3 const& V0, mat4 const& T);
intersectionPoint Test_RaySphereIntersectInverse(vec3 const& P0, vec3 const& V0, mat4 const& T, mat4 const& invT, primative *prim, bool isTest);
// Tries to find the intersection of a ray and a triangle.
// This is just like the above function, but it intersects the ray with a
// triangle instead. The parameters p1, p2, and p3 specify the three
// points of the triangle, in object space.
//intersectionPoint Test_RayPolyIntersect(vec3 const& P0, vec3 const& V0, vec3 const& p1, vec3 const& p2, vec3 const& p3, mat4 const& T);
//intersectionPoint Test_RayPolyIntersectInverse(vec3 const& P0, vec3 const& V0, mat4 const& T, mat4 const& invT, node* _node, vector<primative::face> faceVector);

// This is just like Test_RaySphereIntersect, but with a unit cube instead of a
// sphere. A unit cube extends from -0.5 to 0.5 in all axes.
//intersectionPoint Test_RayCubeIntersect(vec3 const& P0, vec3 const& V0, mat4 const& T);
//intersectionPoint Test_RayCubeIntersectInverse(vec3 const& P0, vec3 const& V0, mat4 const& T, mat4 const& invT, node* _node);

// This is also like Test_RaySphereIntersect, but with a unit cylinder.  A unit
// cylinder is 1 unit tall, 1 unit in diameter, with the bottom base at (0,-0.5,0)
//intersectionPoint Test_RayCylinderIntersect(vec3 const& P0, vec3 const& V0, mat4 const& T);
//intersectionPoint Test_RayCylinderIntersectInverse(vec3 const& P0, vec3 const& V0, mat4 const& T, mat4 const& invT, float width, float height, node* _node);

#endif