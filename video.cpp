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

/**
 * @brief Video::Video
 * Iniciliza objeto video, criando Detector de Faces e Reconhecedor
 */
 Video::Video(){
 qDebug() << "criando video";
    cap.set(CV_CAP_PROP_FRAME_WIDTH, _CAP_WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, _CAP_HEIGHT);
    cap.set(CV_CAP_PROP_FPS, _FPS);
    cap.set(CV_CAP_PROP_FORMAT, CV_8U);

    detector = new FaceDetector(std::string(_FACE_CASCADE_PATH), std::string(_EYE_CASCADE_PATH));
    recognizer = cv::createLBPHFaceRecognizer();
    recognizer1 = cv::createEigenFaceRecognizer();

    connect(this, SIGNAL(emiteSom()), this, SLOT(tocaSom()));
}


Video::~Video(){
    //delete detector;
    this->close();
}

/**
 * @brief Video::run
 * Este é o metódo executado na thread do video. É aqui que o frame é lido, as faces são detectadas e reconhecidas,
 * toma a decisão de o que fazer, ajusta a mira e atira, e emite o frame para a GUI mostra-lo
 */
void Video::run(){
    cap.read(frame);
    short int frame_count = 12;
    recognizer->set("threshold", threshold);
    recognizer1->set("threshold", threshold);

    while(cap.read(frame)){
        if(frame_count == 12){
            detector->findFacesInImage(frame, faces);
            if(faces.empty()){
                isFaceOn = false;
                faceReconhecida = false;
                avisou = false;
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
        if(!faceReconhecida && !faces.empty()){
            //qDebug() << "reconhecendo";
            cv::Mat gray;
            cv::cvtColor(frame(faces[0]), gray, CV_BGR2GRAY);
            cv::resize(gray, gray, cv::Size(200,200));
            //cv::normalize(frame(faces[0]), gray, 0, 255, cv::NORM_MINMAX, CV_8UC1);
            //cv::imshow("teste", gray);
            //cv::waitKey(50);
            //prediction = recognizer->predict(gray);
            recognizer->predict(gray, label, con);
            recognizer1->predict(gray, label1, con1);
            qDebug() << con << con1 << label << label1;
            double ratio = con/con1;
            if(con == 0 || con1 == 0) ratio = 1;
            if(con > 1000 || con1 > 1000) ratio = -1;
            emit emiteConfianca(ratio);
            if(ratio < 2 && ratio > 0.5){
            //if(con < threshold){
                //qDebug() << "reconheceu";
                faceReconhecida = true;
                cor = _COR_POSITIVO;
                //box_text = recognizer->getLabelInfo(label);
                //qDebug() << QString::fromStdString(box_text);
                atirou = false;
            } else{
                //qDebug() << "nao reconheceu";
                faceReconhecida = false;
                cor = _COR_NEGATIVO;
                box_text = "Desconhecido";
                if(!avisou) emit emiteSom();
                avisou = true;
            //}
            }
        }

        if(isFaceOn){
            //for(size_t i = 0; i < faces.size(); i++){
            cv::rectangle(frame, faces[0], cor);
            //int pos_x = std::max(faces[0].tl().x - 10, 0);
            //int pos_y = std::max(faces[0].tl().y - 10, 0);
            //cv::putText(frame, box_text, cv::Point(pos_x, pos_y), cv::FONT_HERSHEY_DUPLEX, 0.5, cor, 1.0);
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

/**
 * @brief Video::open
 * Abre webcam
 */
//TODO Escolher qual camera abrir
bool Video::open(){
    //qDebug("Abrindo Video");
    cap.open(0);
    if(cap.isOpened()){
        return true;
    } else{
        return false;
    }
}

/**
 * @brief Video::close
 * Para a execução da thread
 */
void Video::close(){
    //qDebug() << "release";
    cap.release();
    //qDebug() << "quit";
    this->quit();
}

/**
 * @brief Video::play
 * Começa a execução da thread
 */
void Video::play(){
    //qDebug("video start()");
    this->start();
}

/**
 * @brief Video::train
 * Procura faces nas imagens para terinar, as pre-processa e então terina os algoritmos
 */
void Video::train(std::vector<cv::Mat> src, std::vector<std::string> names, std::vector<int> labels){
    std::vector<cv::Mat> temp;
    std::vector<cv::Rect> faces;
    for(size_t i = 0; i < src.size(); i++){
        qDebug() << "detectando";
        detector->findFacesInImage(src[i], faces);
        qDebug() << "achou";
        qDebug() << faces.size();
        cv::Mat mat = src[i](faces[0]);
        cv::resize(mat, mat, cv::Size(200,200));
        cv::cvtColor(mat, mat, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(mat, mat);
        temp.push_back(mat);
    }
    recognizer->train(temp, labels);
    recognizer1->train(temp, labels);
    qDebug() << "treinou";
    std::map<int, std::string> infoLabels;
    for(size_t i = 0; i < labels.size(); i++){
        if(infoLabels.find(labels[i]) == infoLabels.end()){
            infoLabels.insert(std::pair<int,std::string>(labels[i],names[i]));
            //qDebug() << i << labels[i] << QString::fromStdString(names[i]);
        }
        qDebug() << i << labels[i] << QString::fromStdString(infoLabels[labels[i]]);
    }
    //recognizer->setLabelsInfo(infoLabels);

    for (std::map<int, std::string>::iterator it=infoLabels.begin(); it!=infoLabels.end(); ++it)
        qDebug() << it->first << " => " << QString::fromStdString(it->second);
    return;
}

/**
 * @brief Video::save
 * Salva treinamento dos algoritmos
 */
void Video::save(const QString& filename){
    std::string file = filename.toStdString();
    std::string file1 = filename.toStdString() + "_eigen";
    recognizer->save(file);
    recognizer1->save(file1);
    return;
}

/**
 * @brief Video::load
 * Carrega treinamento dos algoritmos
 */
void Video::load(const QString& filename){
    std::string file = filename.toStdString();
    std::string file1 = filename.toStdString() + "_eigen";
    recognizer->load(file);
    recognizer1->load(file1);
    return;
}

/**
 * @brief Video::testarLaser
 * Testa algortimo de detecção do laser
 */
void Video::testarLaser(){

    //configuraLaser();

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
    }
    return;
}

/**
 * @brief Video::configuraLaser
 * Configura a detecção do laser
 */
void Video::configuraLaser(){
    cv::namedWindow("Controle", CV_WINDOW_AUTOSIZE);

    cvCreateTrackbar("Min Hue", "Control", &iLowH, 179); //Hue (0 - 179)
    cvCreateTrackbar("Max Hue", "Control", &iHighH, 179);

    cvCreateTrackbar("Min Sat", "Control", &iLowS, 255); //Saturation (0 - 255)
    cvCreateTrackbar("Max Sat", "Control", &iHighS, 255);

    cvCreateTrackbar("Min Val", "Control", &iLowV, 255); //Value (0 - 255)
    cvCreateTrackbar("Max Val", "Control", &iHighV, 255);
}

/**
 * @brief Video::achaLaser
 * Acha a posição do laser no frame
 */
void Video::achaLaser(cv::Mat *frame){
    double minVal, maxVal;
    cv::Point minPos;

    std::vector<cv::Mat> channels(3);

    cv::split(*frame, channels); // canais na ordem BGR

    cv::minMaxLoc(channels[1], &minVal, &maxVal, &minPos, &laserPos, cv::noArray());
    //qDebug() << "minVal = " << minVal << "maxVal = " << maxVal << "minLoc = " << minLoc.x << minLoc.y << "maxLoc = " << maxLoc.x << maxLoc.y;
}

/**
 * @brief Video::ajustaMira
 * Ajusta a mira da sentry e atira, enviando informações para o Arduino
 */
void Video::ajustaMira(cv::Rect facePos){
    if(!serial.isOpen()) return;

    int faceMin = facePos.x - 20; // menor ponto para atirar com um valor de ajuste
    int faceMax = facePos.x + facePos.width + 20; // maior ponto para atirar com um valor de ajuste

    if(laserPos.x > faceMax){
        char *data = (char *)'d';
        serial.write(data);
        qDebug() << "Girando motor para direita";
        //QThread::msleep(500);
    } else if(laserPos.x < faceMin ){
        char *data = (char *)'l';
        serial.write(data);
        qDebug() << "Girando motor para esquerda";
        //QThread::msleep(500);
    } else{
        qDebug() << "Atirou!";
        char *data = (char *)'a';
        serial.write(data);
        //QThread::msleep(500);
        atirou = true;
    }
}

/**
 * @brief Video::abreSerial
 * Abre porta USB
 */
void Video::abreSerial(){
    serial.exec();
}

/**
 * @brief Video::fechaSerial
 * Fecha porta USB
 */
void Video::fechaSerial(){
    serial.fechaConexao();
}

/**
 * @brief Video::tocaSom
 * Toca som de aviso
 */
void Video::tocaSom(){
    QSound::play(":/rsc/aviso.wav");
}

/**
 * @brief Video::setThreshold
 * Atualiza threshold dos algoritmos
 */
void Video::setThreshold(double threshold){
    recognizer->set("threshold", threshold);
    recognizer1->set("threshold", threshold);
    this->threshold = threshold;
}
