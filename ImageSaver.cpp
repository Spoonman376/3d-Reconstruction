//
//

#include "ImageSaver.h"

using namespace boost::filesystem;

ImageSaver::ImageSaver(Scanner* scanner)
{
  const string irPath = "irframes";
  const string colorPath = "colourframes";
  const string depthPath = "depthframes";

  imagePaths.push_back(irPath);
  imagePaths.push_back(colorPath);
  imagePaths.push_back(depthPath);

  oniRecorder.create("test.oni");

  if (scanner != nullptr) {
    for (VideoStream* stream : scanner->streams) {
      if (stream->isValid())
        oniRecorder.attach(*stream);
    }
  }
}


ImageSaver::~ImageSaver()
{
  oniRecorder.stop();
  oniRecorder.destroy();
}

// maybe change this to create a new directory rather than delete the current one.
void ImageSaver::setUpDirectories()
{
	path irDir(imagePaths[SENSOR_IR - 1]);
	path colourDir(imagePaths[SENSOR_COLOR - 1]);
  path depthDir(imagePaths[SENSOR_DEPTH - 1]);

  if (is_directory(irDir))
    boost::filesystem::remove_all(irDir);

  if (is_directory(colourDir))
    boost::filesystem::remove_all(colourDir);

  if (is_directory(depthDir))
    boost::filesystem::remove_all(depthDir);

  create_directory(irDir);
  create_directory(colourDir);
  create_directory(depthDir);

  oniRecorder.start();

  imageCount = 0;
}


void ImageSaver::saveImages(vector<VideoFrameRef*> frames)
{
  for(int i = 0; i <= 2; ++i)
  {
    VideoFrameRef* frame = frames[i];
    if (frame == nullptr)
      continue;

    int width = frame->getWidth();
    int height = frame->getHeight();
    int size = frame->getDataSize();

    Mat image;
    string path = imagePaths[frame->getSensorType() - 1] + "/Image" + to_string(imageCount) + ".jpg";;

    switch (frame->getSensorType()) {
      case openni::SENSOR_IR:
      image = Mat(height, width, size / (width * height), (Grayscale16Pixel*)frame->getData());
      break;

      case openni::SENSOR_COLOR:
      image = Mat(height, width, size / (width * height), (RGB888Pixel*)frame->getData());
      break;

      case openni::SENSOR_DEPTH:
      image = Mat(height, width, size / (width * height), (DepthPixel*)frame->getData());
      break;
    }

    imwrite(path, image);

    delete frame;
  }

  imageCount++;
}

