#include "database.h"

/**
 * @brief database::database
 * Abre conexão com o banco de dados
 */
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

database::~database(){}

/**
 * @brief database::retrieveFaces
 * Executa query que busca os usuarios cadastrados, seus nomes e o diretorio de suas imagens
 */
bool database::retrieveFaces(std::vector<std::string> *path, std::vector<std::string> *names, std::vector<int> *labels)
{
    QSqlQuery query;
    bool ok = query.exec("select a.nome, b.path, a.id_usuario "
               "from usuarios a, imagens b, usuarios_has_imagens c "
               "where a.id_usuario = c.usuarios_id_usuario "
               "and b.id_imagem = c.imagens_id_imagem;");

    //qDebug() << query.lastError().text();

    if(ok){
        //qDebug() << query.isSelect();
    }

    while(query.next()){
        //qDebug() << "name: " << query.value(0).toString() << " path: " << query.value(1).toString();
        path->push_back(query.value(1).toString().toStdString());
        names->push_back(query.value(0).toString().toStdString());
        labels->push_back(query.value(2).toInt());
    }

    return true;
}

/**
 * @brief database::isConnected
 * Retorna se a conexão está aberta ou não
 */
 bool database::isConnected(){
     return isOpen;
 }

 /**
  * @brief database::uploadImagem
  * Envia imagem ao servidor
  */
 void database::uploadImagem(QByteArray imagem)
 {
     QSqlQuery query;
     QString query1 = "insert into imagens";
     QString query2 = "";
     QString query3 = "";
     bool ok1 = query.exec(query1);
     bool ok2 = query.exec(query2);
     bool ok3 = query.exec(query3);


     QNetworkAccessManager *manager = new QNetworkAccessManager(this);
     connect(manager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(replyFinished(QNetworkReply*)));

     manager->post(QNetworkRequest(QUrl("http://guilhermo.com.br/projeto/sistema/recebeimagem.php")), imagem);



     /*QHttp
     http->setHost(siteURLEdit->text());

     QByteArray ba;
     QBuffer buffer( &ba );
     buffer.open( QBuffer::ReadWrite );
     originalPixmap.save( &buffer , "PNG" );

     http->post("/wp-content/plugins/wp-shotcam.php",ba);
     */
 }
