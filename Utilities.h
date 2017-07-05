//
//

#ifndef Utilities_h
#define Utilities_h

#include <iostream>
#include <stdio.h>
#include <GLFW/glfw3.h>

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
#endif

//A thing I use to allow CMAKE to build without interrupting your build system.
#ifdef CMAKE
	#include <OpenNI.h>
#else
	#include <openni2/OpenNI.h>
#endif
#include <cmath>

using namespace openni;
using namespace std;

bool checkGLErrors();
void queryGLVersion();

bool initalizeOpenNI();
void closeOpenNI();


#endif /* Utilities_h */
