//
//  World.cpp
//  JimboTrace
//
//  Created by James Bartolozzi on 9/14/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#include "World.h"

World::World()
{
    
    BlinnPhong *s1 = new BlinnPhong();
    BlinnPhong *s2 = new BlinnPhong(vec3(1,0,0));
    BlinnPhong *s3 = new BlinnPhong(vec3(0,1,0));
    BlinnPhong *s4 = new BlinnPhong(vec3(0,0,1));
    CheckerBoard *s5 = new CheckerBoard();
    
    Sphere *obj3 = new Sphere(1.f,vec3(0,0,0),s1);
    Objects.push_back(obj3);
    
    Sphere *obj4 = new Sphere(0.5f,vec3(-2,1,0),s1);
    //Objects.push_back(obj4);
    Sphere *obj6 = new Sphere(0.5f,vec3(-2,-1,0),s1);
    //Objects.push_back(obj6);
    Sphere *obj5 = new Sphere(0.5f,vec3(2,1,0),s1);
    //Objects.push_back(obj5);
    Sphere *obj7 = new Sphere(0.5f,vec3(2,-1,0),s1);
    //Objects.push_back(obj7);
    
    
    Plane *p1 = new Plane(1,1,s1);
    p1->transform(vec3(0,-2.5,0), vec3(0.f), vec3(20,1,10));
    Objects.push_back(p1);
    
    Plane *p2 = new Plane(1,1,s1);
    p2->transform(vec3(0,2.5,0), vec3(180.f,0,0), vec3(20,1,10));
    Objects.push_back(p2);
    
    Plane *p3 = new Plane(1,1,s1);
    p3->transform(vec3(0,0,-3), vec3(90.f,0,0), vec3(20,1,10));
    Objects.push_back(p3);
    
    Plane *p4 = new Plane(1,1,s2);
    p4->transform(vec3(-3,0,0), vec3(0,0,-90), vec3(20,1,10));
    Objects.push_back(p4);
    
    Plane *p5 = new Plane(1,1,s3);
    p5->transform(vec3(3,0,0), vec3(0,0,90), vec3(20,1,10));
    Objects.push_back(p5);
    
    Plane *p6 = new Plane(1,1,s1);
    p6->transform(vec3(0,0,11), vec3(-90,0,0), vec3(20,1,10));
    Objects.push_back(p6);
    
    
    
    createLight(vec3(1,1,1), vec3(1.75,0.5,1),0.3f);
    //createLight(vec3(1,1,1), vec3(0,1,10), 1.f);
    createLight(vec3(1,1,1), vec3(-1.75,-0.5,1), 0.3f);
    
    MainCamera = new Camera();
    perlin = new Perlin(3, 0.01, 2, 123);
    outputImage.SetSize(MainCamera->imgWidth, MainCamera->imgHeight);
    outputImage.SetBitDepth(24);
    
    passCounter = 0;
    
    //////////////////////////////////////////////////////////////////
    
    totalPasses = 1280;
    sampleX = 3;
    sampleY = 3;
    rCol = new float*[MainCamera->imgWidth];
    gCol = new float*[MainCamera->imgWidth];
    bCol = new float*[MainCamera->imgWidth];
    Hits = new int*[MainCamera->imgWidth];
    for(int i = 0; i < MainCamera->imgWidth;i ++)
    {
        rCol[i] = new float[MainCamera->imgHeight];
        gCol[i] = new float[MainCamera->imgHeight];
        bCol[i] = new float[MainCamera->imgHeight];
        Hits[i] = new int[MainCamera->imgHeight];
    }
    for (int i = 0; i < MainCamera->imgWidth; i++)
    {
        for (int j = 0; j < MainCamera->imgHeight; j++)
        {
            Hits[i][j] = 0;
        }
    }
    OUTCOLORS = new float[MainCamera->imgWidth * MainCamera->imgHeight * 3];
}

float* World::Render()
{
    // For each horizontal pixel
    for (int i = 0; i < MainCamera->imgWidth; i++)
    {
        // For each vertical pixel
        for (int j = 0; j < MainCamera->imgHeight; j++) {
            vec3 tCol = vec3(0.0);
            vec3 fCol = vec3(1.0);
            vec3 sCol,dCol,eCol;
            vec3 p1 = MainCamera->eye;
            vec3 v1 = MainCamera->getDirectionFromCoordinate(i, j,sampleX,sampleY,0);
            int objid = -1;
            for (int pathIteration = 0; pathIteration < 10; pathIteration++)
            {
                IntersectionPoint check = IntersectWorld(p1,v1,objid, true);
                if (check.didIntersect)
                {
                    if (Objects[check.objid]->isLight && pathIteration == 0)
                    {
                        int asd = 1;
                    }
                    // get surface color and direct lighting for each light
                    sCol = getSurfaceColor(check.p, check.n, check.objid, 0);
                    //dCol = getDirectLightingColor(check.p, check.n, 0, 0, check.objid);
                    dCol = Objects[check.objid]->emittance;
                    // prepare ray for indirect lighting gathering
                   
                    fCol *= sCol;
                    tCol += fCol*dCol;
                    if (Objects[check.objid]->isLight)
                    {
                        Hits[i][j]++;
                        break;
                    }
                    else if(pathIteration > 5 && (fCol.r + fCol.g + fCol.b) < 0.5)
                    {
                        break;
                    }
                }
                //else if (pathIteration == 0){
                    //tCol += getBackgroundColor(i, j);
                //}
                p1 = check.p + 0.001f*check.n;
                v1 = getBRDF(check.p, check.n, 90.f);
            }
            // gamma correction
            rCol[i][j] += powf(tCol.r,0.45f);
            gCol[i][j] += powf(tCol.g,0.45f);
            bCol[i][j] += powf(tCol.b,0.45f);
        }
    }
    
    for (int i = 0; i < MainCamera->imgWidth; i++)
    {
        for (int j = 0; j < MainCamera->imgHeight; j++)
        {
            //(j*w + i) * 3 + k
            int index = (j*MainCamera->imgWidth + i)*3;
            OUTCOLORS[index] =   rCol[i][j]/(float)Hits[i][j];
            OUTCOLORS[index+1] = gCol[i][j]/(float)Hits[i][j];
            OUTCOLORS[index+2] = bCol[i][j]/(float)Hits[i][j];
        }
    }
    return OUTCOLORS;
    /*
    // Copy color data from array to image output
    for (int i = 0; i < MainCamera->imgWidth; i++)
    {
        for(int j = 0; j < MainCamera->imgHeight; j++)
        {
            float a = rCol[i][j];
            float b = 255.f * a / (float)totalPasses;
            int c = MainCamera->imgHeight - j;

            outputImage(i,j)->Red = (255.f*rCol[i][c])/(float)Hits[i][j];
            outputImage(i,j)->Green = (255.f*gCol[i][c])/(float)Hits[i][j];
            outputImage(i,j)->Blue = (255.f*bCol[i][c])/(float)Hits[i][j];
        }
    }
    outputImage.WriteToFile("Output.bmp");
    */
}

IntersectionPoint World::IntersectWorld(vec3 pi, vec3 d, int objid, bool checkLights)
{
    vec3 n,t,pf;
    float minT = 1000000;
    IntersectionPoint min;
    for (int i = 0; i < Objects.size(); i++)
    {
        /*if (Objects[i]->isLight && !checkLights)
            continue;
        else {*/
            IntersectionPoint output = Objects[i]->intersectShape(pi, d);
            if (output.didIntersect && output.tValue < minT)
            {
                minT = output.tValue;
                output.objid = i;
                min = output;
                min.didIntersect = true;
            }
        //}
    }
    //IntersectionPoint(bool dI, vec3 iL, vec3 iN, float t)
    return min;
}

vec3 World::getSurfaceColor(vec3 p, vec3 n, int objid, int seed)
{
    if (!Objects[objid]->isLight)
    {
        vec3 output = vec3(0,0,0);
        for (int i = 0; i < Lights.size(); i++)
        {
            output += Objects[objid]->objShader->getColorValue(p, n, MainCamera->eye, Lights[i]->position, Lights[i]->color);
        }
        output = output/(float)Lights.size();
        return output;
    }
    else
    {
        return Objects[objid]->emittance;
    }
}

vec3 World::getDirectLightingColor(vec3 p, vec3 n, int lightNum, int seed, int objid)
{
    float numHits;
    p = p + 0.001f*n;
    for (int i = 0; i < Lights.size(); i++)
    {
        vec3 lp = Lights[i]->position;
        vec3 n2 = normalize(vec3(lp - p));
        IntersectionPoint check = IntersectWorld(p,n2,objid,false);
        float distToLight = distance(Lights[lightNum]->position,p);
        float distToObject = distance(check.p, p);
        if (!check.didIntersect || distToLight <= distToObject)
        {
            numHits++;
        }
    }
    return vec3(numHits/(float)Lights.size());
}

vec3 World::getBackgroundColor(int sx, int sy)
{
    //float p = min(max(perlin->Get(sx, sy),0.1f),1.f);
    float xn = (float)sx / (float) MainCamera->imgWidth;
    float yn = (float)sy / (float) MainCamera->imgHeight;
    float cX = (MainCamera->imgWidth/2.f)/(MainCamera->imgWidth);
    float cY = (MainCamera->imgHeight/2.f)/(MainCamera->imgHeight);
    vec2 a = vec2(cX,cY);
    vec2 b = vec2(xn,yn);
    float dist = 0.5*(1-distance(a,b));
    vec3 output;
    int gridsize = 100;
    /*if (sx % gridsize == 0)
        output = vec3(0,0,0);
    else if (sy % gridsize == 0)
        output = vec3(0,0,0);
    else*/
        output = vec3(dist,dist,dist);
    return output;
}

void World::createLight(vec3 color, vec3 position, float radius)
{
    Light *li = new Light(color,position,radius);
    Lights.push_back(li);
    LightShader *ls = new LightShader(color);
    Sphere *sp = new Sphere(radius,position,ls);
    sp->emittance = color;
    sp->isLight = true;
    Objects.push_back(sp);
}

vec3 World::getBRDF(vec3 p, vec3 n, float sa)
{
    vec3 output;
    // create new axes for axis aligned hemisphere
    vec3 N1 = n;
    vec3 axis;
    if (n.x < n.y && n.x < n.z)
    {
        axis = vec3(1,0,0);
    }
    else if (n.y < n.z)
    {
        axis = vec3(0,1,0);
    }
    else
    {
        axis = vec3(0,0,1);
    }
    vec3 N2 = normalize(cross(n,axis));
    vec3 N3 = normalize(cross(n,N2));
    // convert sa from degrees to radians
    float sa2 = sa * M_PI/180.f;
    // get random point on z up aligned hemisphere
    MTRand r;
    float r1 = r();
    float r2 = r();
    float x = cos(2*M_PI*r1)*sqrt(1-pow((1-r2*(1-cos(sa2))),2));
    float y = sin(2*M_PI*r1)*sqrt(1-pow((1-r2*(1-cos(sa2))),2));
    float z = 1 - r2*(1-cos(sa2));
    // transform random ray
    output = N2*x + N3*y + n*z;
    return output;
}

/*
 vec3 rendererCalculateColor( vec3 ro, vec3 rd, int numLevels )
 {
 vec3 tcol = vec3(0.0);
 vec3 fcol = vec3(1.0);
 
 // create numLevels light paths iteratively
 for( int i=0; i < numLevels; i++ )
 {
 // intersect scene
 vec2 tres = worldIntersect( ro, rd, 1000.0 );
 
 // if nothing found, return background color or break
 if( tres.y < 0.0 )
 {
 if( i==0 )  fcol = worldGetBackground( rd );
 else        break;
 }
 // get position and normal at the intersection point
 vec3 pos = ro + rd * tres.x;
 vec3 nor = worldGetNormal( pos, tres.y );
 
 // get color for the surface
 vec3 scol = worldGetColor( pos, nor, tres.y );
 
 // compute direct lighting
 vec3 dcol = worldApplyLighting( pos, nor );
 
 // prepare ray for indirect lighting gathering
 ro = pos;
 rd = worldGetBRDFRay( pos, nor, rd, tres.y );
 
 // surface * lighting
 fcol *= scol;
 tcol += fcol * dcol;
 }
 
 return tcol;
 }*/
