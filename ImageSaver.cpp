//
//

#include "ImageSaver.h"

ImageSaver::ImageSaver()
{
  shouldClose = false;
  // create a folder to hold the image files

}


ImageSaver::~ImageSaver()
{


}


void ImageSaver::saveImages()
{
  while(!shouldClose) // or there are still images to write
  {
    if (!scanner->iRFrames.empty())
    {
      // write the first frame in the queue to a file
    }
  }
}


void ImageSaver::close()
{
  shouldClose = true;
}
