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
   qDebug() << "carregando cascade oculos: " << cascadeOculos.load("C:/OpenCV/opencv/sources/data/haarcascades/haarcascade_mcs_mouth.xml");

}

FaceDetector::~FaceDetector() {}

void FaceDetector::findFacesInImage(const cv::Mat &img, std::vector<cv::Rect> &res) {
    //qDebug() << "procurndo faces";
    cv::Mat tmp = img.clone();
    std::vector<cv::Rect> faces;
    std::vector<cv::Rect> eyes;

    //convert the image to grayscale and normalize histogram:
    int channels = img.channels();
    if((channels == 3 || channels == 4)) cv::cvtColor(tmp, tmp, CV_BGR2GRAY);
    cv::equalizeHist(tmp, tmp);

    //clear the vector:
    res.clear();

    //detect faces:
    cascadeFaces.detectMultiScale(tmp, faces, 1.1, 10, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));
    //qDebug() << faces.size();

    for(size_t i = 0; i < faces.size(); i++){
        cascadeEyes.detectMultiScale(tmp, eyes, 1.1, 10, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));
        if(!eyes.empty()){
            res.push_back(faces[i]);
        } else{
            cascadeOculos.detectMultiScale(tmp, eyes, 1.1, 10, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));
        }
        if(!eyes.empty()){
            res.push_back(faces[i]);
        } else qDebug() << "nao encontrou nada";
    }

}
