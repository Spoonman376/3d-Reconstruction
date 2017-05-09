//
//

#include "main.h"

int main(int argc, const char * argv[])
{
  // Required before any other OpenNI operations occur
  initalizeOpenNI();
  
  // initialize the GLFW windowing system
  if (!glfwInit()) {
    cout << "ERROR: GLFW failed to initilize, TERMINATING" << endl;
    return -1;
  }
  
  // attempt to create a window with an OpenGL 4.1 core profile context
  GLFWwindow *window = 0;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  window = glfwCreateWindow(1024, 768, "Scanner Test", 0, 0);
  if (!window) {
    cout << "Program failed to create GLFW window, TERMINATING" << endl;
    glfwTerminate();
    return -1;
  }
  
  glfwSetKeyCallback(window, keyCallback);


  scanner = new Scanner(ANY_DEVICE);
  imageSaver = new ImageSaver(scanner);


  // Set up a few view/geometries and create the coresponding texture items
  scanner->startScanning();

  while (!glfwWindowShouldClose(window))
  {
    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClearDepth(MAXFLOAT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Update the textures with the new frames
    if (scanner->isScanning()) {
      VideoFrameRef *frame = new VideoFrameRef();
      scanner->getFrame(SENSOR_IR, frame);
      Grayscale16Pixel *data = (Grayscale16Pixel*)frame->getData();
      delete frame;
    }

    // Render

    glfwSwapBuffers(window);

    // sleep until next event before drawing again
    glfwPollEvents();
  }

  scanner->stopScanning();


  imageSaver->saveImages();
  imageSaver->close();
  delete imageSaver;
  delete scanner;

  glfwDestroyWindow(window);
  glfwTerminate();
  
  closeOpenNI();
  return 0;
}


void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (action == GLFW_RELEASE)
    return;
  
  switch (key) {
    case GLFW_KEY_ESCAPE :
      glfwSetWindowShouldClose(window, GL_TRUE);
      break;
      
    case GLFW_KEY_SPACE:
      scanner->isScanning() ? scanner->stopScanning() : scanner->startScanning();
      break;
      
    default:
      break;
  }
}


