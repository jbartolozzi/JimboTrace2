#include "stubs.h"

using namespace glm;


intersectionPoint Test_RaySphereIntersectInverse(vec3 const& P0, vec3 const& V0, mat4 const& T, mat4 const& invT, primative* prim, bool isTest) {
	float radius;
	if (isTest) {
		radius = 0.6;
	}
	else {
	 radius = 0.5;
	}

	float r2 = pow(radius,2);

	intersectionPoint output;
	output.tValue = -1;
	output.normal = vec3(0,0,0);
	output.point = vec3(0,0,0);
	output.intersectionPrimative = prim;
	vec4 P02 = invT * vec4(P0,1);
	vec4 V02 = invT * vec4(V0,0);
	vec3 P03 = vec3(P02.x,P02.y,P02.z);
	vec3 V03 = vec3(V02.x,V02.y,V02.z);

	float A = glm::dot(V03,V03);
	float B = 2*(glm::dot(V03,P03));
	float C = glm::dot(P03,P03) - r2;
	float discriminant = B*B - (4*A*C);
	if (discriminant < 0) {
		return output;
	}
	else {
		float t0 = (-B - glm::sqrt(discriminant)) / 2*A;
		float t1 = (-B + glm::sqrt(discriminant)) / 2*A;
		if (t0 < 0 && t1 >= 0) {
			output.tValue = t1;
			output.normal = normalize(vec3( transpose(invT) * vec4(normalize(P03 + t1 * V03),0)));
			output.point = vec3(T * vec4(P03 + t1*V03,1));
			return output;
		}
		if (t1 < 0 && t0 >= 0) {
			output.tValue = t0;
			output.normal = normalize(vec3( transpose(invT) * vec4(normalize(P03 + t0 * V03),0)));
			output.point = vec3(T * vec4(P03 + t0*V03,1));
			return output;
		}
		float min = glm::min(t0,t1);
		if (min > 0) {
			output.tValue = min;
			output.normal = normalize(vec3( transpose(invT) * vec4(normalize(P03 + min * V03),0)));
			output.point =  vec3(T * vec4(P03 + min*V03,1)); 
			return output;
		}
		else {
			return output;
		}
	}
}

float intersectPlane(const vec3 &n, const vec3 &p0, const vec3& l0, const vec3 &l)
{
    // assuming vectors are all normalized

	float denom = dot(n,normalize(l));
	if (denom > 0) {
		float t = dot(n,(p0-l0)) / denom;
		return t;
	}
	return -1;
}

intersectionPoint Test_RayPlaneIntersectInverse(vec3 const& P0, vec3 const& V0, mat4 const& T, mat4 const& invT, primative* prim) {
	float SIZE = 5;
	intersectionPoint output;
	output.tValue = -1;
	output.normal = vec3(0,0,0);
	output.point = vec3(0,0,0);
	output.intersectionPrimative = prim;

	vec4 P02 = invT * vec4(P0,1.f);
	vec4 V02 = invT * vec4(V0,0.f);
	vec3 P03 = vec3(P02.x,P02.y,P02.z);
	vec3 V03 = vec3(V02.x,V02.y,V02.z);

	vec3 norm = normalize(vec3(0,-1,0));
	vec3 point = vec3(0,0,0);

	float t = intersectPlane(norm,point,P03,V03);

	vec3 checkpoint = P03 + t * V03;
	if (t > 0 && checkpoint.x <= 0.5*SIZE && checkpoint.x >= -0.5*SIZE && checkpoint.z <= 0.5*SIZE && checkpoint.z >= -0.5*SIZE) {
		output.point = vec3( T * vec4(checkpoint,1));
		output.normal = normalize(vec3( transpose(invT) * vec4(normalize(vec3(0,1,0)),0)));
		output.tValue = t;
	}
	return output;
}

/*intersectionPoint Test_RayPolyIntersectInverse(vec3 const& P0, vec3 const& V0, mat4 const& T, mat4 const& invT, node* _node, vector<primative::face> faceVector) {
	intersectionPoint output;
	output.tValue = -1;
	output.normal = vec3(0,0,0);
	output.point = vec3(0,0,0);
	output.node = _node;
	
	vec4 P02 = invT * vec4(P0,1);
	vec4 V02 = invT * vec4(V0,0);
	vec3 P03 = vec3(P02); //vec3(P02.x,P02.y,P02.z);
	vec3 V03 = vec3(V02);//vec3(V02.x,V02.y,V02.z);

	vector<intersectionPoint> intersectionPoints;

	for(int k = 0; k < faceVector.size(); k++) {
		vec3 p1 = faceVector[k].ind1;
		vec3 p2 = faceVector[k].ind2;
		vec3 p3 = faceVector[k].ind3;

		glm::vec3 E1 = p2 - p1;
		glm::vec3 E2 = p3 - p1;
		glm::vec3 T1 = P03 - p1;
		glm::vec3 D = V03;
		glm::vec3 P = glm::cross(D,E2);
		glm::vec3 Q = glm::cross(T1,E1);
		glm::vec3 tuv = (1.f / (glm::dot(P,E1))) * glm::vec3(glm::dot(Q,E2),glm::dot(P,T1),glm::dot(Q,D));
	
		if (tuv.y + tuv.z > 1 || tuv.y < 0 || tuv.z < 0) {
			
		}
		else {
			if (tuv.x > 0) {
				intersectionPoint ip;
				ip.tValue = tuv.x;
				ip.normal = vec3(transpose(invT) * vec4(normalize(cross(p3-p1,p2-p1)),0));
				ip.point = vec3( T * vec4((P03 + tuv.x * V03),1));
				intersectionPoints.push_back(ip);
			}
		}
	}
	if (intersectionPoints.size() < 1) {
		return output;
	}

	else {
		intersectionPoint minCheck;
		minCheck.tValue = 10000;
		for (int p = 0; p < intersectionPoints.size(); p++) {
			if (intersectionPoints[p].tValue < minCheck.tValue) {
				minCheck = intersectionPoints[p];
			}
		}
		return minCheck;
	}
	
}*/


/*
intersectionPoint Test_RayCubeIntersectInverse(vec3 const& P0, vec3 const& V0, mat4 const& T, mat4 const& invT, node* _node) {
	intersectionPoint output;
	output.tValue = -1;
	output.normal = vec3(0,0,0);
	output.point = vec3(0,0,0);
	output.node = _node;

	vec4 P02 = invT * vec4(P0,1);
	vec4 V02 = invT * vec4(V0,0.f);
	vec3 P03 = vec3(P02.x,P02.y,P02.z);
	vec3 V03 = vec3(V02.x,V02.y,V02.z);

	std::vector<float> tees;
	// top face
	glm::vec3 norm = glm::vec3(0,1.f,0);
	glm::vec3 p0 = glm::vec3(0,0.5,0);
	float t1 = intersectPlane(norm,p0,P03,V03);
	//float t1 = glm::dot((p0 - P03),norm) / glm::dot(V03,norm);

	// bottom face
	norm = glm::vec3(0,-1.f,0);
	p0 = glm::vec3(0,-0.5,0);
	float t2 = intersectPlane(norm,p0,P03,V03);
	//float t2 = glm::dot((p0 - P03),norm) / glm::dot(V03,norm);
	
	// front face
	norm = glm::vec3(0,0,1.f);
	p0 = glm::vec3(0,0,0.5);
	float t3 = intersectPlane(norm,p0,P03,V03);
	//float t3 = glm::dot((p0 - P03),norm) / glm::dot(V03,norm);

	// back face
	norm = glm::vec3(0,0,-1.f);
	p0 = glm::vec3(0,0,-0.5);
	float t4 = intersectPlane(norm,p0,P03,V03);
	//float t4 = glm::dot((p0 - P03),norm) / glm::dot(V03,norm);

	// left face
	norm = glm::vec3(-1.f,0,0);
	p0 = glm::vec3(-0.5,0,0);
	//float t5 = glm::dot((p0 - P03),norm) / glm::dot(V03,norm);
	float t5 = intersectPlane(norm,p0,P03,V03);
	
	// right face
	norm = glm::vec3(1,0,0);
	p0 = glm::vec3(0.5,0,0);
	//float t6 = glm::dot((p0 - P03),norm) / glm::dot(V03,norm);
	float t6 = intersectPlane(norm,p0,P03,V03);

	tees.push_back(t1);
	tees.push_back(t2);
	tees.push_back(t3);
	tees.push_back(t4);
	tees.push_back(t5);
	tees.push_back(t6);
	float min = 100000;
	for(unsigned int i = 0; i < tees.size(); i++) {
		if (tees[i] > 0 && tees[i] < min) {
			min = tees[i];
		}
	}
	vec3 checkPoint = P03 + min*V03;
	if (checkPoint.x <= 0.51 && checkPoint.y <= 0.51 && checkPoint.z <= 0.51
		&& checkPoint.x >= -0.51 && checkPoint.y >= -0.51 && checkPoint.z >= -0.51 && min > 0) {
			output.tValue = min;
			output.point = vec3( T * vec4(checkPoint,1.f));
			// find the largest magnitude of x,y,z position
			float xv = abs(output.point.x);
			float yv = abs(output.point.y);
			float zv = abs(output.point.z);

			// if x has the largest magnitude
			if (xv > yv && xv > zv) {
				// check which side
				if (output.point.x > 0) {
					vec3 norm = normalize(vec3(1,0,0));
					output.normal = normalize(vec3(transpose(invT) * vec4(norm,0)));
				}
				else {
					vec3 norm = normalize(vec3(-1,0,0));
					output.normal = normalize(vec3(transpose(invT) * vec4(norm,0)));
				}
			}
			// if y has the largest magnitude
			else if ( yv > xv && yv > zv) {
				if (output.point.y > 0) {
					vec3 norm = normalize(vec3(0,1,0));
					output.normal = normalize(vec3(transpose(invT) * vec4(norm,0)));
				}
				else {
					vec3 norm = normalize(vec3(0,-1,0));
					output.normal = normalize(vec3(transpose(invT) * vec4(norm,0)));
				}
			}
			// if z has the largest magnitude
			else {
				if (output.point.z > 0) {
					vec3 norm = normalize(vec3(0,0,1));
					output.normal = normalize(vec3(transpose(invT) * vec4(norm,0)));
				}
				else {
					vec3 norm = normalize(vec3(0,0,-1));
					output.normal = normalize(vec3(transpose(invT) * vec4(norm,0)));
				}
			}
			return output;
	}
	else {
		return output;
	}
}

float intersectDisk(const vec3 &n, const vec3 &p0, const float &radius, const vec3 &l0, const vec3 &l)
{
		float d1 = intersectPlane(-n, p0, l0, l);
		if (d1 > 0) {
			vec3 p = l0 + d1 * normalize(l);
			float x = p.x * p.x;
			float z = p.z * p.z;
			if (x + z <= radius*radius) {
				return d1;
			//return (sqrtf(d2) <= radius);
			// or you can use the following optimisation (and precompute radius^2)
			} // where radius2 = radius * radius
		}
		return -1;
}

intersectionPoint Test_RayCylinderIntersectInverse(vec3 const& P0, vec3 const& V0, mat4 const& T, mat4 const& invT, float width, float height, node* _node) {
	intersectionPoint output;
	output.node = _node;
	output.tValue = -1;
	output.normal = vec3(0,0,0);
	output.point = vec3(0,0,0);

	vec4 P02 = invT * vec4(P0,1);
	vec4 V02 = invT * vec4(V0,0.f);
	vec3 P03 = vec3(P02.x,P02.y,P02.z);
	vec3 V03 = vec3(V02.x,V02.y,V02.z);


	// Begin the tube test
	std::vector<float> tees;
	vec3 AB = vec3(0,0.5,0) - vec3(0,-0.5,0);
	vec3 AO = P03 - vec3(0,-0.5,0);

	float AB_dot_d  = glm::dot(AB,V03);
	float AB_dot_AO = glm::dot(AB,AO);
	float AB_dot_AB = glm::dot(AB,AB);
	float m = AB_dot_d / AB_dot_AB;
	float n = AB_dot_AO / AB_dot_AB;

	vec3 Q = V03 - (AB * m);
	vec3 R = AO - (AB * n);

	float a = glm::dot(Q,Q);
	float b = 2.f * glm::dot(Q,R);
	float c = glm::dot(R,R) - (width*width);

	float discriminant = b * b - 4.f * a * c;
	if (discriminant >= 0.f) {
		float tmin = (-b - glm::sqrt(discriminant)) / (2.f * a);
		float tmax = (-b + glm::sqrt(discriminant)) / (2.f * a);
		if (tmin < 0 && tmax >= 0) {
			tees.push_back(tmax);
		}
		else if (tmax < 0 && tmin >= 0) {
			tees.push_back(tmin);
		}
		if (tmin > tmax) {
			float temp = tmin;
			tmin = tmax;
			tmax = temp;
		}
		if (tmin >= 0) {
			tees.push_back(tmin);
		}
	}

	// find the smallest tube value
	float min = 1000000;
	for (unsigned int i = 0; i < tees.size(); i++) {
		if (tees[i] < min && tees[i] > 0) {
			min = tees[i];
		}
	}
	// check if the point is in the tube
	vec3 checkpoint = P03 + min * V03;
	if (checkpoint.y < height && checkpoint.y > -height) {
		output.point = vec3( T * vec4(checkpoint,1));
		output.normal =  normalize( vec3( transpose(invT) * vec4(normalize(vec3(output.point.x, 0, output.point.z)),0.f)));
		output.tValue = min;
		return output;
	}
	// otherwise check the caps
	else {
		// top
		vec3 norm1 = glm::vec3(0,1,0);
		vec3 p01 = glm::vec3(0,height,0);
		float dC1 = intersectDisk(norm1,p01,width,P03,V03);

		// bottom
		vec3 norm2 = glm::vec3(0,-1.0f,0);
		vec3 p02 = glm::vec3(0,-height,0);
		float dC2 = intersectDisk(norm2,p02,width,P03,V03);
		
		float minTeeCap;
		if (dC1 > 0 && dC2 < 0) {
			minTeeCap = dC1;
		}
		else if (dC1 < 0 && dC2 > 0) {
			minTeeCap = dC2;
		}
		else {
			minTeeCap = glm::min(dC1,dC2);
		}

		if (minTeeCap > 0) {
			vec4 point = vec4(P03 + minTeeCap * V03,1.f);
			output.point = vec3 ( T * point);
			output.tValue = minTeeCap;
			if (point.y > 0) {
				output.normal = normalize ( vec3 ( transpose(invT) * vec4(normalize(vec3(0.f, 1.f, 0.f)),0)));
			}
			else {
				output.normal = normalize ( vec3 ( transpose(invT) * vec4(normalize(vec3(0.f, -1.f, 0.f)),0)));
			}
		}
		return output;
	}
}
	/* otherwise check the caps
	else {
		// top
		vec3 norm1 = glm::vec3(0,1.0f,0);
		vec3 p01 = glm::vec3(0,0.5,0);
		float dC1 = intersectDisk(norm1,p01,0.5,P03,V03);

		// bottom
		vec3 norm2 = glm::vec3(0,-1.0f,0);
		vec3 p02 = glm::vec3(0,-0.5,0);
		float dC2 = intersectDisk(norm2,p02,0.5,P03,V03);

		// find the smaller of the cap values
		float minTeeCap;
		if (dC1 > 0 && dC2 < 0) {
			minTeeCap = dC1;
		}
		else if (dC1 < 0 && dC2 > 0) {
			minTeeCap = dC2;
		}
		else {
			minTeeCap = glm::min(dC1,dC2);
		}

		vec3 checkpoint = P03 + minTeeCap * V0;

	}

	/*
	// check the t values
	if (tees.size() < 1 && minTeeCap < 0) {
		return output;
	}
	else if (tees.size() > 0) {
		float min = 1000000;
		for (unsigned int i = 0; i < tees.size(); i++) {
			if (tees[i] < min && tees[i] > 0) {
				min = tees[i];
			}
		}
		
		vec3 checkPoint = P03 + min * V03;
		
		// if the intersection is outside the cap in the infinite tube
		// 
		if (checkPoint.y <= 0.500001f && checkPoint.y >= -0.5000001f) {
			output.point = checkPoint;
			vec2 norm = normalize(vec2(output.point.x,output.point.z));
			output.normal = normalize(vec3(norm.x,0,norm.y));
			output.tValue = min;

			if (minTeeCap > 0 && minTeeCap < min) {
				output.tValue = minTeeCap;
				output.point = P03 + minTeeCap * V03;
				if (output.point.y < 0.500001f && output.point.y > 0.4999999f) {
					output.normal = vec3(0,1,0);
					return output;
				}
				else if (output.point.y > -0.51f && output.point.y < -0.49f) {
					output.normal = vec3(0,-1,0);
					return output;
				}
			}
			return output;
		}
		// cap test
		else {
			output.tValue = -1;
			return output;
		}
		//otherwise, use the minimum t value
	}
	else if (minTeeCap > 0) {
		output.tValue = minTeeCap;
		output.point = P03 + minTeeCap * V03;
		if (output.point.y >= 0.49999f) {
			output.normal = vec3(0,1,0);
		}
		else if (output.point.y >= -0.499999f) {
			output.normal = vec3(0,-1,0);
		}
		return output;
	}
	
	else {
		output.tValue = -1;
		output.point = vec3(0,0,0);
		output.normal = vec3(0,0,0);
	}
	
}*/
