#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QImage>
#include <QString>
#include "qtopencv.h"
#include "facedetector.h"
#include "facetracker.h"
//#include "recognizer.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"


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

    void train(std::vector<cv::Mat> src, std::vector<std::string> names, std::vector<int> labels);
    int predict(cv::InputArray src);
    void save(const QString& filename);
    void load(const QString& filename);

signals:
    void sendQImage(QImage image);

private:
    cv::VideoCapture cap;
    FaceDetector* detector;
    //Recognizer* recognizer;
    cv::Ptr<cv::FaceRecognizer> recognizer;
    cv::Rect face_point;
};

#endif // VIDEO_H
