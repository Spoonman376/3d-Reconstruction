//
//

#include "ImageSaver.h"

using namespace boost::filesystem;

ImageSaver::ImageSaver(Scanner* scanner, bool reg)
{
  registerDepthAndColour = reg;

  string irPath = "irframes";
  string colorPath = "colourframes";
  string depthPath = "depthframes";
  string undistortedPath = "undistortedframes";
  string registeredPath = "registeredframes";

  imagePaths.push_back(irPath);
  imagePaths.push_back(colorPath);
  imagePaths.push_back(depthPath);
  imagePaths.push_back(undistortedPath);
  imagePaths.push_back(registeredPath);

  oniRecorder.create("test.oni");

  if (scanner != nullptr) {
    for (VideoStream* stream : scanner->streams) {
      if (stream->isValid())
        oniRecorder.attach(*stream);
    }
  }

  if (registerDepthAndColour)
    registration = new ImageRegistrationKinect();
}


ImageSaver::~ImageSaver()
{
  oniRecorder.stop();
  oniRecorder.destroy();

  if (registration != nullptr)
    delete registration;
}

// maybe change this to create a new directory rather than delete the current one.
void ImageSaver::setUpDirectories()
{
	path irDir(imagePaths[SENSOR_IR - 1]);
	path colourDir(imagePaths[SENSOR_COLOR - 1]);
  path depthDir(imagePaths[SENSOR_DEPTH - 1]);
  path undistortedDir(imagePaths[3]);
  path registeredDir(imagePaths[4]);

  if (is_directory(irDir))
    boost::filesystem::remove_all(irDir);

  if (is_directory(colourDir))
    boost::filesystem::remove_all(colourDir);

  if (is_directory(depthDir))
    boost::filesystem::remove_all(depthDir);

  if (is_directory(undistortedDir))
    boost::filesystem::remove_all(undistortedDir);

  if (is_directory(registeredDir))
    boost::filesystem::remove_all(registeredDir);

  create_directory(irDir);
  create_directory(colourDir);
  create_directory(depthDir);

  if (registerDepthAndColour) {
    create_directory(undistortedDir);
    create_directory(registeredDir);
  }

  oniRecorder.start();

  imageCount = 0;
}


void ImageSaver::saveImages(vector<VideoFrameRef*> &frames)
{
  cv::Mat depth;
  cv::Mat colour;

  for (int i = 0; i <= 2; ++i) {
    VideoFrameRef* frame = frames[i];
    if (frame == nullptr)
      continue;

    int width = frame->getWidth();
    int height = frame->getHeight();

    Mat image;
    string path = imagePaths[frame->getSensorType() - 1] + "/Image" + to_string(imageCount);

    switch (frame->getSensorType()) {
      case openni::SENSOR_IR:
      image = Mat(height, width, CV_16U, (Grayscale16Pixel*)frame->getData());
      path += ".png";
      break;

      case openni::SENSOR_COLOR:
      image = Mat(height, width, CV_8UC3, (RGB888Pixel*)frame->getData());
      colour = Mat(image);
      cv::cvtColor(image, image, CV_RGB2BGR);
      path += ".jpg";
      break;

      case openni::SENSOR_DEPTH:
      image = Mat(height, width, CV_16U, (DepthPixel*)frame->getData());
      depth = Mat(image);
      path += ".png";
      break;
    }

    imwrite(path, image);
  }


  // Currently hardCoded for the Kinect
  if (registerDepthAndColour) {
    cv::Mat undistorted, registered;
    undistorted.create(depth.rows, depth.cols, depth.type());
    registered.create(depth.rows, depth.cols, colour.type());

    registration->apply(&colour, &depth, &undistorted, &registered);

    string undistortedPath = imagePaths[3] + "/Image" + to_string(imageCount) + ".png";
    string registeredPath = imagePaths[4] + "/Image" + to_string(imageCount) + ".jpg";

    imwrite(undistortedPath, undistorted);
    imwrite(registeredPath, registered);
  }


  imageCount++;
}

