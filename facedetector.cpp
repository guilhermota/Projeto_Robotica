/*
 * File:   FaceDetector.cpp
 * Author: Eyal Arubas <EyalArubas at gmail>
 */

#include <vector>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"

#include "FaceDetector.h"

FaceDetector::FaceDetector(const std::string &cascadePathFaces, const std::string &cascadePathEyes)
{
   qDebug() << "carregando cascade faces: " << cascadeFaces.load(cascadePathFaces);
   qDebug() << "carregando cascade olhos: " << cascadeEyes.load(cascadePathEyes);

}

FaceDetector::~FaceDetector() {}

void FaceDetector::findFacesInImage(const cv::Mat &img, std::vector<cv::Rect> &res) {
    //qDebug() << "procurndo faces";
    //cv::Mat tmp;
    //std::vector<cv::Rect> faces;
    //std::vector<cv::Rect> eyes;

    //convert the image to grayscale and normalize histogram:
    cv::cvtColor(img, tmp, CV_BGR2GRAY);
    cv::equalizeHist(tmp, tmp);

    //clear the vector:
    res.clear();

    //detect faces:
    cascadeFaces.detectMultiScale(tmp, faces, 1.1, 10, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));

    for(size_t i = 0; i < faces.size(); i++){
        cascadeEyes.detectMultiScale(tmp, eyes, 1.1, 10, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));
        if(!eyes.empty()){
            res.push_back(faces[i]);
        }
    }

}
