#include "facetracker.h"

FaceTracker::FaceTracker()
{

}

bool FaceTracker::findFaces(cv::Mat frame, cv::Rect *point_result)
{
    return true;
}

void FaceTracker::setObject(cv::Mat _object){
    //cv::Mat image = _object;
    cv::cvtColor(_object, object, CV_BGR2GRAY);
    cv::goodFeaturesToTrack(object, points[1], 50, 0.01, 10, cv::Mat(), 3, 0, 0.04);
    //for(size_t i = 0; i < points[1].size(); i++ ){
    //    circle( image, points[1][i], 3, cv::Scalar(0,255,0), -1, 8);
    //}
    //cv::imshow("teste", image);
    cv::TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.3);
    cv::Size subPixWinSize(10,10);
    cv::cornerSubPix(object, points[1], subPixWinSize,cv::Size(-1,-1), termcrit);
    for(size_t i = 0; i < points[1].size(); i++){
        cv::circle(object, points[1][i], 3, cv::Scalar(255,123,12));
    }
    cv::imshow("teste", object);
    cv::waitKey(100);

}

