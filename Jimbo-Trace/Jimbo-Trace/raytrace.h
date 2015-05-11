#ifndef RAYTRACE
#define RAYTRACE

#include "camera.h"
#include "EasyBMP.h"
#include "stubs.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include "material.h"
#include "primative.h"

using namespace glm;

class raytrace {
private:
	vec3 EYEP;
	vec3 VDIR;
	vec3 UVEC;
	vec3 ACOL;
	vec3 LPOS;
	vec3 LCOL;
	char* FILENAME;
	char* FILENAME_ZD;
	float FOVY;
	int RESOX;
	int RESOY;
	int numShadow;
	float nudge;
	float pi;
	int NUMBOUNCES;
	float lightRadius;
	float shadowRatioMultiplier;
	mat4 LightTransform;
	mat4 LightInvTransform;
	primative *lightPrimative;
	int frameNumber;
	primative *sphere;
	primative *sphere2;
	primative *sphere3;
	primative *sphere4;
	int SAMPLEX;
	int SAMPLEY;
	vec3 getPixelSample(int i, int j,camera* mainCamera);

	std::vector<float> depths;

	void constructDepthBMP();

public:
	// Attribute values
	camera *mainCamera;
	void setFrame(int f);
	std::vector<material*> materials;
	std::vector<primative*> primatives;

	raytrace();
	BMP output;
	BMP zDepth;
	void runRaytrace(int thread);
	intersectionPoint recursiveTrace(vec3 eye, vec3 ray, bool shadowCheck);
	vec3 reflectionTrace(vec3 p0, vec3 v0, int iteration);
	
	void finishRaytrace();

};

#endif