//
//

#ifndef ImageSaver_h
#define ImageSaver_h

#include <stdio.h>
#include <openni2/OpenNI.h>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include "Scanner.h"
#include "ImageRegistration.h"

using namespace openni;
using namespace cv;
using namespace std;

class ImageSaver
{
protected:
  int imageCount;

  bool registerDepthAndColour = false;
  ImageRegistrationKinect* registration;

  vector<string> imagePaths;
  Recorder oniRecorder;

public:
  ImageSaver(Scanner*, bool reg = false);
  ~ImageSaver();

  void setUpDirectories();

  void saveImages(vector<VideoFrameRef*> &frames);
};


#endif /* ImageSaver_h */
