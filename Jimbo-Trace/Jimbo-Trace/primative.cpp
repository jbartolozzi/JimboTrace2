#include "primative.h"

primative::primative() {
	polyType = SPHERE;
}

primative::primative(primativeTypes _type, material* _mat, mat4 _rotation, mat4 _translation, mat4 _scale) {
	polyType = _type;
	pMaterial = _mat;
	transformation = _translation * _rotation * _scale;
	invTransformation = inverse(transformation);
}

void primative::setTransformation( mat4 _rotation, mat4 _translation, mat4 _scale) {
	transformation = _translation * _rotation * _scale;
	invTransformation = inverse(transformation);
}