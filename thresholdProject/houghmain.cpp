#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cmath>

using namespace cv;
using namespace std;

Mat src;
Mat hsvImg;
Mat hsvThresholded;
RNG rng(12345);
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

/// Function header
void thresh_callback(int, void*);

/** @function main */
int main(int argc, char** argv) {
    /// Load source image and convert it to gray
    src = imread("obrN1.jpg", 1);

   
    /// Convert image to gray and blur it
    //    cvtColor(src, src_gray, CV_BGR2GRAY);
    //    blur(src, src, Size(3, 3));



    /// Create Window
    char source_window[] = "Source";
//    namedWindow(source_window, CV_WINDOW_AUTOSIZE);

    cvtColor(src, hsvImg, COLOR_BGR2HSV);
    inRange(hsvImg, Scalar(bLowH, bLowS, bLowV), Scalar(bHighH, bHighS, bHighV), hsvThresholded);

    Mat greenTh = hsvThresholded;
//    namedWindow("Green background", CV_WINDOW_AUTOSIZE);
//    imshow("Green background", greenTh);
    
    //get out background - set on green
    
    int minBound = 0;
    int maxBound = hsvThresholded.cols;
    
    //row optimalization
    for (int row = 0; row < hsvThresholded.rows; row++){
        //find leftBound
        unsigned char* data = hsvThresholded.ptr(row);
        for (int col = 0; col < hsvThresholded.cols; col++){
            if (*data != 0){
                minBound = col;
                break;
            }
            data++;
        }
        //find rightBound
        data = hsvThresholded.ptr(row) + hsvThresholded.cols - 1;
        for (int col = hsvThresholded.cols - 1; col >= 0; col--){
            if (*data != 0){
                maxBound = col;
                break;
            }
            data--;
        }
        //adjust
        if (maxBound - 20 <= minBound){
            minBound += 22;
        }
        //set proper background
        data = hsvThresholded.ptr(row);
        for (int col = 0; col < hsvThresholded.cols; col++){
            if (col < minBound || col > maxBound || *data != 0){
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
    for (int row = 0; row < hsvThresholded.rows; row++){
        //find leftBound
        unsigned char* data = hsvThresholded.ptr(row);
        for (int col = 0; col < hsvThresholded.cols; col++){
            if (*data != 0){
                minBound = col;
                break;
            }
            data++;
        }
        //find rightBound
        data = hsvThresholded.ptr(row) + hsvThresholded.cols - 1;
        for (int col = hsvThresholded.cols - 1; col >= 0; col--){
            if (*data != 0){
                maxBound = col;
                break;
            }
            data--;
        }
        //adjust
        if (maxBound - 20 <= minBound){
            minBound += 22;
        }
        //set proper background
        data = hsvThresholded.ptr(row);
        for (int col = 0; col < hsvThresholded.cols; col++){
            if (col < minBound || col > maxBound || *data != 0){
                src.at<cv::Vec3b>(col, row)[0] = 0;
                src.at<cv::Vec3b>(col, row)[1] = 255;
                src.at<cv::Vec3b>(col, row)[2] = 0;
            }
            data++;
        }
    }
    
    
//    int minX = hsvThresholded.cols;
//    int minY = hsvThresholded.rows;
//    int maxX = 0;
//    int maxY = 0;
//
//    for (int row = 0; row < hsvThresholded.rows; row++) {
//        unsigned char* data = hsvThresholded.ptr(row);
//        for (int col = 0; col < hsvThresholded.cols; col++) {
//            if (*data != 0) {
//                if (col < minX){
//                    minX = col;
//                }
//                if (col > maxX){ \
//                    maxX = col; 
//                }
//                if (row < minY){
//                    minY = row;
//                }
//                if (row > maxY){
//                    maxY = row;
//                }
//            }
//            data++;
//        }
//    }
//    src = src.colRange(minX, maxX).rowRange(minY, maxY);
//    hsvImg = hsvImg.colRange(minX, maxX).rowRange(minY, maxY);
    cvtColor(src, hsvImg, COLOR_BGR2HSV);
//    imshow(source_window, src);



    createTrackbar("BodyLowH", "Source", &iLowH, 179, thresh_callback); //Hue (0 - 179)
    createTrackbar("BodyHighH", "Source", &iHighH, 179, thresh_callback);
    createTrackbar("BodyLowS", "Source", &iLowS, 255, thresh_callback); //Saturation (0 - 255)
    createTrackbar("BodyHighS", "Source", &iHighS, 255, thresh_callback);
    createTrackbar("BodyLowV", "Source", &iLowV, 255, thresh_callback); //Value (0 - 255)
    createTrackbar("BodyHighV", "Source", &iHighV, 255, thresh_callback);

    createTrackbar("PointLowH", "Source", &pLowH, 179, thresh_callback); //Hue (0 - 179)
    createTrackbar("PointHighH", "Source", &pHighH, 179, thresh_callback);
    createTrackbar("PointLowS", "Source", &pLowS, 255, thresh_callback); //Saturation (0 - 255)
    createTrackbar("PointHighS", "Source", &pHighS, 255, thresh_callback);
    createTrackbar("PointLowV", "Source", &pLowV, 255, thresh_callback); //Value (0 - 255)
    createTrackbar("PointHighV", "Source", &pHighV, 255, thresh_callback);



    thresh_callback(0, 0);

    waitKey(0);
    return (0);
}

bool isInsideEllipse(RotatedRect inside, RotatedRect ellipse){
    double x = inside.center.x;
    double y = inside.center.y;
    x -= ellipse.center.x;
    y -= ellipse.center.y;
    double rotation = - ellipse.angle;
    rotation *= 3.14159 / 180.0;
    double newX = x * cos(rotation) - y * sin(rotation);
    double newY = x * sin(rotation) + y * cos(rotation);
    newX = abs(newX);
    newY = abs(newY);
    if (newX <= ellipse.size.width / 2.0 && newY <= ellipse.size.height / 2.0){
        return true;
    }
    return false;
}

void hsvThreshold() {
    Mat pointThresholded;
    //    cvtColor(src, hsvImg, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
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

//    namedWindow("Body thresholded", CV_WINDOW_AUTOSIZE);
//    imshow("Body thresholded", hsvThresholded);

//    namedWindow("Points thresholded", CV_WINDOW_AUTOSIZE);
//    imshow("Points thresholded", pointThresholded);

    hsvThresholded = hsvThresholded - pointThresholded;

    /// Show in a window
//    namedWindow("Thresholded", CV_WINDOW_AUTOSIZE);
//    imshow("Thresholded", hsvThresholded);
}

RotatedRect countBetterFit(RotatedRect old){
    RotatedRect newEllipse;
    
    int tmp = old.center.x;
    if (tmp > 240){
        tmp = 480 - tmp;
    }
    double xPerc = 0.0008125 * tmp + 0.805;
    double yPerc = 0.0016667 * old.center.y + 0.5;
    
    newEllipse.size = Size2f(old.size.width * xPerc, old.size.height * yPerc);
    if (old.center.x < 240){
        newEllipse.center = Point2f(old.center.x - (old.size.width - newEllipse.size.width) / 2, old.center.y - (old.size.height - newEllipse.size.height) / 2);
    } else {
        newEllipse.center = Point2f(old.center.x + (old.size.width - newEllipse.size.width) / 2, old.center.y - (old.size.height - newEllipse.size.height) / 2);
    }
    
    return newEllipse;
}

/** @function thresh_callback */
void thresh_callback(int, void*) {
    hsvThreshold();
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using Threshold
    //    threshold(hsvThresholded, threshold_output, thresh, 255, THRESH_BINARY);
    /// Find contours
    findContours(hsvThresholded, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    /// Find the rotated rectangles and ellipses for each contour
    //    vector<RotatedRect> minRect(contours.size());
    vector<RotatedRect> minEllipse(contours.size());

    for (int i = 0; i < contours.size(); i++) {
        //        minRect[i] = minAreaRect(Mat(contours[i]));
        if (contours[i].size() > 5) {
            minEllipse[i] = fitEllipse(Mat(contours[i]));
        }
    }

    /// Draw contours + rotated rects + ellipses
    Mat drawing = Mat::zeros(hsvThresholded.size(), CV_8UC3);
    RotatedRect die;
    vector<RotatedRect> points;
    for (int i = 0; i < contours.size(); i++) {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        // contour
        //        drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
        // ellipse
//        ellipse(drawing, minEllipse[i], color, 2, 8);
        cout << minEllipse[i].size.width << " : " << minEllipse[i].size.height << endl;
        if (minEllipse[i].size.width >= 45.0 && minEllipse[i].size.width <= 70.0 
                && minEllipse[i].size.height >= 55.0 && minEllipse[i].size.height <= 90.0){
            //possible die
            die = minEllipse[i];
        } else if (minEllipse[i].size.width >= 5.0 && minEllipse[i].size.width <= 13.0 
                && minEllipse[i].size.height >= 8.0 && minEllipse[i].size.height <= 17.0){
            //possible point
            points.push_back(minEllipse[i]);
        }
        // rotated rectangle
        //        Point2f rect_points[4];
        //        minRect[i].points(rect_points);
        //        for (int j = 0; j < 4; j++)
        //            line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
    }
    //draw die
    ellipse(drawing, die, Scalar(0,0,255), 3, 8);
    RotatedRect empiricEllipse = countBetterFit(die);
    ellipse(drawing, empiricEllipse, Scalar(0,255,255), 2, 8);
    int counter = 0;
    for (vector<RotatedRect>::iterator iter = points.begin(); iter != points.end();){
        //check if it is inside
        if (!isInsideEllipse(*iter, die)){
            iter = points.erase(iter);
            continue;
        }
        if (isInsideEllipse(*iter, empiricEllipse)){
            counter++;
            ellipse(drawing, *iter, Scalar(255,255,0), 2, 8);
        } else {
            ellipse(drawing, *iter, Scalar(0,255,0), 2, 8);
        }
        iter++;
    }
    cout << "Result = " << counter << endl;    
    /// Show in a window
    namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    imshow("Contours", drawing);
}