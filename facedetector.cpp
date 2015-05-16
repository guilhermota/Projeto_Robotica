/*
 * File:   FaceDetector.cpp
 * Author: Eyal Arubas <EyalArubas at gmail>
 */

#include <vector>
#include <iostream>

#include "FaceDetector.h"

FaceDetector::FaceDetector(const std::string &cascadePath)
{
   qDebug() << "carregando cascade faces: " << _cascade.load(cascadePath);
}

FaceDetector::~FaceDetector() {}

void FaceDetector::findFacesInImage(const cv::Mat &img, std::vector<cv::Rect> &res) {
    qDebug() << "procurndo faces";
    cv::Mat tmp;  

    //convert the image to grayscale and normalize histogram:
    cv::cvtColor(img, tmp, CV_BGR2GRAY);
    cv::equalizeHist(tmp, tmp);

    //clear the vector:
    res.clear();

    //detect faces:
    _cascade.detectMultiScale(tmp, res, 1.1, 10, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));
}
