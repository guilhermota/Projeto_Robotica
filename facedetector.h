
#ifndef FACEDETECTOR_H
#define	FACEDETECTOR_H

#include <QDebug>
#include <string>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class FaceDetector {
public:
    FaceDetector(const std::string &cascadePath);
    virtual ~FaceDetector();
    void findFacesInImage(const cv::Mat &img, std::vector<cv::Rect> &res);
private:
    cv::CascadeClassifier _cascade;
};

#endif	/* FACEDETECTOR_H */

