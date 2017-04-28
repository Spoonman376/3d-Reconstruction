//
//

#include "ImageSaver.h"

ImageSaver::ImageSaver(Scanner * s)
{
  shouldClose = false;
  scanner = s;
  // create a folder to hold the image files
}


ImageSaver::~ImageSaver()
{

}


void ImageSaver::saveImages()
{
  lock.lock();
  while(!shouldClose) // or there are still images to write
  {
    if (!scanner->iRFrames.empty())
    {
      // write the first frame in the queue to a file
    }
  }
  lock.unlock();
}


void ImageSaver::close()
{
  shouldClose = true;
  lock.lock();
  lock.unlock();
}
