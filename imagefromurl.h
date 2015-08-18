#ifndef IMAGEFROMURL_H
#define IMAGEFROMURL_H

#include "opencv2/core/core.hpp"
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include "qtopencv.h"
#include <QImage>


class ImageFromURL : public QObject
{
    Q_OBJECT

public:
    explicit ImageFromURL(QObject *parent = 0);
    ~ImageFromURL();

    void retrieveImage(cv::Mat *mat, QUrl url);

public slots:
    void replyFinished (QNetworkReply *reply);


private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
};

#endif // IMAGEFROMURL_H
