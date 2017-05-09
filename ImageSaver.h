//
//

#ifndef ImageSaver_h
#define ImageSaver_h

#include <stdio.h>
#include "Scanner.h"
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <string>

using namespace cv;

class ImageSaver
{
protected:
  bool shouldClose;
  int imageCount;

  mutex lock;

public:
  Scanner* scanner;

  ImageSaver(Scanner*);
  ~ImageSaver();

  void saveImages();
  void close();
};


#endif /* ImageSaver_h */
