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
/*
    while (true)
    {
        Mat imgOriginal;

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video

         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }
*/
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
Mat originalImg(img);
imwrite("obrN10.jpg", originalImg);

Mat imgHSV;

cvtColor(originalImg, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
/*
Mat imgThresholded;

inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

//morphological opening (remove small objects from the foreground)
erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

//morphological closing (fill small holes in the foreground)
dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
*/
/*
imshow("Thresholded Image", imgThresholded); //show the thresholded image
imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
       {
            cout << "esc key is pressed by user" << endl;
            break;
       }
    }

    ;

  if ( !cap.isOpened() )  // if not success, exit program
  {
       cout << "Cannot open the web cam" << endl;
       return -1;
  }
  */
cameraProxy.releaseImage(subscriberID);

// Unsubscribe the V.M.
cameraProxy.unsubscribe(subscriberID);
   return 0;

}
