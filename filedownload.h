#ifndef FILEDOWNLOAD_H
#define FILEDOWNLOAD_H

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class FileDownload
{
public:
    FileDownload();
    ~FileDownload();

    QNetworkReply* get(QUrl url);

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
};

#endif // FILEDOWNLOAD_H
