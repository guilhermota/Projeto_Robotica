#include "video.h"

#define _FPS 10
#define _CAP_WIDTH 640
#define _CAP_HEIGHT 480
#define _FACE_CASCADE_PATH "C:/OpenCV/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml"
#define _COR_POSITIVO cv::Scalar(0, 255, 0)
#define _FACE_RADIUS_RATIO 0.75
#define _CIRCLE_THICKNESS 2.5
#define _LINE_TYPE CV_AA

Video::Video()
{
    cap.set(CV_CAP_PROP_FRAME_WIDTH, _CAP_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, _CAP_HEIGHT);
    cap.set(CV_CAP_PROP_FPS, _FPS);
    cap.set(CV_CAP_PROP_FORMAT, CV_8U);

    detector = new FaceDetector(std::string(_FACE_CASCADE_PATH));
    //detector = new FaceDetector(std::string(_FACE_CASCADE_PATH), 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(30,30));
}

Video::~Video()
{
    //delete detector;
    this->close();
}

void Video::run()
{
    //qDebug("Thread Running");

    cv::Mat frame;
    QImage image;
    int delay = 50;

    cap.read(frame);
    short int frame_count = 0;

    //qDebug() << delay;
    std::vector<cv::Rect> faces;

    while(cap.read(frame)){
        //cv::resize(frame, frame, cv::Size(640, 480));
        //qDebug("Frame capturado");
        if(frame_count == 5){
            frame_count = 0;
            detector->findFacesInImage(frame, faces);
            //qDebug() << "Faces empty: " << faces.empty();
            /*for (std::vector<cv::Rect>::const_iterator face = faces.begin() ; face != faces.end() ; face++){
                qDebug("tem algo aqui");
                cv::Point center(face->x + face->width * 0.5, face->y + face->height * 0.5);
                cv::circle(frame, center, _FACE_RADIUS_RATIO * face->width, _COR_POSITIVO);
            }*/
        }
        for(size_t i = 0; i < faces.size(); i++){
            cv::Point center(faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height * 0.5);
            cv::circle(frame, center, _FACE_RADIUS_RATIO * faces[i].width, _COR_POSITIVO);
            //cv::rectangle(frame, faces[i], _COR_POSITIVO);
        }

        image = cvtCvMat2QImage(frame);
        //qDebug("Frame convertido");
        emit sendQImage(image);
        //qDebug("Frame enviado");
        //cv::imshow("MyVideo",frame);
        msleep(delay);
        //if(cv::waitKey(delay) == 27) break;

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
    cap.release();
    this->quit();
}

void Video::play()
{
    //qDebug("video start()");
    this->start();
}
