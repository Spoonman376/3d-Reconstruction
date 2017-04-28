//
//

#ifndef Texture_h
#define Texture_h

#include <stdio.h>
#include "Utilities.h"

class Texture
{
public:
  
  // OpenGL names for array buffer objects, vertex array object
  GLuint textureName;
  GLenum textureTarget;

  // dimensions of the image stored in this texture
  GLuint width, height;

  Texture(GLenum target = GL_TEXTURE_RECTANGLE);
  ~Texture();
  
  void updateTexture(VideoFrameRef *frame);
};



#endif /* Texture_h */
