#ifndef VIDEO_H
#define VIDEO_H

#include "opencv2/highgui/highgui.hpp"
#include <QDebug>

class Video
{
public:
    Video();
    ~Video();

    bool open();
    void close();
private:
    cv::VideoCapture cap;
};

#endif // VIDEO_H
