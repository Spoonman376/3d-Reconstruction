/*
 * This file is part of the OpenKinect Project. http://www.openkinect.org
 *
 * Copyright (c) 2014 individual OpenKinect contributors. See the CONTRIB file
 * for details.
 *
 * This code is licensed to you under the terms of the Apache License, version
 * 2.0, or, at your option, the terms of the GNU General Public License,
 * version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
 * or the following URLs:
 * http://www.apache.org/licenses/LICENSE-2.0
 * http://www.gnu.org/licenses/gpl-2.0.txt
 *
 * If you redistribute this file in source form, modified or unmodified, you
 * may:
 *   1) Leave this header intact and distribute it under the same terms,
 *      accompanying it with the APACHE20 and GPL20 files, or
 *   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
 *   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
 * In all cases you must keep the copyright notice intact and include a copy
 * of the CONTRIB file.
 *
 * Binary distributions must follow the binary distribution requirements of
 * either License.
 */

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

  KinectDepthParams() {
    fx = 364.630493;    fy = 364.630493;
    cx = 260.71759;     cy = 206.957504;
    k1 = 0.0940838233;  k2 = -0.268225789;  k3 = 0.0886202827;
    p1 = 0;             p2 = 0;
  }
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

  KinectColourParams() {
    fx = 1081.37207;            fy = 1081.37207;
    cx = 959.5;                 cy = 539.5;
    shift_d = 864;              shift_m = 52;

    mx_x3y0 = 0.000526354997;   mx_x2y1 = -3.83278311e-05;  mx_x1y2 = 0.000375511911;   mx_x3y0 = 0.000526354997;
    mx_x2y0 = 5.37541891e-05;   mx_x1y1 = 7.39642492e-05;   mx_x1y0 = 0.6373806;
    mx_x1y0 = 0.6373806;        mx_x0y1 = 0.01219822;
    mx_x0y0 = 0.157183394;

    my_x3y0 = -1.99896003e-05;  my_x2y1 = 0.000468397688;   my_x1y2 = -4.46143204e-05;  my_x0y3 = 0.000756927882;
    my_x2y0 = 7.73532975e-06,   my_x1y1 = -0.000127991996;  my_x0y2 = -7.76459783e-05;
    my_x1y0 = -0.0120614301;    my_x0y1 = 0.636487603;
    my_x0y0 = 0.0174007304;
  }
};

class ImageRegistrationKinect
{
public:
  ImageRegistrationKinect(KinectDepthParams dParams = KinectDepthParams(), KinectColourParams cParams = KinectColourParams());

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
