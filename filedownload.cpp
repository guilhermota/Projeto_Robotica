#include "filedownload.h"

FileDownload::FileDownload()
{
    manager = new QNetworkAccessManager();
}

FileDownload::~FileDownload()
{
    delete manager;
}

 QNetworkReply* FileDownload::get(QUrl url){
    connect(manager, SIGNAL(finished(QNetworkReply*)),
        this, SLOT(replyFinished(QNetworkReply*)));

    reply = manager->get(QNetworkRequest(QUrl(url)));
}
