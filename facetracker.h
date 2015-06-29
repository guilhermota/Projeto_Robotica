#ifndef FACETRACKER_H
#define FACETRACKER_H

#include <vector>
#include "opencv2/core/core.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/nonfree/nonfree.hpp"
#include "opencv2/nonfree/features2d.hpp"

class FaceTracker
{
public:
    FaceTracker();
    bool findFaces(cv::Mat frame, cv::Rect *point_result);
    void faceWasOnLastFrame(int i);
    void setObject(cv::Mat _object);
private:
    cv::Mat object;

    bool objectFound = false;

    cv::Mat gray, prevGray, image;
    std::vector<cv::Point2f> points[2];


};

#endif // FACETRACKER_H








