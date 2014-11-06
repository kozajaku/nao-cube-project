#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

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

int iLowV = 85;
int iHighV = 255;

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
    src = imread("obr3.jpg", 1);

    /// Convert image to gray and blur it
    //    cvtColor(src, src_gray, CV_BGR2GRAY);
    //    blur(src_gray, src_gray, Size(3, 3));

    /// Create Window
    char* source_window = "Source";
    namedWindow(source_window, CV_WINDOW_AUTOSIZE);
    imshow(source_window, src);



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

void hsvThreshold() {
    Mat pointThresholded;
    cvtColor(src, hsvImg, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
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
    erode(hsvThresholded, hsvThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate(hsvThresholded, hsvThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    //morphological closing (removes small holes from the foreground)
    dilate(hsvThresholded, hsvThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    erode(hsvThresholded, hsvThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    erode(pointThresholded, pointThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    dilate(pointThresholded, pointThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    //morphological closing (removes small holes from the foreground)
    dilate(pointThresholded, pointThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    erode(pointThresholded, pointThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

    namedWindow("Body thresholded", CV_WINDOW_AUTOSIZE);
    imshow("Body thresholded", hsvThresholded);

    namedWindow("Points thresholded", CV_WINDOW_AUTOSIZE);
    imshow("Points thresholded", pointThresholded);

    hsvThresholded = hsvThresholded - pointThresholded;

    /// Show in a window
    namedWindow("Thresholded", CV_WINDOW_AUTOSIZE);
    imshow("Thresholded", hsvThresholded);
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
    cout << "=====================OUTPUT=========================" << endl;
    for (int i = 0; i < contours.size(); i++) {
        Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
        // contour
        //        drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
        // ellipse
        ellipse(drawing, minEllipse[i], color, 2, 8);
        cout << minEllipse[i].size.width << " " << minEllipse[i].size.height << endl;
        // rotated rectangle
        //        Point2f rect_points[4];
        //        minRect[i].points(rect_points);
        //        for (int j = 0; j < 4; j++)
        //            line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
    }
    /// Show in a window
    namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    imshow("Contours", drawing);
}