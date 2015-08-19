#ifndef RECOGNIZER_H
#define RECOGNIZER_H

#include "opencv2/core/core.hpp"
#include "opencv2/contrib/contrib.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include <QString>

class Recognizer
{
public:
    Recognizer();
    ~Recognizer();

    void train(cv::InputArray src, cv::InputArray labels);
    void predict(cv::InputArray src);
    void save(const QString& filename);
    void load(const QString& filename);

private:
    //cv::FaceRecognizer facerec;

};

#endif // RECOGNIZER_H
