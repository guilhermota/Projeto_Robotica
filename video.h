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
#include "serialport.h"
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

    void abreSerial();
    void fechaSerial();

    void train(std::vector<cv::Mat> src, std::vector<std::string> names, std::vector<int> labels);
    int predict(cv::InputArray src);
    void save(const QString& filename);
    void load(const QString& filename);

    void testarLaser();
    void configuraLaser();
    void achaLaser(cv::Mat *frame);
    void ajustaMira(cv::Rect facePos);

signals:
    void sendQImage(QImage image);

private:
    cv::VideoCapture cap;
    FaceDetector* detector;
    SerialPort serial;
    //Recognizer* recognizer;
    cv::Ptr<cv::FaceRecognizer> recognizer;
    cv::Rect face_point;

    cv::Mat frame;
    QImage image;
    int delay = 50;
    std::vector<cv::Rect> faces;
    bool faceReconhecida = false;
    bool isFaceOn = false;
    double con;
    int label = -1;
    cv::Scalar cor;
    std::string box_text;

    int iLowH = 0;
    int iHighH = 179;
    int iLowS = 0;
    int iHighS = 255;
    int iLowV = 0;
    int iHighV = 255;

    cv::Point laserPos;
    bool atirou;
};

#endif // VIDEO_H
