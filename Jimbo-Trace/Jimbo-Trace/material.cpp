#include "material.h"

material::material(vec3 _diffuse, vec3 _ambient, float _shininess, float _refCoeff) {
	diffuse = _diffuse;
	ambient = _ambient;
	shininess = _shininess;
	reflectivityCoefficient = _refCoeff;
}

material::material() {
	diffuse = vec3(0.4,0.4,0.4);
	ambient = vec3(0.1,0.1,0.1);
	shininess = 10.f;
	reflectivityCoefficient = 0.5;
}

// point, normal, eye, light pos
vec3 material::getColor(vec3 point, vec3 normal, vec3 p0, vec3 light, vec3 lightColor) {

	vec3 lightDirection = normalize(light-point);
	float distance = length(lightDirection);
	lightDirection = lightDirection / distance;
	distance = distance * distance;
	float intensity;

	// calculate diffuse
	vec3 diffuseM;
	float NdotL = dot(normal,lightDirection);
	intensity = clamp(NdotL,0.f,1.f);

	float diffuseTerm = clamp((dot(normal,lightDirection)),0.f,1.f);
	diffuseM = intensity * diffuse / distance;
	
	// calculate specular
	vec3 specularM;
	vec3 H = normalize(lightDirection + normalize(p0 - point));
	float NdotH = dot( normal, H);
	intensity = pow( clamp(NdotH,0.f,1.f), shininess);
	if (shininess == 0) {
		specularM = vec3(0,0,0);
	}
	else {
		specularM = intensity * lightColor / distance;
	}

	// add all together
	vec3 output = diffuseM + ambient + specularM;
	output.x = clamp(output.x, 0.f, 1.f);
	output.y = clamp(output.y, 0.f, 1.f);
	output.z = clamp(output.z, 0.f, 1.f);
	return output;
}

lambert::lambert() {
	diffuse = vec3(0.2,0.2,0.2);
	ambient = vec3(0,0,0);
	shininess = 10000000000;
	reflectivityCoefficient = 0;
}

lambert::lambert(vec3 _color) {
	diffuse = _color;
	ambient = vec3(0,0,0);
	shininess = 10000000000;
	reflectivityCoefficient = 0;
}

chrome::chrome() {
	diffuse = vec3(0,0,0);
	ambient = vec3(0,0,0);
	shininess = 2;
	reflectivityCoefficient = 0.6;
}