#include "camera.h"

camera::camera(vec3 _eye, vec3 _view, vec3 _up, float _fovy, int _imgWidth, int _imgHeight) {
	eye = _eye;
	view = _view;
	up = _up;
	fovY = radians(_fovy);
	
	imgWidth = _imgWidth;
	imgHeight = _imgHeight;
	camSet = false;

	fovX = atan(((float)imgWidth/(float)imgHeight)*tan(fovY));
}

vec3 camera::getDirectionFromCoordinate(int sx, int sy) {
	if (!camSet) { 
		// A = C x U
		glm::vec3 A = glm::cross(view,up);
		// B = A x C
		glm::vec3 B = glm::cross(A,view);
		// M = E + C
		M = eye + view;
		// H = (A|C|tan(theta))/|A|
		H = (A * glm::length(view) * glm::tan(fovX))/(glm::length(A));
		// V = (B|C|tan(phi))/|B|
		V = (B * glm::length(view) * glm::tan(fovY))/(glm::length(B));
		camSet = true;
	}
	glm::vec3 P = M + (2*((float)sx/(imgWidth-1)) - 1)*H + (2*((float)sy/(imgHeight-1)) - 1)*V;
	return normalize(P-eye);
}
