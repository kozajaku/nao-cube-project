/*
 * Copyright (c) 2012-2014 Aldebaran Robotics. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the COPYING file.
 */
#include <iostream>
//#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <alproxies/alvideodeviceproxy.h>
#include <alvision/alvisiondefinitions.h>
#include "houghmain.cpp"
#include <stdio.h>

using namespace cv;
using namespace std;

 int main( int argc, char** argv )
 {
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
for (int i = 0; i < 5; i++){
    // Retrieve the current image.
    AL::ALValue results;
    results = cameraProxy.getImageRemote(subscriberID);
    const unsigned char* imageData =  static_cast<const unsigned char*>(results[6].GetBinary());

    if (imageData == NULL) {
        std::cerr << "Could not retrieve current image." << std::endl;
        return 0;
    }
    IplImage* img = cvCreateImage(cvSize(640, 480), IPL_DEPTH_8U, 3);
    memcpy(img->imageData, imageData, 640*480*3);
    //cvShowImage("xxx", img);
    originalImg = Mat(img);


    Mat imgRGB;

    cvtColor(originalImg, imgRGB, COLOR_BGR2RGB); //Convert the captured frame from BGR to RGB


    resCount = findOutDieCount(imgRGB);
    if (resCount != -1){
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

}
