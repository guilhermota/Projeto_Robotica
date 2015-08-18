#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QDebug>

#include <vector>
#include <string>

class database
{
public:
    database(QString hostname, QString dbname, QString username, QString password);
    ~database();

    bool retrieveFaces(std::vector<std::string> *path, std::vector<std::string> *labels);
    bool isConnected();

private:
    //QSqlDatabase *db;
    bool isOpen;
};

#endif // DATABASE_H
