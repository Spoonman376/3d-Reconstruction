//
//

#ifndef ImageRegistration_h
#define ImageRegistration_h

#include <stdio.h>




class ImageRegistrationKinect
{

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


  KinectDepthParams depth;    ///< Depth camera parameters.
  KinectColourParams color; ///< Color camera parameters.



  int distort_map[512 * 424];
  float depth_to_color_map_x[512 * 424];
  float depth_to_color_map_y[512 * 424];
  int depth_to_color_map_yi[512 * 424];




};


#endif /* ImageRegistration_h */
