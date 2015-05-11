//
//  Shader.cpp
//  JimboTrace
//
//  Created by James Bartolozzi on 9/14/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#include "Shader.h"

Shader::Shader()
{
}

vec3 Shader::getColorValue(vec3 p, vec3 n, vec3 eye, vec3 lPos, vec3 lCol)
{
    return vec3(0,0,0);
}

NormalShader::NormalShader(bool _positiveOnly) : positiveOnly(_positiveOnly) {}

vec3 NormalShader::getColorValue(vec3 p, vec3 n, vec3 eye, vec3 lPos, vec3 lCol)
{
    if (positiveOnly)
        return vec3(abs(n.x),abs(n.y),abs(n.z));
    else {
        vec3 nout = vec3(clamp(n.x,0.f,1.f),clamp(n.y,0.f,1.f),clamp(n.z,0.f,1.f));
        return nout;
    }
}

BlinnPhong::BlinnPhong()
{
    //Perlin(int octaves, float freq, float amp, int seed)
    
    diffuse = vec3(1,1,1);
	ambient = vec3(0,0,0);
    specular = vec3(1,1,1);
	specularHardness = 100.f;
	reflectivityCoefficient = 1.0;
}

BlinnPhong::BlinnPhong(vec3 _diffuse):diffuse(_diffuse)
{
    //Perlin(int octaves, float freq, float amp, int seed)
    ambient = vec3(0,0,0);
    specular = vec3(1,1,1);
    specularHardness = 100.f;
    reflectivityCoefficient = 1.0;
}

vec3 BlinnPhong::getColorValue(vec3 p, vec3 n,vec3 eye, vec3 lPos, vec3 lCol)
{
    //n = normalize(n*per);
    //return vec3(abs(n[0]),abs(n[1]),abs(n[2]));
    vec3 output;
    vec3 lightDirection = normalize(lPos-p);
	float distance = length(lPos-p);
	distance = pow(distance,0.1);
	float intensity;
    
	// calculate diffuse
	vec3 diffuseM;
	float NdotL = dot(n,lightDirection);
	intensity = clamp(NdotL,0.f,1.f);
    
	//float diffuseTerm = clamp((dot(n,lightDirection)),0.f,1.f);
	diffuseM = (intensity * diffuse * lCol)/ distance;
    
	// calculate specular
	vec3 specularM;
	vec3 H = normalize(lightDirection + normalize(p-eye));
	float NdotH = dot( n, H);
	intensity = pow( clamp(NdotH,0.f,1.f), specularHardness);
    specularM = intensity * lCol * specular / distance;
    
	// add all together
    output = (diffuseM + specularM);
	output.x = clamp(output.x, 0.f, 1.f);
	output.y = clamp(output.y, 0.f, 1.f);
	output.z = clamp(output.z, 0.f, 1.f);
	return output;
}

LightShader::LightShader()
{
    color = vec3(1,1,1);
}

LightShader::LightShader(vec3 _color) : color(_color) {}

vec3 LightShader::getColorValue(vec3 p, vec3 n, vec3 eye, vec3 lPos, vec3 lCol)
{
    return color;
}

CheckerBoard::CheckerBoard()
{
    c1 = BlinnPhong();
    c2 = NormalShader(true);
}

CheckerBoard::CheckerBoard(Shader c1, Shader c2, int scale)
{
    
}

vec3 CheckerBoard::getColorValue(vec3 p, vec3 n, vec3 eye, vec3 lPos, vec3 lCol)
{
    float val = p.x + p.z;
    if ((int)val % 2)
    {
        return vec3(0,0,0);
        //return c1.getColorValue( p,  n,  eye,  lPos,  lCol);
    }
    else {
        return vec3(1,1,1);
        //return c2.getColorValue( p,  n,  eye,  lPos,  lCol);
    }
}
