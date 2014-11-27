#ifndef RECOGNITION
#define RECOGNITION

#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <string>

using namespace std;
using namespace cv;

Mat hsvImg;
Mat hsvThresholded;
int iLowH = 0;
int iHighH = 179;

int iLowS = 0;
int iHighS = 85;

int iLowV = 40;
int iHighV = 255;

int bLowH = 30;
int bHighH = 90;
int bLowS = 110;
int bHighS = 255;
int bLowV = 0;
int bHighV = 255;

int pLowH = 150;
int pHighH = 30;
int pLowS = 100;
int pHighS = 255;
int pLowV = 40;
int pHighV = 255;

int pointLowWidth = 5;
int pointHighWidth = 20;
int pointLowHeight = 8;
int pointHighHeight = 22;

int dieLowWidth = 45;
int dieHighWidth = 70;
int dieLowHeight = 50;
int dieHighHeight = 90;


int findOutDieCount(Mat &);

bool isInsideEllipse(RotatedRect, RotatedRect);

void hsvThreshold();

RotatedRect countBetterFit(RotatedRect);



#endif //RECOGNITION
