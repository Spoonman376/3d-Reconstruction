//
//

#ifndef main_h
#define main_h

#include "Utilities.h"
#include "ImageSaver.h"
#include "Scanner.h"

#include <boost/program_options.hpp>


Scanner* scanner;
ImageSaver* imageSaver;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);



#endif /* main_h */
