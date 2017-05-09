//
//

#ifndef Scanner_h
#define Scanner_h

#include <iostream>
#include <stdio.h>
#include <openni/OpenNI.h>
#include <vector>
#include <queue>
#include <mutex>

using namespace std;
using namespace openni;

class Scanner
{
protected:
  Device device;
  
  VideoStream infraRedStream;
  VideoStream depthStream;
  VideoStream colourStream;

  bool scanning;

public:
  queue<VideoFrameRef*> iRFrames;
  queue<VideoFrameRef*> depthFrames;
  queue<VideoFrameRef*> colourFrames;

  mutex lock;

  Scanner(const char* uri);
  ~Scanner();
  

  
  void startScanning();
  void stopScanning();

  bool isScanning();
  
  void getFrame(SensorType type, VideoFrameRef* frame);
};


#endif /* Scanner_h */


