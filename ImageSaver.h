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

using namespace openni;
using namespace cv;
using namespace std;

class ImageSaver
{
protected:
  int imageCount;
  vector<string> imagePaths;
  Recorder oniRecorder;

public:
  ImageSaver(Scanner*);
  ~ImageSaver();

  void setUpDirectories();

  void saveImages(vector<VideoFrameRef*>);
};


#endif /* ImageSaver_h */
