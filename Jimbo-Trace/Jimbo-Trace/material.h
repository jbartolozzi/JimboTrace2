#ifndef MATERIAL
#define MATERIAL

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/transform.hpp"
#include "camera.h"

using namespace glm;

class material {
private:
	
public:
	vec3 diffuse;
	vec3 ambient;
	float shininess; // specular component
	float reflectivityCoefficient;
	material(vec3 _diffuse, vec3 _ambient, float _shininess, float _refCoeff);
	material();
	vec3 getColor(vec3 point, vec3 normal, vec3 p0, vec3 light, vec3 lightColor);
};

class lambert : public material {
public:
	lambert();
	lambert(vec3 _color);
};

class chrome : public material {
public:
	chrome();
};
#endif