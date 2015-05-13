#include "video.h"
#include <QDebug>
#include <QMessageBox>

#define _FPS 20

Video::Video()
{
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(CV_CAP_PROP_FPS, _FPS);
    cap.set(CV_CAP_PROP_FORMAT, CV_8U);
}

void Video::run()
{
    qDebug("Thread Running");

    cv::Mat frame;
    QImage image;
    int delay = 50;

    cap.read(frame);

    qDebug() << delay;

    while(cap.read(frame)){
        //qDebug("Frame capturado");
        image = cvtCvMat2QImage(frame);
        //qDebug("Frame convertido");
        emit sendQImage(image);
        //qDebug("Frame enviado");
        //cv::imshow("MyVideo",frame);
        msleep(delay);
        //if(cv::waitKey(delay) == 27) break;
    }
}

Video::~Video()
{
    this->close();
}


//TODO Escolher qual camera abrir
bool Video::open()
{
    //qDebug("Abrindo Video");
    cap.open(0);

    //qDebug("Video aberto");
    if(cap.isOpened()){
        return true;
    } else{
        return false;
    }
}

void Video::close()
{
    cap.release();
    this->quit();
}

void Video::play()
{
    //qDebug("video start()");
    this->start();
}
