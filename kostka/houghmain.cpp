#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>
#include <string>

using namespace cv;
using namespace std;


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

/// Function headers
int findOutDieCount(Mat &);
bool isInsideEllipse(RotatedRect, RotatedRect);
void hsvThreshold();
RotatedRect countBetterFit(RotatedRect);

int findOutDieCount(Mat & src) {
    cvtColor(src, hsvImg, COLOR_BGR2HSV);
    inRange(hsvImg, Scalar(bLowH, bLowS, bLowV), Scalar(bHighH, bHighS, bHighV), hsvThresholded);

    Mat greenTh = hsvThresholded;

    //get out background - set on green

    int minBound = 0;
    int maxBound = hsvThresholded.cols;

    //row optimalization
    for (int row = 0; row < hsvThresholded.rows; row++) {
        //find leftBound
        unsigned char* data = hsvThresholded.ptr(row);
        for (int col = 0; col < hsvThresholded.cols; col++) {
            if (*data != 0) {
                minBound = col;
                break;
            }
            data++;
        }
        //find rightBound
        data = hsvThresholded.ptr(row) + hsvThresholded.cols - 1;
        for (int col = hsvThresholded.cols - 1; col >= 0; col--) {
            if (*data != 0) {
                maxBound = col;
                break;
            }
            data--;
        }
        //adjust
        if (maxBound - 20 <= minBound) {
            minBound += 22;
        }
        //set proper background
        data = hsvThresholded.ptr(row);
        for (int col = 0; col < hsvThresholded.cols; col++) {
            if (col < minBound || col > maxBound || *data != 0) {
                src.at<cv::Vec3b>(row, col)[0] = 0;
                src.at<cv::Vec3b>(row, col)[1] = 255;
                src.at<cv::Vec3b>(row, col)[2] = 0;
            }
            data++;
        }
    }
    //col optimalization
    cvtColor(src, hsvImg, COLOR_BGR2HSV);
    inRange(hsvImg, Scalar(bLowH, bLowS, bLowV), Scalar(bHighH, bHighS, bHighV), hsvImg);
    transpose(hsvImg, hsvThresholded);
    minBound = 0;
    maxBound = hsvThresholded.cols;
    for (int row = 0; row < hsvThresholded.rows; row++) {
        //find leftBound
        unsigned char* data = hsvThresholded.ptr(row);
        for (int col = 0; col < hsvThresholded.cols; col++) {
            if (*data != 0) {
                minBound = col;
                break;
            }
            data++;
        }
        //find rightBound
        data = hsvThresholded.ptr(row) + hsvThresholded.cols - 1;
        for (int col = hsvThresholded.cols - 1; col >= 0; col--) {
            if (*data != 0) {
                maxBound = col;
                break;
            }
            data--;
        }
        //adjust
        if (maxBound - 20 <= minBound) {
            minBound += 22;
        }
        //set proper background
        data = hsvThresholded.ptr(row);
        for (int col = 0; col < hsvThresholded.cols; col++) {
            if (col < minBound || col > maxBound || *data != 0) {
                src.at<cv::Vec3b>(col, row)[0] = 0;
                src.at<cv::Vec3b>(col, row)[1] = 255;
                src.at<cv::Vec3b>(col, row)[2] = 0;
            }
            data++;
        }
    }

    cvtColor(src, hsvImg, COLOR_BGR2HSV);

    hsvThreshold();
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Find contours
    findContours(hsvThresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    /// Find the rotated rectangles and ellipses for each contour
    vector<RotatedRect> minEllipse(contours.size());

    for (int i = 0; i < contours.size(); i++) {
        if (contours[i].size() > 5) {
            minEllipse[i] = fitEllipse(Mat(contours[i]));
        }
    }

    /// Draw contours + rotated rects + ellipses
    Mat drawing = Mat::zeros(hsvThresholded.size(), CV_8UC3);
    RotatedRect die;
    vector<RotatedRect> points;
    for (int i = 0; i < contours.size(); i++) {
        // ellipse
        //        ellipse(drawing, minEllipse[i], color, 2, 8);
//                cout << minEllipse[i].size.width << " : " << minEllipse[i].size.height << endl;
        if (minEllipse[i].size.width >= dieLowWidth && minEllipse[i].size.width <= dieHighWidth
                && minEllipse[i].size.height >= dieLowHeight && minEllipse[i].size.height <= dieHighHeight) {
            //possible die
            die = minEllipse[i];
        } else if (minEllipse[i].size.width >= pointLowWidth && minEllipse[i].size.width <= pointHighWidth
                && minEllipse[i].size.height >= pointLowHeight && minEllipse[i].size.height <= pointHighHeight) {
            //possible point
            points.push_back(minEllipse[i]);
        }
    }
    //draw die
    ellipse(drawing, die, Scalar(0, 0, 255), 3, 8);
    RotatedRect empiricEllipse = countBetterFit(die);
    ellipse(drawing, empiricEllipse, Scalar(0, 255, 255), 2, 8);
    int counter = 0;
    for (vector<RotatedRect>::iterator iter = points.begin(); iter != points.end();) {
        //check if it is inside
        if (!isInsideEllipse(*iter, die)) {
            iter = points.erase(iter);
            continue;
        }
        if (isInsideEllipse(*iter, empiricEllipse)) {
            counter++;
            ellipse(drawing, *iter, Scalar(255, 255, 0), 2, 8);
        } else {
            ellipse(drawing, *iter, Scalar(0, 255, 0), 2, 8);
        }
        iter++;
    }
    /// Show in a window
    stringstream ss;
    if (counter < 1 || counter > 6){
        //fail
        counter = -1;
    }
    //===========================debug output===================================
    /*namedWindow(ss.str(), CV_WINDOW_AUTOSIZE);
    imshow(ss.str(), drawing);*/
//    imwrite("test.jpg", drawing);
    //===========================/debug output==================================
    return counter;
}

bool isInsideEllipse(RotatedRect inside, RotatedRect ellipse) {
    double x = inside.center.x;
    double y = inside.center.y;
    x -= ellipse.center.x;
    y -= ellipse.center.y;
    double rotation = -ellipse.angle;
    rotation *= 3.14159 / 180.0;
    double newX = x * cos(rotation) - y * sin(rotation);
    double newY = x * sin(rotation) + y * cos(rotation);
    newX = abs(newX);
    newY = abs(newY);
    if (newX <= ellipse.size.width / 2.0 && newY <= ellipse.size.height / 2.0) {
        return true;
    }
    return false;
}

RotatedRect countBetterFit(RotatedRect old) {
    RotatedRect newEllipse;

    int tmp = old.center.x;
    if (tmp > 240) {
        tmp = 480 - tmp;
    }
    double xPerc = 0.0008125 * tmp + 0.805;
    double yPerc = 0.0016667 * old.center.y + 0.5;

    newEllipse.size = Size2f(old.size.width * xPerc, old.size.height * yPerc);
    if (old.center.x < 240) {
        newEllipse.center = Point2f(old.center.x - (old.size.width - newEllipse.size.width) / 2, old.center.y - (old.size.height - newEllipse.size.height) / 2);
    } else {
        newEllipse.center = Point2f(old.center.x + (old.size.width - newEllipse.size.width) / 2, old.center.y - (old.size.height - newEllipse.size.height) / 2);
    }

    return newEllipse;
}

void hsvThreshold() {
    Mat pointThresholded;
    if (iLowH <= iHighH) {
        inRange(hsvImg, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), hsvThresholded);
    } else {
        Mat imgThresh1;
        Mat imgThresh2;
        inRange(hsvImg, Scalar(0, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresh1);
        inRange(hsvImg, Scalar(iLowH, iLowS, iLowV), Scalar(179, iHighS, iHighV), imgThresh2);
        hsvThresholded = imgThresh1 + imgThresh2;
    }

    if (pLowH <= pHighH) {
        inRange(hsvImg, Scalar(pLowH, pLowS, pLowV), Scalar(pHighH, pHighS, pHighV), pointThresholded);
    } else {
        Mat imgThresh1;
        Mat imgThresh2;
        inRange(hsvImg, Scalar(0, pLowS, pLowV), Scalar(pHighH, pHighS, pHighV), imgThresh1);
        inRange(hsvImg, Scalar(pLowH, pLowS, pLowV), Scalar(179, pHighS, pHighV), imgThresh2);
        pointThresholded = imgThresh1 + imgThresh2;
    }
    erode(hsvThresholded, hsvThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 3)));
    dilate(hsvThresholded, hsvThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 3)));

    //morphological closing (removes small holes from the foreground)
    dilate(hsvThresholded, hsvThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 3)));
    erode(hsvThresholded, hsvThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 3)));

    erode(pointThresholded, pointThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 3)));
    dilate(pointThresholded, pointThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 3)));

    //morphological closing (removes small holes from the foreground)
    dilate(pointThresholded, pointThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 3)));
    erode(pointThresholded, pointThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 3)));

    hsvThresholded = hsvThresholded - pointThresholded;
}


