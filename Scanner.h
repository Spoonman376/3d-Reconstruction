//
//

#ifndef Scanner_h
#define Scanner_h

#include <iostream>
#include <stdio.h>
#include "OpenNI.h"
#include <vector>
#include <queue>
#include <mutex>

using namespace std;
using namespace openni;

class Scanner
{
protected:
  Device* device;
  
  VideoStream infraRedStream;
  VideoStream depthStream;
  VideoStream colourStream;

  bool scanning;
  Scanner(Device *d);

public:
  queue<VideoFrameRef*> iRFrames;
  queue<VideoFrameRef*> depthFrames;
  queue<VideoFrameRef*> colourFrames;

  mutex lock;

  static Scanner* getScanner(const char* uri);
  ~Scanner();
  

  
  void startScanning();
  void stopScanning();

  bool isScanning();
  
  void getFrame(SensorType type, VideoFrameRef* frame);
};


#endif /* Scanner_h */


