#include "video.h"

#define _FPS 10
#define _CAP_WIDTH 640
#define _CAP_HEIGHT 480
#define _FACE_CASCADE_PATH "C:/OpenCV/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml"
#define _PROFILE_FACE_CASCADE_PATH "C:/OpenCV/opencv/sources/data/haarcascades/haarcascade_profile_face.xml"
#define _EYE_CASCADE_PATH "C:/OpenCV/opencv/sources/data/haarcascades/haarcascade_mcs_nose.xml"
#define _COR_POSITIVO CV_RGB(0, 255,0)
#define _COR_NEGATIVO CV_RGB(255, 0,0)
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
                //faceReconhecida = false;
                //qDebug("face on");
            }

            frame_count = 0;
        }
        //isFaceOn = tracker.findFaces(frame, &face_point);
        //cv::namedWindow("teste");
        recognizer->set("threshold", 275);
        if(!faceReconhecida && !faces.empty()){
            //qDebug() << "reconhecendo";
            cv::Mat gray;
            cv::cvtColor(frame(faces[0]), gray, CV_BGR2GRAY);
            //cv::normalize(frame(faces[0]), gray, 0, 255, cv::NORM_MINMAX, CV_8UC1);
            //cv::imshow("teste", gray);
            //cv::waitKey(50);
            //prediction = recognizer->predict(gray);
            recognizer->predict(gray, label, con);
            qDebug() << label << con;
            if(con < 275){
                qDebug() << "reconheceu";
                faceReconhecida = true;
                cor = _COR_POSITIVO;
                box_text = recognizer->getLabelInfo(label);
                qDebug() << QString::fromStdString(box_text);
                atirou = false;
            } else{
                qDebug() << "nao reconheceu";
                faceReconhecida = false;
                cor = _COR_NEGATIVO;
                box_text = "Desconhecido";
            }
        }

        if(isFaceOn){
            //for(size_t i = 0; i < faces.size(); i++){
            cv::rectangle(frame, faces[0], cor);
            int pos_x = std::max(faces[0].tl().x - 10, 0);
            int pos_y = std::max(faces[0].tl().y - 10, 0);
            cv::putText(frame, box_text, cv::Point(pos_x, pos_y), cv::FONT_HERSHEY_DUPLEX, 0.5, cor, 1.0);
             //}
        }

        if(!faceReconhecida && !atirou){
            ajustaMira(faces[0]);
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
    //qDebug() << "vai treinar";
    recognizer->train(src, labels);
    qDebug() << "treinou";
    std::map<int, std::string> infoLabels;
    for(size_t i = 0; i < labels.size(); i++){
        if(infoLabels.find(labels[i]) == infoLabels.end()){
            infoLabels.insert(std::pair<int,std::string>(labels[i],names[i]));
            //qDebug() << i << labels[i] << QString::fromStdString(names[i]);
        }
        qDebug() << i << labels[i] << QString::fromStdString(infoLabels[labels[i]]);
    }
    recognizer->setLabelsInfo(infoLabels);

    for (std::map<int, std::string>::iterator it=infoLabels.begin(); it!=infoLabels.end(); ++it)
        qDebug() << it->first << " => " << QString::fromStdString(it->second);
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

void Video::testarLaser()
{
    /*cv::namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

     int iLowH = 49;
     int iHighH = 102;

     int iLowS = 94;
     int iHighS = 253;

     int iLowV = 147;
     int iHighV = 255;

      //Create trackbars in "Control" window
     cvCreateTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
     cvCreateTrackbar("HighH", "Control", &iHighH, 179);

     cvCreateTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
     cvCreateTrackbar("HighS", "Control", &iHighS, 255);

     cvCreateTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
     cvCreateTrackbar("HighV", "Control", &iHighV, 255);*/

    configuraLaser();

     while (true){
      cv::Mat imgOriginal;

      bool bSuccess = cap.read(imgOriginal); // read a new frame from video

      if (!bSuccess){
        qDebug() << "Cannot read a frame from video stream" << endl;
        break;
      }

      cv::Point minLoc, maxLoc;
      double minVal, maxVal;
      // circle center

       // "channels" is a vector of 3 Mat arrays:
       std::vector<cv::Mat> channels(3);
       // split img:
       cv::split(imgOriginal, channels);
       // get the channels (dont forget they follow BGR order in OpenCV)

       cv::minMaxLoc(channels[1], &minVal, &maxVal, &minLoc, &maxLoc, cv::noArray());
       qDebug() << "minVal = " << minVal << "maxVal = " << maxVal << "minLoc = " << minLoc.x << minLoc.y << "maxLoc = " << maxLoc.x << maxLoc.y;

        cv::circle( imgOriginal, maxLoc, 3, cv::Scalar(0,255,0), -1, 8, 0 );
        cv::namedWindow("teste");
        cv::imshow("teste", imgOriginal);
        if (cv::waitKey(30) == 27){
          qDebug() << "esc key is pressed by user" << endl;
          break;
        }

      //cv::cvtColor(imgOriginal, imgOriginal, cv::COLOR_BGR2GRAY); qDebug() << "foi";
      /*imgOriginal = imgOriginal + cv::Scalar(-10, -10 -10);
      imgOriginal.convertTo(imgOriginal, -1, 0.7, 0);

      cv::medianBlur(imgOriginal, imgOriginal, 3);

      cv::Mat imgHSV;

      cv::cvtColor(imgOriginal, imgHSV, cv::COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

      cv::Mat imgThresholded;

      cv::inRange(imgHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded);

      cv::erode(imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));
      cv::dilate( imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));

      cv::dilate( imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));
      cv::erode(imgThresholded, imgThresholded, cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3)));

      cv::GaussianBlur(imgThresholded, imgThresholded, cv::Size(9, 9), 2, 2);

      std::vector<cv::Vec3f> circles;
      cv::HoughCircles( imgThresholded, circles, CV_HOUGH_GRADIENT, 1, imgThresholded.rows/8, 200, 100, 0, 0 );

        /// Draw the circles detected
        for( size_t i = 0; i < circles.size(); i++ )
        {
            cv::Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
            int radius = cvRound(circles[i][2]);
            // circle center
            cv::circle( imgOriginal, center, 3, cv::Scalar(0,255,0), -1, 8, 0 );
            // circle outline
            cv::circle( imgOriginal, center, radius, cv::Scalar(0,0,255), 3, 8, 0 );
         }

      cv::imshow("Thresholded Image", imgThresholded); //show the thresholded image
      cv::imshow("Original", imgOriginal); //show the original image

      if (cv::waitKey(30) == 27){
        qDebug() << "esc key is pressed by user" << endl;
        break;
      }*/

    }
    return;

}

void Video::configuraLaser()
{
    cv::namedWindow("Controle", CV_WINDOW_AUTOSIZE);

    cvCreateTrackbar("Min Hue", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("Max Hue", "Control", &iHighH, 179);

    cvCreateTrackbar("Min Sat", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("Max Sat", "Control", &iHighS, 255);

    cvCreateTrackbar("Min Val", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("Max Val", "Control", &iHighV, 255);
}

void Video::achaLaser(cv::Mat *frame)
{
    double minVal, maxVal;
    cv::Point minPos;

    std::vector<cv::Mat> channels(3);

    cv::split(*frame, channels); // canais na ordem BGR

    cv::minMaxLoc(channels[1], &minVal, &maxVal, &minPos, &laserPos, cv::noArray());
    //qDebug() << "minVal = " << minVal << "maxVal = " << maxVal << "minLoc = " << minLoc.x << minLoc.y << "maxLoc = " << maxLoc.x << maxLoc.y;
}

void Video::ajustaMira(cv::Rect facePos)
{
    if(!serial.isOpen()) return;

    int faceMin = facePos.x + 20; // menor ponto para atirar com um valor de ajuste
    int faceMax = facePos.x + facePos.width - 20; // maior ponto para atirar com um valor de ajuste

    if(laserPos.x < faceMin){
        serial.write("d", 64);
        qDebug() << "Girando motor para direita";
        QThread::msleep(50);
    } else if(laserPos.x > faceMax ){
        serial.write("l", 64);
        qDebug() << "Girando motor para esquerda";
        QThread::msleep(50);
    } else{
        qDebug() << "Atirou!";
        serial.write("a", 64);
        QThread::msleep(50);
        atirou = true;
    }
}

void Video::abreSerial()
{
    serial.exec();
}

void Video::fechaSerial()
{
    serial.fechaConexao();
}
