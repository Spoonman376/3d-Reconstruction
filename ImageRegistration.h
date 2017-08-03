//
//

#ifndef ImageRegistration_h
#define ImageRegistration_h

#include <stdio.h>
#include <opencv2/opencv.hpp>

// these seem to be hardcoded in the original SDK
static const float depth_q = 0.01;
static const float color_q = 0.002199;

struct KinectDepthParams
{
  float fx; ///< Focal length x (pixel)
  float fy; ///< Focal length y (pixel)
  float cx; ///< Principal point x (pixel)
  float cy; ///< Principal point y (pixel)
  float k1; ///< Radial distortion coefficient, 1st-order
  float k2; ///< Radial distortion coefficient, 2nd-order
  float k3; ///< Radial distortion coefficient, 3rd-order
  float p1; ///< Tangential distortion coefficient
  float p2; ///< Tangential distortion coefficient
};

struct KinectColourParams
{
  /** @name Intrinsic parameters */
  ///@{
  float fx; ///< Focal length x (pixel)
  float fy; ///< Focal length y (pixel)
  float cx; ///< Principal point x (pixel)
  float cy; ///< Principal point y (pixel)
  ///@}

  /** @name Extrinsic parameters
   * These parameters are used in [a formula](https://github.com/OpenKinect/libfreenect2/issues/41#issuecomment-72022111) to map coordinates in the
   * depth camera to the color camera.
   *
   * They cannot be used for matrix transformation.
   */
  ///@{
  float shift_d, shift_m;

  float mx_x3y0; // xxx
  float mx_x0y3; // yyy
  float mx_x2y1; // xxy
  float mx_x1y2; // yyx
  float mx_x2y0; // xx
  float mx_x0y2; // yy
  float mx_x1y1; // xy
  float mx_x1y0; // x
  float mx_x0y1; // y
  float mx_x0y0; // 1

  float my_x3y0; // xxx
  float my_x0y3; // yyy
  float my_x2y1; // xxy
  float my_x1y2; // yyx
  float my_x2y0; // xx
  float my_x0y2; // yy
  float my_x1y1; // xy
  float my_x1y0; // x
  float my_x0y1; // y
  float my_x0y0; // 1
  ///@}
};


class ImageRegistrationKinect
{
public:
  ImageRegistrationKinect();
  ImageRegistrationKinect(KinectDepthParams dParams, KinectColourParams cParams);

  void distort(int mx, int my, float& dx, float& dy) const;
  void depth_to_color(float mx, float my, float& rx, float& ry) const;
  void apply(const cv::Mat *rgb, const cv::Mat *depth, cv::Mat *undistorted, cv::Mat *registered, const bool enable_filter = true, cv::Mat *bigdepth = 0, int *color_depth_map = 0) const;

private:
  KinectDepthParams depth;    ///< Depth camera parameters.
  KinectColourParams color; ///< Color camera parameters.

  int distort_map[512 * 424];
  float depth_to_color_map_x[512 * 424];
  float depth_to_color_map_y[512 * 424];
  int depth_to_color_map_yi[512 * 424];

  const int filter_width_half = 2;
  const int filter_height_half = 1;
  const float filter_tolerance = 0.01;
};


#endif /* ImageRegistration_h */
