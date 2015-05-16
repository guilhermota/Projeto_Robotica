#ifndef VIDEO_H
#define VIDEO_H

#include <QDebug>
#include <QThread>
#include <QImage>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "qtopencv.h"

class Video : public QThread
{
     Q_OBJECT
public:
    Video();
    ~Video();

    void run();

    bool open();
    void close();
    void play();

signals:
    void sendQImage(QImage image);

private:
    cv::VideoCapture cap;
};

#endif // VIDEO_H
