//
//

#ifndef Scanner_h
#define Scanner_h

#include <iostream>
#include <stdio.h>
#ifdef CMAKE
	#include <OpenNI.h>
#else
	#include <openni2/OpenNI.h>
#endif
#include <vector>
#include <queue>
#include <mutex>

using namespace std;
using namespace openni;

class Scanner
{
protected:
  Device device;


public:
  Scanner(const char* uri);
  ~Scanner();

  vector<VideoStream*> streams;

  void startScanning();
  void stopScanning();

  void getFrame(SensorType type, VideoFrameRef* frame);
  void getFrames(vector<VideoFrameRef*> &frames);
};


#endif /* Scanner_h */


