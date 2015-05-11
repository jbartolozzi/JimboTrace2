#ifndef CAMERA
#define CAMERA

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtx/transform.hpp"

using namespace glm;

class camera {
private:
	vec3 M;
	vec3 H;
	vec3 V;
public:
	bool camSet;
	vec3 up;
	vec3 eye;
	vec3 view;
	float fovX;
	float fovY;
	int imgWidth;
	int imgHeight;
	vec3 getDirectionFromCoordinate(int sx, int sy);
	camera(vec3 _eye, vec3 _view, vec3 _up, float _fovy, int _imgWidth, int _imgHeight);
};

#endif