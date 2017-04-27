//
//

#include "Utilities.h"


void QueryGLVersion()
{
  // query opengl version and renderer information
  string version  = reinterpret_cast<const char *>(glGetString(GL_VERSION));
  string glslver  = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));
  string renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
  
  cout << "OpenGL [ " << version << " ] "
  << "with GLSL [ " << glslver << " ] "
  << "on renderer [ " << renderer << " ]" << endl;
}

bool checkGLErrors()
{
  bool error = false;
  for (GLenum flag = glGetError(); flag != GL_NO_ERROR; flag = glGetError())
  {
    cout << "OpenGL ERROR: ";
    switch (flag) {
      case GL_INVALID_ENUM:
        cout << "GL_INVALID_ENUM" << endl; break;
      case GL_INVALID_VALUE:
        cout << "GL_INVALID_VALUE" << endl; break;
      case GL_INVALID_OPERATION:
        cout << "GL_INVALID_OPERATION" << endl; break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << endl; break;
      case GL_OUT_OF_MEMORY:
        cout << "GL_OUT_OF_MEMORY" << endl; break;
      default:
        cout << "[unknown error code]" << endl;
    }
    error = true;
  }
  return error;
}


bool initalizeOpenNI()
{
  if (OpenNI::initialize() != STATUS_OK) {
    cout << "Failed to Initialize OpenNI" << endl;
    cout << OpenNI::getExtendedError() << endl;
    return false;
  }
  return true;
}

void closeOpenNI()
{
  OpenNI::shutdown();
}





