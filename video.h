#ifndef VIDEO_H
#define VIDEO_H

#include <string>
#include <QDebug>
#include <QMessageBox>
#include <QThread>
#include <QImage>
#include <QString>
#include <QSound>
#include "qtopencv.h"
#include "facedetector.h"
#include "serialport.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QBuffer>


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

    void setThreshold(double threshold);

    void enviaImagem(QByteArray imagem, int id);

    void replyFinished(QNetworkReply*);

signals:
    void sendQImage(QImage image);
    void emiteSom();
    void emiteConfianca(double confianca);

private slots:
    void tocaSom();

private:
    cv::VideoCapture cap;
    FaceDetector* detector;
    SerialPort serial;
    //Recognizer* recognizer;
    cv::Ptr<cv::FaceRecognizer> recognizer;
    cv::Ptr<cv::FaceRecognizer> recognizer1;
    cv::Rect face_point;

    cv::Mat frame;
    QImage image;
    int delay = 50;
    std::vector<cv::Rect> faces;
    bool faceReconhecida = false;
    bool isFaceOn = false;
    double con;
    int label = -1;
    double con1;
    int label1 = -1;
    cv::Scalar cor;
    std::string box_text;

    int iLowH = 0;
    int iHighH = 179;
    int iLowS = 0;
    int iHighS = 255;
    int iLowV = 0;
    int iHighV = 255;

    cv::Point laserPos;
    bool atirou = false;
    bool avisou = false;
    bool enviou = false;
    double threshold = 275;
};

#endif // VIDEO_H
