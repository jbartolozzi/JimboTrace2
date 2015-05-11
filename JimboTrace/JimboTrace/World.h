//
//  World.h
//  JimboTrace
//
//  Created by James Bartolozzi on 9/14/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#ifndef JimboTrace_World_h
#define JimboTrace_World_h
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include "EasyBMP.h"
#include <vector>
#include "perlin.h"


using namespace glm;

class World
{
private:
    vec3 getBackgroundColor(int sx, int sy);
    IntersectionPoint IntersectWorld(vec3 pi, vec3 d, int objid, bool checkLights);
    // Creates a light and a sphere with the color, position, and radius
    void createLight(vec3 color, vec3 position, float radius);
    vec3 getSurfaceColor(vec3 p, vec3 n, int objid, int seed);
    vec3 getDirectLightingColor(vec3 p, vec3 n, int lightNum, int seed, int objid);
    Perlin *perlin;
    
    int totalPasses;
    int sampleX;
    int sampleY;
    float **rCol;
    float **gCol;
    float **bCol;
    int **Hits;
    float *OUTCOLORS;
    
protected:
public:
    int passCounter;
    //BMP output image
    EasyBMP::BMP outputImage;
    //World Camera
    Camera *MainCamera;
    //Vector of all scene lights
    std::vector<Light*> Lights;
    std::vector<Object*> Objects;
    //Default constructor
    World();
    float* Render();
    vec3 getBRDF(vec3 p, vec3 n, float sa);
    
};


#endif
