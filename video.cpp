#include "video.h"

#define _FPS 10
#define _CAP_WIDTH 640
#define _CAP_HEIGHT 480
#define _FACE_CASCADE_PATH "C:/OpenCV/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml"
#define _PROFILE_FACE_CASCADE_PATH "C:/OpenCV/opencv/sources/data/haarcascades/haarcascade_profile_face.xml"
#define _EYE_CASCADE_PATH "C:/OpenCV/opencv/sources/data/haarcascades/haarcascade_mcs_nose.xml"
#define _COR_POSITIVO cv::Scalar(255, 128, 0)
#define _COR_NEGATIVO cv::Scalar(255, 0, 0)
#define _FACE_RADIUS_RATIO 0.75
#define _CIRCLE_THICKNESS 2.5
#define _LINE_TYPE CV_AA

Video::Video()
{
    qDebug() << "criando video";
    cap.set(CV_CAP_PROP_FRAME_WIDTH, _CAP_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, _CAP_HEIGHT);
    cap.set(CV_CAP_PROP_FPS, _FPS);
    cap.set(CV_CAP_PROP_FORMAT, CV_8U);

    detector = new FaceDetector(std::string(_FACE_CASCADE_PATH), std::string(_EYE_CASCADE_PATH));
    recognizer = cv::createLBPHFaceRecognizer();
}

Video::~Video()
{
    //delete detector;
    this->close();
}

void Video::run()
{
    cv::Mat frame;
    QImage image;
    int delay = 50;
    std::vector<cv::Rect> faces;
    bool faceReconhecida = false;
    bool isFaceOn = false;
    int prediction = 0;

    cap.read(frame);
    short int frame_count = 12;

    while(cap.read(frame)){
        if(frame_count == 12){
            detector->findFacesInImage(frame, faces);
            if(faces.empty()){
                isFaceOn = false;
                faceReconhecida = false;
                //qDebug("face off");
            } else {
                isFaceOn = true;
                //reconhecer;
                cv::Mat gray;
                cv::cvtColor(frame(faces[0]), gray, CV_BGR2GRAY);
                prediction = predict(gray);
                if(prediction == 0){
                    qDebug() << "nao reconheceu" << prediction;
                } else{
                    qDebug() << "reconheceu" << prediction;
                }
                //faceReconhecida = false;
                //qDebug("face on");
            }

            frame_count = 0;
        }
        //isFaceOn = tracker.findFaces(frame, &face_point);

        if(!faceReconhecida && !faces.empty()){
            qDebug() << "reconhecendo";
            faceReconhecida = true;
        }

        if(isFaceOn){
            //for(size_t i = 0; i < faces.size(); i++){
            cv::Scalar cor;
            if(prediction == 0 ) cor = _COR_NEGATIVO;
            else cor = _COR_POSITIVO;

            cv::rectangle(frame, faces[0], cor);
            std::string box_text = cv::format("Prediction = %d -> %s", prediction, recognizer->getLabelInfo(prediction));
            int pos_x = std::max(faces[0].tl().x - 10, 0);
            int pos_y = std::max(faces[0].tl().y - 10, 0);
            cv::putText(frame, box_text, cv::Point(pos_x, pos_y), cv::FONT_HERSHEY_DUPLEX, 1.0, cor, 2.0);
             //}

            //cv::rectangle(frame, faces[0], _COR_POSITIVO);
        }

        image = cvtCvMat2QImage(frame);

        emit sendQImage(image);

        msleep(delay);

        frame_count++;
    }
}


//TODO Escolher qual camera abrir
bool Video::open()
{
    //qDebug("Abrindo Video");
    //cap.open("C:/Users/guilhermo/Desktop/Two.and.a.Half.Men.S12E09.HDTV.x264-LOL.mp4");
    cap.open(0);

    //qDebug("Video aberto");
    if(cap.isOpened()){
        return true;
    } else{
        return false;
    }
}

void Video::close()
{
    //qDebug() << "release";
    cap.release();
    //qDebug() << "quit";
    this->quit();
}

void Video::play()
{
    //qDebug("video start()");
    this->start();
}



void Video::train(std::vector<cv::Mat> src, std::vector<std::string> names, std::vector<int> labels)
{
    qDebug() << "vai treinar";
    recognizer->train(src, labels);
    qDebug() << "treinou";
    std::map<int, std::string> infoLabels;
    for(size_t i = 0; i < labels.size(); i++){
        if(infoLabels.find(labels[i]) == infoLabels.end()){
            infoLabels[labels[i]] = names[i];
        }
    }
    recognizer->setLabelsInfo(infoLabels);
    return;
}

int Video::predict(cv::InputArray src)
{
    return recognizer->predict(src);
}

void Video::save(const QString& filename)
{
    recognizer->save(filename.toStdString());
    return;
}

void Video::load(const QString& filename)
{
    recognizer->load(filename.toStdString());
    return;
}
