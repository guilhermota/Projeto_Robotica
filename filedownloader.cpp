#include "filedownloader.h"

FileDownloader::FileDownloader(QUrl imageUrl, QObject *parent) :
 QObject(parent)
{
     connect(
     &m_WebCtrl, SIGNAL (finished(QNetworkReply*)),
     this, SLOT (fileDownloaded(QNetworkReply*))
     );

     QNetworkRequest request(imageUrl);
     request.setRawHeader("User-Agent","Mozila/5.0"); // Succeeds
     m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader() { }

void FileDownloader::fileDownloaded(QNetworkReply* pReply) {
    //qDebug() << pReply->errorString();
    //qDebug() << "pReply.attribute():" << QVariant(pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()).toString();
    qDebug() << "pReply.size():" << pReply->size();
    qDebug() << "preply" << pReply->isFinished() << pReply->bytesAvailable();
    m_DownloadedData = pReply->readAll();
    qDebug() << "m_DownloadedData.size(): " << m_DownloadedData.size();
    pReply->deleteLater();
    emit downloaded();

}

QByteArray FileDownloader::downloadedData() const {
    //qDebug() << "downloadedData";
    qDebug() << "isempty" << m_DownloadedData.isEmpty();
    qDebug() << "isnull" << m_DownloadedData.isNull();
    return m_DownloadedData;
}
