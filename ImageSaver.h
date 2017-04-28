//
//

#ifndef ImageSaver_h
#define ImageSaver_h

#include <stdio.h>
#include "Scanner.h"

class ImageSaver
{
protected:
  bool shouldClose;

  mutex lock;

public:
  Scanner* scanner;

  ImageSaver(Scanner*);
  ~ImageSaver();

  void saveImages();
  void close();
};


#endif /* ImageSaver_h */
