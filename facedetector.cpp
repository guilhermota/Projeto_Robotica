#include <vector>
#include <iostream>
#include "opencv2/highgui/highgui.hpp"

#include "FaceDetector.h"

/**
 * @brief FaceDetector::FaceDetector
 * Inicializa obeto carregando os cascacades necessários para realizar o reconhecimento
 */
FaceDetector::FaceDetector(const std::string &cascadePathFaces, const std::string &cascadePathEyes)
{
   qDebug() << "carregando cascade faces: " << cascadeFaces.load(cascadePathFaces);
   qDebug() << "carregando cascade olhos: " << cascadeEyes.load(cascadePathEyes);
   qDebug() << "carregando cascade oculos: " << cascadeOculos.load("C:/OpenCV/opencv/sources/data/haarcascades/haarcascade_mcs_mouth.xml");

}

FaceDetector::~FaceDetector() {}

/**
 * @brief FaceDetector::findFacesInImage
 * Procura por faces na imagem e as retorna no vector res
 */
void FaceDetector::findFacesInImage(const cv::Mat &img, std::vector<cv::Rect> &res) {
    cv::Mat tmp = img.clone();
    std::vector<cv::Rect> faces;
    std::vector<cv::Rect> eyes;

    //converte a imagem para escala de cinza e a normaliza
    int channels = img.channels();
    if((channels == 3 || channels == 4)) cv::cvtColor(tmp, tmp, CV_BGR2GRAY);
    cv::equalizeHist(tmp, tmp);

    res.clear(); //limpa o vector resultado para não incluir faces que não estão mais na imagem

    //detecta as faces
    cascadeFaces.detectMultiScale(tmp, faces, 1.1, 10, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));

    for(size_t i = 0; i < faces.size(); i++){
        //detecta outras estruturas faciais, como olhos, narizes ou bocas para evitar falsos positivos
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
