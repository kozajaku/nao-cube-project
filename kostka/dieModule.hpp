#ifndef MY_MODULE_H
#define MY_MODULE_H

#include <iostream>
#include <alcommon/almodule.h>
#include "recognition.hpp"
//#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <alproxies/alvideodeviceproxy.h>
#include <alvision/alvisiondefinitions.h>

namespace AL
{
  // This is a forward declaration of AL:ALBroker which
  // avoids including <alcommon/albroker.h> in this header
  class ALBroker;
}

/**
 * This class inherits AL::ALModule. This allows it to bind methods
 * and be run as a remote executable within NAOqi
 */
class DieModule : public AL::ALModule
{
public:
  DieModule(boost::shared_ptr<AL::ALBroker> broker,
           const std::string &name);

  virtual ~DieModule();

  /**
   * Overloading ALModule::init().
   * This is called right after the module has been loaded
   */
  virtual void init();

  // After that you may add all your bind method.

  int findOutThrownCount();
};
#endif // MY_MODULE_H
