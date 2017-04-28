//
//

#ifndef View_h
#define View_h

#include <stdio.h>
#include "Texture.h"

class View
{
public:

  Texture texture;

  View();
  ~View();


  void Render();

};


#endif /* View_h */
