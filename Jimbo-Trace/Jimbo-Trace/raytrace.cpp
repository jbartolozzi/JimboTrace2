#include "raytrace.h"

raytrace::raytrace() {
	// Initial Setup
	RESOX = 1920;
	RESOX *=2;
	RESOY = 1080;
	RESOY *=2;
	nudge = 0.01;
	EYEP = vec3(0,-1.25,4);
	//VDIR = vec3(-0.575, -0.6388, -0.5111);
	VDIR = vec3(0,0,-1);
	UVEC = vec3(0.0,1.0,0.0);
	FOVY = 26.0;
	LPOS = vec3(-1.f,4,1.5);
	LCOL = vec3(1,1,1);
	ACOL = vec3(0,0,0);

	FILENAME = "output_";
	FILENAME_ZD = "zd_";
	LightTransform = translate(LPOS);
	LightInvTransform = inverse(LightTransform);
	lightPrimative = new primative();
	
	numShadow = 250;
	NUMBOUNCES = 5;
	shadowRatioMultiplier = 1.f;
	pi = 3.1415;
	lightRadius = 1;
	mainCamera = new camera(EYEP, VDIR, UVEC, FOVY, RESOX, RESOY);

	SAMPLEX = 1;
	SAMPLEY = 1;

	material *m1 = new material(vec3(0.8,0.8,0.8),ACOL,200, 0.1f);
	material *m2 = new material(vec3(0.f,0.3f,0.5f),ACOL,10.f, 0.2f);

	material *bluePhong = new material(vec3(0,0,1),ACOL,30, 0.3f);
	material *redPhong = new material(vec3(1,0,0),ACOL,30, 0.3f);
	material *greenPhong = new material(vec3(0,0.5,0),ACOL,20, 0.3f);

	chrome *chrome1 = new chrome();
	lambert *redLambert = new lambert(vec3(1,0.1,0.1));
	lambert *greenLambert = new lambert(vec3(0,1,0));
	lambert *blueLambert = new lambert(vec3(0.1,0.1,1));
	lambert *whiteLambert = new lambert(vec3(0.8,0.8,0.8));
	lambert *blackLambert = new lambert(vec3(0.3,0.3,0.3));
	lambert *purpleLambert = new lambert(vec3(0.7,0,0.7));
	lambert *yelloLambert = new lambert(vec3(0.7,0.6,0));


	// rotation translation scale
	primative *backWall = new primative   (primative::PLANE,greenPhong,rotate (90.f,vec3(1,0,0)),translate(vec3(0.f,-1.f,-2.5f)),mat4(1.f));
	primative *frontWall = new primative   (primative::PLANE,greenPhong,rotate(-90.f,vec3(1,0,0)),translate(vec3(0.f,-1.f,2.5f)),mat4(1.f));
	
	primative *rightWall = new primative   (primative::PLANE,redPhong,rotate (90.f,vec3(0,0,1)),translate(vec3(2.5f,-1.f,0.f)),mat4(1.f));
	primative *leftWall = new primative    (primative::PLANE,bluePhong,rotate(-90.f,vec3(0,0,1)),translate(vec3(-2.5f,-1.f,0.f)),mat4(1.f));

	primative *roof = new primative    (primative::PLANE,whiteLambert,rotate(180.f,vec3(1,0,0)),translate(vec3(0.f,5.f,0.f)),mat4(1.f));
	primative *floor = new primative   (primative::PLANE,whiteLambert,mat4(1.f),translate(vec3(0.f,-3.f,0.f)),mat4(1.f));

	sphere  = new primative  (primative::SPHERE,chrome1,mat4(1.f),        translate(vec3(0.f,-1.2,0)),mat4(1.f));
	float rad = radians((float) frameNumber);
	float dS = 1.f;
	sphere2 = new primative  (primative::SPHERE,chrome1,mat4(1.f),        translate(vec3(0,cos(rad)*dS -1.2, sin(rad) * dS)),mat4(1.f));
	sphere3 = new primative  (primative::SPHERE,chrome1,mat4(1.f),        translate(vec3(sin(rad)  *dS, -1.2, cos(rad) * dS)),mat4(1.f));
	sphere4 = new primative  (primative::SPHERE,chrome1,mat4(1.f),        translate(vec3(cos(rad)  *dS, sin(rad)*dS -1.2,0)),mat4(1.f));
	
	output.SetSize(RESOX, RESOY);
	output.SetBitDepth(24);

	
	zDepth.SetSize(RESOX, RESOY);
	zDepth.SetBitDepth(24);

	primatives.push_back(backWall);
	primatives.push_back(frontWall);
	primatives.push_back(rightWall);
	primatives.push_back(leftWall);
	//primatives.push_back(roof);
	primatives.push_back(floor);
	primatives.push_back(sphere);
	primatives.push_back(sphere2);
	primatives.push_back(sphere3);
	primatives.push_back(sphere4);
	//primatives.push_back(sphere3);
	std::cout << "Performing Jimbo-Trace" << std::endl;

	for (int i = 0; i <= RESOX; i++) {
		for (int j = 0; j < RESOY; j++) {
			depths.push_back(-1);
		}
	}

}

// INPUT: thread number
// renders vertical sections of image output based on thread number
void raytrace::runRaytrace(int thread) {
	int start = thread * RESOX/8;
	int end = start + RESOX/8;
	float rad = radians((float) frameNumber);
	sphere2->setTransformation(mat4(1.f),translate(vec3(0, cos(rad) *1 -1.2, sin(rad) * 1)),mat4(1.f));
	sphere3->setTransformation(mat4(1.f),translate(vec3(sin(rad)    * 1, -1.2, cos(rad) * 1)),mat4(1.f));
	sphere4->setTransformation(mat4(1.f),translate(vec3(cos(rad)    *1, sin(rad)*1 -1.2,0)),mat4(1.f));

	vec3 outR;

	
	for (int i = start; i < end; i++) {
		for (int j = 0; j < RESOY; j++) {
			int xPixel = i;
			int yPixel = (RESOY -1) - j;

			vec3 color = getPixelSample(i,j,mainCamera);
			output(xPixel,yPixel)->Red   = color.x * 255.f;
			output(xPixel,yPixel)->Green = color.y * 255.f;
			output(xPixel,yPixel)->Blue  = color.z * 255.f;

		}
		if (thread == 0 && i%50 == 0) {
			std::cout << (float)i/(float)end * 100.f << "%" << std::endl;
		}
	}
}

// Input: p0= point, v0 = direction vector, iteration = number of iterations
// Output: color value from calculated reflection
vec3 raytrace::reflectionTrace(vec3 p0, vec3 v0, int iteration) {
	intersectionPoint outputIntersection = recursiveTrace(p0 + nudge*v0,v0,false);
	int iter2 = iteration - 1;
	if (outputIntersection.tValue > 0 && iteration > 0) {
		float reflec = outputIntersection.intersectionPrimative->pMaterial->reflectivityCoefficient;
		vec3 mater = outputIntersection.intersectionPrimative->pMaterial->getColor(outputIntersection.point,outputIntersection.normal,p0,LPOS,LCOL);
		vec3 color;
		
		// run preliminary shadow tests with 25 random points on a sphere
		int occlusions = 0;
		for(int q = 0; q < 20; q++) {
			float u = (rand() % 101)/100.f;
			float v = (rand() % 101)/100.f;

			float q1 = 2 * pi * u;
			float f = acos(2*v-1);
			vec3 lightPoint = LPOS + vec3(lightRadius*cos(q1)*sin(f),lightRadius*sin(q1)*sin(f),lightRadius*cos(f));
			intersectionPoint shadowIntersection1 = recursiveTrace(outputIntersection.point + nudge*normalize(outputIntersection.normal), normalize(lightPoint - outputIntersection.point),false);
			if (shadowIntersection1.tValue > 0.0001) {
				occlusions++;
			}
		}
		// if the light intersections are all blocked, then return the shadow color
		if (occlusions == 20) {
			color = ACOL;
		}
		// if the light intersections all make it to the light then return the material color
		else if (occlusions == 0) {
			if (outputIntersection.intersectionPrimative->pMaterial->reflectivityCoefficient == 0) {
				color = mater;
			}
			else {
				color = (1-reflec) * mater +  (reflec *  reflectionTrace(outputIntersection.point, normalize(reflect(outputIntersection.point - p0, outputIntersection.normal)),iter2));
			}
		}
		// otherwise calculate the softshadow color value
		else {
			int numOccluded = 0;
			for(int q = 0; q < numShadow; q++) {
				float u = (rand() % 100)/100.f;
				float v = (rand() % 100)/100.f;

				float q1 = 2 * pi * u;
				float f = acos(2*v-1);

				vec3 lightPoint = LPOS + vec3(lightRadius*cos(q1)*sin(f),lightRadius*sin(q1)*sin(f),lightRadius*cos(f));

				intersectionPoint shadowIntersection = recursiveTrace(outputIntersection.point + nudge*normalize(outputIntersection.normal), normalize(lightPoint - outputIntersection.point),false);
				if (shadowIntersection.tValue > 0.0001) {
					numOccluded++;
				}
			}

			if (numOccluded > 0) {
				float ratio = ((float)numOccluded/(float)numShadow);
				ratio *= shadowRatioMultiplier;
				// return shadow
				return ratio*ACOL + (1-ratio)*((1-reflec)*mater + (reflec *  reflectionTrace(outputIntersection.point, normalize(reflect(outputIntersection.point - p0, outputIntersection.normal)),iter2)));
					
				//return ACOL +  (reflec *  reflectionTrace(outputIntersection.point, normalize(reflect(outputIntersection.point - p0, outputIntersection.normal)),iter2));
			}
			else {
				if (outputIntersection.intersectionPrimative->pMaterial->reflectivityCoefficient == 0) {
					color = mater;
				}
				else {
					color = (1-reflec) * mater +  (reflec *  reflectionTrace(outputIntersection.point, normalize(reflect(outputIntersection.point - p0, outputIntersection.normal)),iter2));
				}
			}
		}
		// return the color
		return color;
	}
	// if there is no intersection
	else {
		// either return the material
		if (outputIntersection.tValue > 0) {
			return outputIntersection.intersectionPrimative->pMaterial->getColor(outputIntersection.point,outputIntersection.normal,p0,LPOS,LCOL);
		}
		// or return white as the reflection color
		else {
			return vec3(1,1,1);
		}
	}
}

// Input: eye position, ray direction, shadow check is obsolete
// Output: stubs::intersection point with necessary material and point intersection information

intersectionPoint raytrace::recursiveTrace(vec3 eye, vec3 ray, bool shadowCheck) {
	std::vector<intersectionPoint> intersectionPoints;
	for (int j = 0; j < primatives.size(); j++) {
		intersectionPoint is;
		// check if the poly is a sphere
		mat4 transform     = primatives[j]->transformation;
		mat4 invTransform  = primatives[j]->invTransformation;

		if (primatives[j]->polyType == primative::primativeTypes::SPHERE) {
			is = Test_RaySphereIntersectInverse(eye,ray,transform,invTransform,primatives[j],shadowCheck);
			if (is.tValue > 0) {
				intersectionPoints.push_back(is);
			}
		}

		//plane
		else if (primatives[j]->polyType == primative::primativeTypes::PLANE) {
			is = Test_RayPlaneIntersectInverse(eye,ray,transform,invTransform,primatives[j]);
			if (is.tValue > 0) {
				intersectionPoints.push_back(is);
			}
		}
	}
	
	//if there were any intersections, go through them and find the smallest
	if (intersectionPoints.size() > 0) {
		intersectionPoint minT = intersectionPoints[0];
		//for the size of the array set minT to the smallest t value intersection
		for (int i = 1; i < intersectionPoints.size(); i++) {
			if (intersectionPoints[i].tValue < minT.tValue && intersectionPoints[i].tValue > 0) {
				minT = intersectionPoints[i];
			}
		}
		return minT;
	}
	else {
		intersectionPoint failed;
		failed.tValue = -1;
		return failed;
	}
}

// Complete the raytrace and write out the files
void raytrace::finishRaytrace() {
	constructDepthBMP();
	char* filename = FILENAME;
	int size = strlen(filename)+4+frameNumber;
	char* str = new char[size];
	char integer_string[32];
	std::sprintf(integer_string, "%d", frameNumber);
	std::strcpy (str,filename);
	std::strcat (str,integer_string);
	std::strcat (str,".bmp");
	output.WriteToFile(str);
	std::cout << "Successfully wrote file named: " << str << std::endl;

	char* filename2 = FILENAME_ZD;
	int size2 = strlen(filename)+4+frameNumber;
	char* str2 = new char[size2];
	char integer_string2[32];
	std::sprintf(integer_string2, "%d", frameNumber);
	std::strcpy (str2,filename2);
	std::strcat (str2,integer_string2);
	std::strcat (str2,".bmp");
	zDepth.WriteToFile(str2);
	std::cout << "Successfully wrote file named: " << str2 << std::endl;

	
}

// sets the current frame number for the render
void raytrace::setFrame(int f) {
	frameNumber = f;
}

// Inputs: i = x pixel value, j = y pixel value, mainCamera is the camera
// Output: the averaged antialiased pixel RGB value
// ALSO responsible for pushing depth values to the depth map
vec3 raytrace::getPixelSample(int i, int j, camera* mainCamera) {
	vec3 SUM_COLOR = vec3(0,0,0);
	float SUM_DEPTH = 0;
	float start = i - (SAMPLEX/2) * (1/SAMPLEX); 
	for(int u = 0; u < SAMPLEX; u++){
		for(int v = 0; v < SAMPLEY; v++){
			float xCoordinate = i+u*(1/SAMPLEX);
			float yCoordinate = j+v*(1/SAMPLEY);

			// Perform raytrace per pixel
			vec3 ray = mainCamera->getDirectionFromCoordinate(xCoordinate,yCoordinate);
			intersectionPoint outputIntersection = recursiveTrace(mainCamera->eye,ray,false);
			if (outputIntersection.tValue > 0) {
				vec3 color;
				//depths[i+RESOX*j] = length(outputIntersection.point - mainCamera->eye);
				SUM_DEPTH += (length(outputIntersection.point.z - EYEP.z));

				float reflec = outputIntersection.intersectionPrimative->pMaterial->reflectivityCoefficient;
				vec3 mater = outputIntersection.intersectionPrimative->pMaterial->getColor(outputIntersection.point,outputIntersection.normal,mainCamera->eye,LPOS,LCOL);
				//get shadow
				// cast trace from point to light, if there is an inerseciton, return ambient
		
				// run preliminary light test
				int occlusions = -1;
				for(int q = 0; q < 40; q++) {
					float u = (rand() % 101)/100.f;
					float v = (rand() % 101)/100.f;

					float q1 = 2 * pi * u;
					float f = acos(2*v-1);

					vec3 lightPoint = LPOS + vec3(lightRadius*cos(q1)*sin(f),lightRadius*sin(q1)*sin(f),lightRadius*cos(f));

					intersectionPoint shadowIntersection1 = recursiveTrace(outputIntersection.point + nudge*normalize(outputIntersection.normal), normalize(lightPoint - outputIntersection.point),false);
					if (shadowIntersection1.tValue > 0.0001) {
						occlusions++;
					}
				}
				// no shadow
				if (occlusions < 0) {
					if (outputIntersection.intersectionPrimative->pMaterial->reflectivityCoefficient == 0) {
						color = mater;
						SUM_COLOR += color;
					}
					else {
						color = (1-reflec) * mater + (reflec *  reflectionTrace(outputIntersection.point, normalize(reflect(outputIntersection.point - mainCamera->eye, outputIntersection.normal)),NUMBOUNCES));
						
						SUM_COLOR += color;
					}
				}
				
				// calculate shadows
				else {
					int numOccluded = 0;
					vec3 finalC = vec3(0,0,0);
					for(int q = 0; q < numShadow; q++) {
						float u = (rand() % 101)/100.f;
						float v = (rand() % 101)/100.f;

						float q1 = 2 * pi * u;
						float f = acos(2*v-1);

						vec3 lightPoint = LPOS + vec3(lightRadius*cos(q1)*sin(f),lightRadius*sin(q1)*sin(f),lightRadius*cos(f));

						intersectionPoint shadowIntersection = recursiveTrace(outputIntersection.point + nudge*normalize(outputIntersection.normal), normalize(lightPoint - outputIntersection.point),false);
						if (shadowIntersection.tValue > 0.0001) {
							numOccluded++;
						}
					}

					if (numOccluded > 0) {
						float ratio = ((float)numOccluded/(float)numShadow);
						ratio *= shadowRatioMultiplier;
						color = ratio*ACOL + (1-ratio)* ((1-reflec)*mater + (reflec *  reflectionTrace(outputIntersection.point, normalize(reflect(outputIntersection.point - mainCamera->eye, outputIntersection.normal)),NUMBOUNCES)));
						SUM_COLOR += color;
					}
					else {
						if (outputIntersection.intersectionPrimative->pMaterial->reflectivityCoefficient == 0) {
							color = mater;
							SUM_COLOR += color;
						}
						else {
							color = (1-reflec) * mater + (reflec *  reflectionTrace(outputIntersection.point, normalize(reflect(outputIntersection.point - mainCamera->eye, outputIntersection.normal)),NUMBOUNCES));
						
							SUM_COLOR += color;
						}
					}
				}

				/*int numOccluded = 0;
				vec3 finalC = vec3(0,0,0);
				for(int q = 0; q < numShadow; q++) {
					float u = (rand() % 101)/100.f;
					float v = (rand() % 101)/100.f;

					float q1 = 2 * pi * u;
					float f = acos(2*v-1);

					vec3 lightPoint = LPOS + vec3(lightRadius*cos(q1)*sin(f),lightRadius*sin(q1)*sin(f),lightRadius*cos(f));

					intersectionPoint shadowIntersection = recursiveTrace(outputIntersection.point + nudge*normalize(outputIntersection.normal), normalize(lightPoint - outputIntersection.point),false);
					if (shadowIntersection.tValue > 0.0001) {
						numOccluded++;
					}
				}

				if (numOccluded > 0) {
					float ratio = ((float)numOccluded/(float)numShadow);
					ratio *= shadowRatioMultiplier;
					color = ratio*ACOL + (1-ratio)* ((1-reflec)*mater + (reflec *  reflectionTrace(outputIntersection.point, normalize(reflect(outputIntersection.point - mainCamera->eye, outputIntersection.normal)),NUMBOUNCES)));
					
					SUM_COLOR += color;
				}

				else {
					if (outputIntersection.intersectionPrimative->pMaterial->reflectivityCoefficient == 0) {
						color = mater;
						
						SUM_COLOR += color;
					}
					else {
						color = (1-reflec) * mater + (reflec *  reflectionTrace(outputIntersection.point, normalize(reflect(outputIntersection.point - mainCamera->eye, outputIntersection.normal)),NUMBOUNCES));
						
						SUM_COLOR += color;
					}
				}*/
			//depths.push_back(length(outputIntersection.point - EYEP));
			}
			else {
				SUM_COLOR += vec3(1,1,1);
			}
		}
	}
	float rat = ((float) SAMPLEX * (float) SAMPLEY);
	if (SUM_DEPTH > 0.001) {
		depths[i + RESOX*j] = (SUM_DEPTH/rat);
	}
	return vec3(SUM_COLOR) / rat;
}

// Constructs the depth map BMP
void raytrace::constructDepthBMP() {
	float maxlength = 7.5f;
	float minlength = 1.f;
	/*for(int k = 0; k < depths.size(); k++) {
		if (depths[k] > maxlength) {
			maxlength = depths[k];
		}
		if (depths[k] < minlength && depths[k] > 0) {
			minlength = depths[k];
		}
	}*/

	for (int i = 0; i < RESOX; i++) {
		for (int j = 0; j < RESOY; j++) {
			int xPixel = i;
			int yPixel = (RESOY -1) - j;
			float val = depths[i + RESOX * (j)];
			float ratio = (maxlength + minlength) / (maxlength - minlength);
			float ratio2 = (-2*maxlength*minlength) / (maxlength - minlength);

			float output = (ratio + (1/val)*ratio2);
			if (val != -1) {
				output = 1-output;
				zDepth(xPixel,yPixel)->Red =   clamp(output*255,0.f,255.f);
				zDepth(xPixel,yPixel)->Green = clamp(output*255,0.f,255.f);
				zDepth(xPixel,yPixel)->Blue =  clamp(output*255,0.f,255.f);
			}
			else {
				zDepth(xPixel,yPixel)->Red = 0;
				zDepth(xPixel,yPixel)->Green = 0;
				zDepth(xPixel,yPixel)->Blue = 0;
			}
		}
	}
}