//
//

#include "Texture.h"

Texture::Texture(GLenum target)
{
  textureTarget = target;
  
  // create a texture name to associate our image data with
  glGenTextures(1, &textureName);
  // bind the texture GL_TEXTURE_RECTANGLE as a "rectangle" to access using image pixel coordinates
  glBindTexture(textureTarget, textureName);
  
  // send image pixel data to OpenGL texture memory
//  glTexImage2D(textureTarget, 0, GL_RGB, width, height, 0, GL_BGRA, channelDataType, pixels);
  
  // unbind this texture
  glBindTexture(textureTarget, 0);
  
  checkGLErrors();
}

Texture::~Texture()
{
  if (textureName != 0)
    glDeleteTextures(1, &textureName);
}

void Texture::updateTexture(VideoFrameRef *frame)
{
  width = frame->getWidth();
  height = frame->getHeight();
  
  glBindTexture(textureTarget, textureName);
  
  // send image pixel data to OpenGL texture memory
  glTexImage2D(textureTarget, 0, GL_RGB, width, height, 0, GL_LUMINANCE, GL_UNSIGNED_SHORT, frame->getData());
  
  // unbind this texture
  glBindTexture(textureTarget, 0);

}
