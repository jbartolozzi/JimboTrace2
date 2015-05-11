//
//  Shader.h
//  JimboTrace
//
//  Created by James Bartolozzi on 9/14/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#ifndef JimboTrace_Shader_h
#define JimboTrace_Shader_h
#include "glm.hpp"
#include "perlin.h"

using namespace glm;

class Shader
{
private:
public:
    virtual vec3 getColorValue(vec3 p, vec3 n,vec3 eye, vec3 lPos, vec3 lCol);
    Shader();
};

class NormalShader : public Shader
{
public:
    bool positiveOnly;
    NormalShader(bool positiveOnly);
    vec3 getColorValue(vec3 p, vec3 n,vec3 eye, vec3 lPos, vec3 lCol);
};

class BlinnPhong : public Shader
{
public:
    vec3 diffuse,ambient,specular;
	float specularHardness,reflectivityCoefficient;
    vec3 getColorValue(vec3 p, vec3 n,vec3 eye, vec3 lPos, vec3 lCol);
    BlinnPhong();
    BlinnPhong(vec3 _diffuse);
};

class LightShader : public Shader
{
public:
    vec3 color;
    vec3 getColorValue(vec3 p, vec3 n,vec3 eye, vec3 lPos, vec3 lCol);
    LightShader();
    LightShader(vec3 _color);
};

class CheckerBoard : public Shader
{
public:
    Shader c1, c2;
    vec3 getColorValue(vec3 p, vec3 n,vec3 eye, vec3 lPos, vec3 lCol);
    CheckerBoard();
    CheckerBoard(Shader c1, Shader c2, int scale);
};



#endif
