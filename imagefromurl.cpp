#include "imagefromurl.h"
#include <opencv2/highgui/highgui.hpp>

ImageFromURL::ImageFromURL(QObject *parent) : QObject(parent)
{
    //manager = new QNetworkAccessManager(this);
    manager = new QNetworkAccessManager(this); qDebug() << "criou manager";
    reply = NULL;
}

ImageFromURL::~ImageFromURL()
{
    delete manager;
}

void ImageFromURL::retrieveImage(cv::Mat *mat, QUrl url){
    qDebug() << url.toString();

    connect(manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(replyFinished(QNetworkReply*))); qDebug() << "conectou slot manager";

    reply = manager->get(QNetworkRequest(url)); qDebug() << "get";



    QImage image; image.loadFromData(reply->readAll());

    *mat = cvtQImage2CvMat(image);
    cv::imshow("teste", *mat);

}



void ImageFromURL::replyFinished (QNetworkReply *reply)
{
    qDebug() << "entrou no replyFinished";
    if(reply->error()){
        qDebug() << "ERROR!";
        qDebug() << reply->errorString();
    }
    else{
        qDebug() << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << reply->header(QNetworkRequest::LastModifiedHeader).toDateTime().toString();;
        qDebug() << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong();
        qDebug() << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        qDebug() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();

        /*QFile *file = new QFile("C:/Qt/Dummy/downloaded.txt");
        if(file->open(QFile::Append))
        {
            file->write(reply->readAll());
            file->flush();
            file->close();
        }
        delete file;*/
    }

    reply->deleteLater();
}
