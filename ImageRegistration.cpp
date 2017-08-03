//
//

#include "ImageRegistration.h"


ImageRegistrationKinect::ImageRegistrationKinect(KinectDepthParams depth_p, KinectColourParams rgb_p)
{
  depth = depth_p;
  color = rgb_p;

  float mx, my;
  int ix, iy, index;
  float rx, ry;
  int *map_dist = distort_map;
  float *map_x = depth_to_color_map_x;
  float *map_y = depth_to_color_map_y;
  int *map_yi = depth_to_color_map_yi;

  for (int y = 0; y < 424; y++) {
    for (int x = 0; x < 512; x++) {
      // compute the dirstored coordinate for current pixel
      distort(x, y, mx, my);
      // rounding the values and check if the pixel is inside the image
      ix = (int)(mx + 0.5f);
      iy = (int)(my + 0.5f);
      if(ix < 0 || ix >= 512 || iy < 0 || iy >= 424)
        index = -1;
      else
        // computing the index from the coordianted for faster access to the data
        index = iy * 512 + ix;
      *map_dist++ = index;

      // compute the depth to color mapping entries for the current pixel
      depth_to_color(x,y,rx,ry);
      *map_x++ = rx;
      *map_y++ = ry;
      // compute the y offset to minimize later computations
      *map_yi++ = (int)(ry + 0.5f);
    }
  }
}


void ImageRegistrationKinect::distort(int mx, int my, float& x, float& y) const
{
  float dx = ((float)mx - depth.cx) / depth.fx;
  float dy = ((float)my - depth.cy) / depth.fy;
  float dx2 = dx * dx;
  float dy2 = dy * dy;
  float r2 = dx2 + dy2;
  float dxdy2 = 2 * dx * dy;
  float kr = 1 + ((depth.k3 * r2 + depth.k2) * r2 + depth.k1) * r2;
  x = depth.fx * (dx * kr + depth.p2 * (r2 + 2 * dx2) + depth.p1 * dxdy2) + depth.cx;
  y = depth.fy * (dy * kr + depth.p1 * (r2 + 2 * dy2) + depth.p2 * dxdy2) + depth.cy;
}

void ImageRegistrationKinect::depth_to_color(float mx, float my, float& rx, float& ry) const
{
  mx = (mx - depth.cx) * depth_q;
  my = (my - depth.cy) * depth_q;

  float wx =
    (mx * mx * mx * color.mx_x3y0) + (my * my * my * color.mx_x0y3) +
    (mx * mx * my * color.mx_x2y1) + (my * my * mx * color.mx_x1y2) +
    (mx * mx * color.mx_x2y0) + (my * my * color.mx_x0y2) + (mx * my * color.mx_x1y1) +
    (mx * color.mx_x1y0) + (my * color.mx_x0y1) + (color.mx_x0y0);

  float wy =
    (mx * mx * mx * color.my_x3y0) + (my * my * my * color.my_x0y3) +
    (mx * mx * my * color.my_x2y1) + (my * my * mx * color.my_x1y2) +
    (mx * mx * color.my_x2y0) + (my * my * color.my_x0y2) + (mx * my * color.my_x1y1) +
    (mx * color.my_x1y0) + (my * color.my_x0y1) + (color.my_x0y0);

  rx = (wx / (color.fx * color_q)) - (color.shift_m / color.shift_d);
  ry = (wy / color_q) + color.cy;
}

void ImageRegistrationKinect::apply(const cv::Mat *rgb, const cv::Mat *depth, cv::Mat *undistorted, cv::Mat *registered, const bool enable_filter, cv::Mat *bigdepth, int *color_depth_map) const
{
  // Check if all frames are valid and have the correct size
  if (!rgb || !depth || !undistorted || !registered ||
      rgb->cols != 1920 || rgb->rows != 1080 || //rgb->bytes_per_pixel != 4 ||
      depth->cols != 512 || depth->rows != 424 || //depth->bytes_per_pixel != 4 ||
      undistorted->cols != 512 || undistorted->rows != 424 || //undistorted->bytes_per_pixel != 4 ||
      registered->cols != 512 || registered->rows != 424 ) //|| registered->bytes_per_pixel != 4)
    return;

  const float *depth_data = (float*)depth->data;
  const unsigned int *rgb_data = (unsigned int*)rgb->data;
  float *undistorted_data = (float*)undistorted->data;
  unsigned int *registered_data = (unsigned int*)registered->data;
  const int *map_dist = distort_map;
  const float *map_x = depth_to_color_map_x;
  const int *map_yi = depth_to_color_map_yi;

  const int size_depth = 512 * 424;
  const int size_color = 1920 * 1080;
  const float color_cx = color.cx + 0.5f; // 0.5f added for later rounding

  // size of filter map with a border of filter_height_half on top and bottom so that no check for borders is needed.
  // since the color image is wide angle no border to the sides is needed.
  const int size_filter_map = size_color + 1920 * filter_height_half * 2;
  // offset to the important data
  const int offset_filter_map = 1920 * filter_height_half;

  // map for storing the min z values used for each color pixel
  float *filter_map = NULL;
  // pointer to the beginning of the important data
  float *p_filter_map = NULL;

  // map for storing the color offset for each depth pixel
  int *depth_to_c_off = color_depth_map ? color_depth_map : new int[size_depth];
  int *map_c_off = depth_to_c_off;

  // initializing the depth_map with values outside of the Kinect2 range
  if(enable_filter){
    filter_map = bigdepth ? (float*)bigdepth->data : new float[size_filter_map];
    p_filter_map = filter_map + offset_filter_map;

    for(float *it = filter_map, *end = filter_map + size_filter_map; it != end; ++it){
      *it = std::numeric_limits<float>::infinity();
    }
  }

  /* Fix depth distortion, and compute pixel to use from 'rgb' based on depth measurement,
   * stored as x/y offset in the rgb data.
   */

  // iterating over all pixels from undistorted depth and registered color image
  // the four maps have the same structure as the images, so their pointers are increased each iteration as well
  for(int i = 0; i < size_depth; ++i, ++undistorted_data, ++map_dist, ++map_x, ++map_yi, ++map_c_off){
    // getting index of distorted depth pixel
    const int index = *map_dist;

    // check if distorted depth pixel is outside of the depth image
    if(index < 0){
      *map_c_off = -1;
      *undistorted_data = 0;
      continue;
    }

    // getting depth value for current pixel
    const float z = depth_data[index];
    *undistorted_data = z;

    // checking for invalid depth value
    if(z <= 0.0f){
      *map_c_off = -1;
      continue;
    }

    // calculating x offset for rgb image based on depth value
    const float rx = (*map_x + (color.shift_m / z)) * color.fx + color_cx;
    const int cx = rx; // same as round for positive numbers (0.5f was already added to color_cx)
    // getting y offset for depth image
    const int cy = *map_yi;
    // combining offsets
    const int c_off = cx + cy * 1920;

    // check if c_off is outside of rgb image
    // checking rx/cx is not needed because the color image is much wider then the depth image
    if(c_off < 0 || c_off >= size_color){
      *map_c_off = -1;
      continue;
    }

    // saving the offset for later
    *map_c_off = c_off;

    if(enable_filter){
      // setting a window around the filter map pixel corresponding to the color pixel with the current z value
      int yi = (cy - filter_height_half) * 1920 + cx - filter_width_half; // index of first pixel to set
      for(int r = -filter_height_half; r <= filter_height_half; ++r, yi += 1920) // index increased by a full row each iteration
      {
        float *it = p_filter_map + yi;
        for(int c = -filter_width_half; c <= filter_width_half; ++c, ++it)
        {
          // only set if the current z is smaller
          if(z < *it)
            *it = z;
        }
      }
    }
  }

  /* Construct 'registered' image. */

  // reseting the pointers to the beginning
  map_c_off = depth_to_c_off;
  undistorted_data = (float*)undistorted->data;

  /* Filter drops duplicate pixels due to aspect of two cameras. */
  if(enable_filter){
    // run through all registered color pixels and set them based on filter results
    for(int i = 0; i < size_depth; ++i, ++map_c_off, ++undistorted_data, ++registered_data){
      const int c_off = *map_c_off;

      // check if offset is out of image
      if(c_off < 0){
        *registered_data = 0;
        continue;
      }

      const float min_z = p_filter_map[c_off];
      const float z = *undistorted_data;

      // check for allowed depth noise
      *registered_data = (z - min_z) / z > filter_tolerance ? 0 : *(rgb_data + c_off);
    }

    if (!bigdepth) delete[] filter_map;
  }
  else
  {
    // run through all registered color pixels and set them based on c_off
    for(int i = 0; i < size_depth; ++i, ++map_c_off, ++registered_data){
      const int c_off = *map_c_off;

      // check if offset is out of image
      *registered_data = c_off < 0 ? 0 : *(rgb_data + c_off);
    }
  }
  if (!color_depth_map) delete[] depth_to_c_off;
}






