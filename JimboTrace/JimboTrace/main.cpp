//
//  main.cpp
//  JimboTrace
//
//  Created by James Bartolozzi on 9/10/14.
//  Copyright (c) 2014 LinearAlgebros. All rights reserved.
//

#include <iostream>
#include "EasyBMP.h"
#include "World.h"
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#define PATH_TRACER_BITMAP 13

World *w;
int iterations;

void displayFunction()
{
    iterations++;
    // array = pathTracer->render();
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // Update the texture:
    int imageWidth = w->MainCamera->imgWidth;
    int imageHeight = w->MainCamera->imgHeight;
    float* imageData = w->Render();
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 1, GL_RGB, GL_FLOAT, imageData);
    // glTexImage2D makes a copy of the data, so the original data can (and should!) be deleted here (otherwise it will leak memory like a madman).
    
    // Show the texture:
    glBindTexture (GL_TEXTURE_2D, PATH_TRACER_BITMAP);
    glBegin (GL_QUADS);
    glTexCoord2f (0.0, 0.0);
    glVertex3f (0.0, 1.0, 0.0);
    glTexCoord2f (1.0, 0.0);
    glVertex3f (1.0, 1.0, 0.0);
    glTexCoord2f (1.0, 1.0);
    glVertex3f (1.0, 0.0, 0.0);
    glTexCoord2f (0.0, 1.0);
    glVertex3f (0.0, 0.0, 0.0);
    glEnd ();
    
    // Write the iteration count and other information to the display:
    glPushAttrib(GL_LIGHTING_BIT);
    glDisable(GL_LIGHTING);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //glColor4f(1.0, 1.0, 1.0, 1.0);
    char info[1024];
    
    glRasterPos2f(0.5, 0);
    sprintf(info, "Iterations: %u", iterations);
    for (unsigned int i = 0; i < strlen(info); i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, info[i]);
    }
    
    //glColor4f(1.0, 1.0, 1.0, 1.0);
    //
    glPopAttrib();
    glutSwapBuffers();
    glutPostRedisplay();
}

int main(int argc, const char * argv[])
{
    w = new World();
    iterations = 0;
    glutInit(&argc, const_cast<char**>(argv));
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(w->MainCamera->imgWidth, w->MainCamera->imgHeight);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Jimbo Trace 2");
    glutDisplayFunc(displayFunction);
    //glutKeyboardFunc(onKeyboardCb);
    //glutSpecialFunc(onKeyboardSpecialCb);
    //glutMouseFunc(onMouseCb);
    //glutMotionFunc(onMouseMotionCb);
    //glutTimerFunc(100, onTimerCb, 0);
    //glutReshapeFunc(onResizeCb);
    
    // Set up an orthographic view:
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,1,0,1,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    
    // Create a texture for displaying the render:
    // TODO: Move to a function.
    glBindTexture(GL_TEXTURE_2D, PATH_TRACER_BITMAP);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Use nearest-neighbor point sampling instead of linear interpolation:
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    // Enable textures:
    glEnable(GL_TEXTURE_2D);
    
    glutMainLoop();
    return 0;
}

