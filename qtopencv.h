#ifndef QTOPENCV
#define QTOPENCV

#include <QtGui/QImage>
#include <opencv2/core/core.hpp>

// Converter OpenCV Mat para QImage
QImage cvtCvMat2QImage(const cv::Mat & inMat);

// Converter QImage tpara OpenCV Mat
cv::Mat cvtQImage2CvMat(const QImage & image);


#endif // QTOPENCV

