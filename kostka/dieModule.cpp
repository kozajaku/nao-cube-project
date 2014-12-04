#include "dieModule.hpp"

//#include <iostream>
//#include <alcommon/albroker.h>

DieModule::DieModule(boost::shared_ptr<AL::ALBroker> broker,
                   const std::string& name)
  : AL::ALModule(broker, name)
{
  // Describe the module here. This will appear on the webpage
  setModuleDescription("Die count recognition module.");

//  /**
//   * Define callable methods with their descriptions:
//   * This makes the method available to other cpp modules
//   * and to python.
//   * The name given will be the one visible from outside the module.
//   * This method has no parameters or return value to describe
//   * functionName(<method_name>, <class_name>, <method_description>);
//   * BIND_METHOD(<method_reference>);
//   */
//  functionName("printHello", getName(), "Print hello to the world");
//  BIND_METHOD(MyModule::printHello);
//
//  /**
//   * addParam(<attribut_name>, <attribut_descrption>);
//   * This enables to document the parameters of the method.
//   * It is not compulsory to write this line.
//   */
//  functionName("printWord", getName(), "Print a given word.");
//  addParam("word", "The word to be print.");
//  BIND_METHOD(MyModule::printWord);
//
//  /**
//   * setReturn(<return_name>, <return_description>);
//   * This enables to document the return of the method.
//   * It is not compulsory to write this line.
//   */
//  functionName("returnTrue", getName(), "Just return true");
//  setReturn("boolean", "return true");
//  BIND_METHOD(MyModule::returnTrue);
  
  functionName("findOutThrownCount", getName(), "Finds out thrown count on die");
  setReturn("int", "thrown count or -1 if failed");
  BIND_METHOD(DieModule::findOutThrownCount);
  // If you had other methods, you could bind them here...
  /**
   * Bound methods can only take const ref arguments of basic types,
   * or AL::ALValue or return basic types or an AL::ALValue.
   */
}

DieModule::~DieModule()
{
}

void DieModule::init()
{
  /**
   * Init is called just after construction.
   * Do something or not
   */
}


int DieModule::findOutThrownCount(){
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
    }
    //imwrite("obrN10.jpg", originalImg);

    cout << "Thrown: " <<  resCount << endl;

    cameraProxy.releaseImage(subscriberID);

    // Unsubscribe the V.M.
    cameraProxy.unsubscribe(subscriberID);
}
