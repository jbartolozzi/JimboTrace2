#ifndef PRIMATIVE
#define PRIMATIVE
#include "material.h"

#include "../glm/gtx/transform.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "glew.h"
#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"

#include "material.h"

using namespace glm;

class primative {
public:
	enum primativeTypes {
		SPHERE,PLANE
	};
	material* pMaterial;
	mat4 transformation;
	mat4 invTransformation;
	primativeTypes polyType;

	primative(primativeTypes type, material* mat, mat4 rotation, mat4 translation, mat4 scale);
	primative();

	void setTransformation( mat4 rotation, mat4 translation, mat4 scale);
};


#endif