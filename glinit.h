#ifndef _GL_INIT_H_
#define _GL_INIT_H_

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "GLee.h"
#include <GL/gl.h>   
#include <GL/glut.h> 
#include <jpeglib.h>
#include "utils.h"

using namespace std;

extern GLuint gProgramShader;	// used in shader program generation -- Externed from main.cpp

extern GLuint gTextureMap; // OpenGL texture ID

void initShaders(); // function to initialize shaders

void initTexture(char *, int *, int *); // function to read a texture
					// the first parameter is the texture file name
					// the last two parameters are the returned width and height values

#endif
