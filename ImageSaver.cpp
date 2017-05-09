//
//

#include "ImageSaver.h"

using namespace boost::filesystem;

ImageSaver::ImageSaver(Scanner * s)
{
  shouldClose = false;
  scanner = s;
  // create a folder to hold the image files
}


ImageSaver::~ImageSaver()
{

}

// Ideally this method would run on a separate thread
void ImageSaver::saveImages()
{
  const char irPath[] = "depthframes";
  const char depthPath[] = "irframes";

	path irDir(irPath);
	path depthDir(depthPath);

  if (is_directory(irDir))
    boost::filesystem::remove_all(irDir);

  if (is_directory(depthDir))
    boost::filesystem::remove_all(depthDir);

  create_directory(irDir);
  create_directory(depthDir);

  imageCount = 0;

  lock.lock();
  while(!shouldClose || !scanner->iRFrames.empty()) // or there are still images to write
  {
    // write the first frame in the queue to a file
    if (!scanner->iRFrames.empty())
    {
      scanner->lock.lock();

      VideoFrameRef* frame = scanner->iRFrames.front();
      scanner->iRFrames.pop();

      scanner->lock.unlock();

      int width = frame->getWidth();
      int height = frame->getHeight();
      int size = frame->getDataSize();

      Mat image = Mat(height, width, size / (width * height), (Grayscale16Pixel*)frame->getData());

      string s = "irframes/Image" + to_string(imageCount) + ".jpg";
      imwrite(s, image);

      delete frame;
    }
    else
      shouldClose = true;

    imageCount++;
  }
  lock.unlock();
}


void ImageSaver::close()
{
  shouldClose = true;
  lock.lock();
  lock.unlock();
}
