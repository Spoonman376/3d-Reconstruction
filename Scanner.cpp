//
//

#include "Scanner.h"


Scanner::Scanner(const char* uri)
{
  if(device.open(uri) != STATUS_OK) {
    cout << "No device aviable" << endl;
    return;
  }

  for (int i = 0; i <= 2; ++i)
  {
    streams.push_back(new VideoStream());

    if (device.hasSensor(SensorType(i + 1)))
      if (streams[i]->create(device, SensorType(i + 1)) != STATUS_OK)
        cout << "Problem creating stream " << i << endl;
  }

  device.setDepthColorSyncEnabled(true);
  if (device.isImageRegistrationModeSupported(IMAGE_REGISTRATION_DEPTH_TO_COLOR)) {
    cout << "Enabling Image Registraion" << endl;
    device.setImageRegistrationMode(IMAGE_REGISTRATION_DEPTH_TO_COLOR); 
  }
}

Scanner::~Scanner()
{
  for(VideoStream* stream : streams) {
    stream->destroy();
    delete stream;
  }
  device.close();
}


void Scanner::startScanning()
{
  for(VideoStream* stream : streams)
    if(stream->isValid())
      stream->start();
}


void Scanner::stopScanning()
{
  for(VideoStream* stream : streams)
    if(stream->isValid())
      stream->stop();
}


// Eventually change this to grab all avaliable frames;
void Scanner::getFrame(SensorType type, VideoFrameRef *frame)
{
  Status s;
  s = streams[type - 1]->readFrame(frame);

  
  if (s != STATUS_OK) {
    cout << "Problem occured when attempting to read a frame" << endl;
    return;
  }
}


void Scanner::getFrames(vector<VideoFrameRef*> &frames)
{
  for (VideoStream* stream : streams) {
    VideoFrameRef* frame = nullptr;
    Status s;

    if (stream->isValid()) {
      frame = new VideoFrameRef();
      s = stream->readFrame(frame);
    }

    frames.push_back(frame);
  }
}











