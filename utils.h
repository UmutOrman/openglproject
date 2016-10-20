#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <unistd.h>
#include <sys/time.h>
#include "GLee.h"
#include <GL/gl.h>   
#include <GL/glut.h> 

using namespace std;

// function to read and link a vertex shader
GLuint createVS(const string& filename);

// function to read and link a fragment shader
GLuint createFS(const string& filename);

// function to read lines from a file and construct a long string of these lines
bool readDataFromFile(const string& fileName, string &data);

long getCurrentTime();

#endif
