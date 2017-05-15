//
//

#ifndef ImageSaver_h
#define ImageSaver_h

#include <stdio.h>
#include <openni/OpenNI.h>
#include <opencv2/opencv.hpp>
#include <boost/filesystem.hpp>
#include <string>

using namespace openni;
using namespace cv;
using namespace std;

class ImageSaver
{
protected:
  int imageCount;
  vector<const string> imagePaths;

public:
  ImageSaver();
  ~ImageSaver();

  void setUpDirectories();

  void saveImages(vector<VideoFrameRef*>);
};


#endif /* ImageSaver_h */
