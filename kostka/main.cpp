/*#include <iostream>
#include <opencv2/opencv.hpp>
#include <alproxies/alvideodeviceproxy.h>
#include <alvision/alvisiondefinitions.h>
#include <alcommon/alproxy.h>
#include "recognition.hpp"
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
    //---
    const std::string pIp = "10.10.48.252";

    // Proxy to ALVideoDevice.
    AL::ALVideoDeviceProxy cameraProxy(pIp);

    // Subscribe a Vision Module to ALVideoDevice, starting the
    // frame grabber if it was not started before.
    std::string subscriberID = "subscriberID1";

    int fps = 5;
    // The subscriberID can be altered if other instances are already running
    subscriberID = cameraProxy.subscribe(subscriberID, AL::kVGA, AL::kBGRColorSpace, fps);
    cameraProxy.setActiveCamera(subscriberID, 1); //1 - spodni kamera

    //----
    int resCount;
    Mat originalImg;
    for (int i = 0; i < 5; i++) {
        // Retrieve the current image.
        AL::ALValue results;
        results = cameraProxy.getImageRemote(subscriberID);
        const unsigned char* imageData = static_cast<const unsigned char*> (results[6].GetBinary());

        if (imageData == NULL) {
            std::cerr << "Could not retrieve current image." << std::endl;
            return 0;
        }
        IplImage* img = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
        memcpy(img->imageData, imageData, 640 * 480 * 3);
        //cvShowImage("xxx", img);
        originalImg = Mat(img);


        Mat imgRGB;

        cvtColor(originalImg, imgRGB, COLOR_BGR2RGB); //Convert the captured frame from BGR to RGB


        resCount = findOutDieCount(imgRGB);
        if (resCount != -1) {
            break;
        }
        printf("next try\n");
    }
    imwrite("obrN10.jpg", originalImg);

    printf("Padlo: %d\n", resCount);

    cameraProxy.releaseImage(subscriberID);

    // Unsubscribe the V.M.
    cameraProxy.unsubscribe(subscriberID);
    return 0;

}*/


#include <iostream>
#include <stdlib.h>
#include <qi/os.hpp>

#include "dieModule.hpp"

#include <alcommon/almodule.h>
#include <alcommon/albroker.h>
#include <alcommon/albrokermanager.h>


int main(int argc, char* argv[])
{
  // We will try to connect our broker to a running NAOqi
  int pport = 9559;
  std::string pip = "127.0.0.1";

  // command line parse option
  // check the number of arguments
  if (argc != 1 && argc != 3 && argc != 5)
  {
    std::cerr << "Wrong number of arguments!" << std::endl;
    std::cerr << "Usage: mymodule [--pip robot_ip] [--pport port]" << std::endl;
    exit(2);
  }

  // if there is only one argument it should be IP or PORT
  if (argc == 3)
  {
    if (std::string(argv[1]) == "--pip")
      pip = argv[2];
    else if (std::string(argv[1]) == "--pport")
      pport = atoi(argv[2]);
    else
    {
      std::cerr << "Wrong number of arguments!" << std::endl;
      std::cerr << "Usage: mymodule [--pip robot_ip] [--pport port]" << std::endl;
      exit(2);
    }
  }

  // Specified IP or PORT for the connection
  if (argc == 5)
  {
    if (std::string(argv[1]) == "--pport"
        && std::string(argv[3]) == "--pip")
    {
      pport = atoi(argv[2]);
      pip = argv[4];
    }
    else if (std::string(argv[3]) == "--pport"
             && std::string(argv[1]) == "--pip")
    {
      pport = atoi(argv[4]);
      pip = argv[2];
    }
    else
    {
      std::cerr << "Wrong number of arguments!" << std::endl;
      std::cerr << "Usage: mymodule [--pip robot_ip] [--pport port]" << std::endl;
      exit(2);
    }
  }

  // Need this to for SOAP serialization of floats to work
  setlocale(LC_NUMERIC, "C");

  // A broker needs a name, an IP and a port:
  const std::string brokerName = "mybroker";
  // FIXME: would be a good idea to look for a free port first
  int brokerPort = 54000;
  // listen port of the broker (here an anything)
  const std::string brokerIp = "0.0.0.0";


  // Create your own broker
  boost::shared_ptr<AL::ALBroker> broker;
  try
  {
    broker = AL::ALBroker::createBroker(
        brokerName,
        brokerIp,
        brokerPort,
        pip,
        pport,
        0    // you can pass various options for the broker creation,
             // but default is fine
      );
  }
  catch(...)
  {
    std::cerr << "Fail to connect broker to: "
              << pip
              << ":"
              << pport
              << std::endl;

    AL::ALBrokerManager::getInstance()->killAllBroker();
    AL::ALBrokerManager::kill();

    return 1;
  }

  // Deal with ALBrokerManager singleton (add your borker into NAOqi)
  AL::ALBrokerManager::setInstance(broker->fBrokerManager.lock());
  AL::ALBrokerManager::getInstance()->addBroker(broker);

  // Now it's time to load your module with
  // AL::ALModule::createModule<your_module>(<broker_create>, <your_module>);
  AL::ALModule::createModule<DieModule>(broker, "DieModule");

  while (true)
    qi::os::sleep(1);

  return 0;
}

