
#ifndef FACEDETECTOR_H
#define	FACEDETECTOR_H

#include <QDebug>
#include <string>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class FaceDetector {
public:
    FaceDetector(const std::string &cascadePathFaces, const std::string &cascadePathEyes);
    virtual ~FaceDetector();
    void findFacesInImage(const cv::Mat &img, std::vector<cv::Rect> &res);
private:
    cv::CascadeClassifier cascadeFaces;
    cv::CascadeClassifier cascadeEyes;
    cv::CascadeClassifier cascadeOculos;

    cv::Mat tmp;
    std::vector<cv::Rect> faces;
    std::vector<cv::Rect> eyes;
};

#endif	/* FACEDETECTOR_H */

