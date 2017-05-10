//
//

#ifndef Utilities_h
#define Utilities_h

#include <iostream>
#include <stdio.h>
#include <GLFW/glfw3.h>
//#include <OpenGL/gl3.h>
#ifdef CMAKE
	#include <OpenNI.h>
#else
	#include <openni/OpenNI.h>
#endif
#include <cmath>

using namespace openni;
using namespace std;

bool checkGLErrors();
void queryGLVersion();

bool initalizeOpenNI();
void closeOpenNI();


#endif /* Utilities_h */
