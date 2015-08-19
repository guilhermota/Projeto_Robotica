#include "database.h"

#include <QSqlError>
database::database(QString hostname, QString dbname, QString username, QString password)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostname);
    db.setDatabaseName(dbname);
    db.setUserName(username);
    db.setPassword(password);
    db.open();
    isOpen = db.isOpen();
    if(isOpen){
        qDebug() << "Conected to database;";
    } else{
        qDebug() << db.driverName();
        qDebug() << db.lastError();
    }
}

database::~database()
{

}

bool database::retrieveFaces(std::vector<std::string> *path, std::vector<std::string> *names, std::vector<int> *labels)
{
    QSqlQuery query;
    bool ok = query.exec("select a.nome, b.path, a.id_usuario "
               "from usuarios a, imagens b, usuarios_has_imagens c "
               "where a.id_usuario = c.usuarios_id_usuario "
               "and b.id_imagem = c.imagens_id_imagem;");

    qDebug() << query.lastError().text();

    if(ok){
        qDebug() << query.isSelect();
    }

    while(query.next()){
        //qDebug() << "name: " << query.value(0).toString() << " path: " << query.value(1).toString();
        path->push_back(query.value(1).toString().toStdString());
        names->push_back(query.value(0).toString().toStdString());
        labels->push_back(query.value(2).toInt());
    }

    return true;
}

 bool database::isConnected(){
     return isOpen;
 }
