#ifndef HSVIMAGETHRESHOLDER_H
#define HSVIMAGETHRESHOLDER_H

class HSVImageThresholder
{
public:
    HSVImageThresholder(int hueFrom, int hueTo, int saturationFrom, int saturationTo, int valueFrom, int valueTo);
    Mat thesholdImage(Mat imgHSV);
private:
    int hueFrom;
    int hueTo;
    int saturationFrom;
    int saturationTo;
    int valueFrom;
    int valueTo;
};

#endif // HSVIMAGETHRESHOLDER_H
