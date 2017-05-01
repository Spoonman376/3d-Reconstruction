//
//

#include "Scanner.h"

Scanner* Scanner::getScanner(const char* uri)
{
  Device* d;
  if (d->open(uri) == STATUS_OK)
    return new Scanner(d);
  else
    return nullptr;
}


Scanner::Scanner(Device* d)
{
  device = d;
  
  // Open the InfraRed stream if availiable
  if (device->hasSensor(SENSOR_IR))
    if (infraRedStream.create(*device, SENSOR_IR) != STATUS_OK)
      cout << "Problem creating the ifra red Stream" << endl;
  
  // Open the depth stream if availiable
  if (device->hasSensor(SENSOR_DEPTH))
    if (depthStream.create(*device, SENSOR_DEPTH) != STATUS_OK)
      cout << "Problem creating the depth Stream" << endl;

  // Open the colour stream if availiable
  if (device->hasSensor(SENSOR_COLOR))
    if (colourStream.create(*device, SENSOR_COLOR) != STATUS_OK)
      cout << "Problem creating the colour Stream" << endl;
  
  scanning = false;
}

Scanner::~Scanner()
{
  infraRedStream.destroy();
  depthStream.destroy();
  colourStream.destroy();
  
  device->close();
  delete device;
}


void Scanner::startScanning()
{
  if(infraRedStream.isValid())
    infraRedStream.start();

  if(depthStream.isValid())
    depthStream.start();

  if(colourStream.isValid())
    colourStream.start();
  
  scanning = true;
}


void Scanner::stopScanning()
{
  if(infraRedStream.isValid())
    infraRedStream.stop();
  
  if(depthStream.isValid())
    depthStream.stop();
  
  if(colourStream.isValid())
    colourStream.stop();

  scanning = false;
}

void Scanner::getFrame(SensorType type, VideoFrameRef *frame)
{
  Status s;
  switch (type) {
    case SENSOR_IR:
      s = infraRedStream.readFrame(frame);
      break;

    case SENSOR_DEPTH:
      s = depthStream.readFrame(frame);
      break;

    case SENSOR_COLOR:
      s = colourStream.readFrame(frame);
      break;

    default:
      break;
  }
  
  if (s != STATUS_OK) {
    cout << "Problem occured when attempting to read a frame" << endl;
    return;
  }
  
  if (scanning) {
    VideoFrameRef *copy = new VideoFrameRef(*frame);
    
    switch (type) {
      case SENSOR_IR:
        iRFrames.push(copy);
        break;
        
      case SENSOR_DEPTH:
        depthFrames.push(copy);
        break;
        
      case SENSOR_COLOR:
        colourFrames.push(copy);
        break;
        
      default:
        break;
    }
  }
  
}


bool Scanner::isScanning()
{
  return scanning;
}

























