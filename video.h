#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QImage>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "qtopencv.h"
#include "facedetector.h"
#include "facetracker.h"

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
    FaceDetector* detector;
    //FaceTracker tracker;
    cv::Rect face_point;
};

#endif // VIDEO_H
