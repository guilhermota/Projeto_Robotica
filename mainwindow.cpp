#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow
 *
 * Inicializacao do objeto Janela Principal
 *
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //timer update janela principal
    timer = new QTimer(this);

    //conexoes signal slot
    connect(timer, SIGNAL(timeout()), this, SLOT(updateStatus()));

    //criacoes das labels na janela principal
    labelUsoMemoria = new QLabel(this);
    labelTotalMemoria = new QLabel(this);
    labelMemoriaProcesso = new QLabel(this);
    /*labelUsoCpu = new QLabel(this);*/
    labelProcessoCpu = new QLabel(this);
    labelTotal = new QLabel("Uso Total: ", this);
    labelUso = new QLabel("Uso Atual: ", this);
    labelProcesso = new QLabel("Memoria Processo: ", this);
    //labelCpuUso = new QLabel("Uso CPU: ", this);
    labelCpuProcesso = new QLabel("Processo CPU: ", this);

    //adicionando labels na status bar
    /*ui->statusBar->addPermanentWidget(labelCpuUso);
        ui->statusBar->addPermanentWidget(labelUsoCpu);*/
    ui->statusBar->addPermanentWidget(labelCpuProcesso);
        ui->statusBar->addPermanentWidget(labelProcessoCpu);

    ui->statusBar->addPermanentWidget(labelTotal);
        ui->statusBar->addPermanentWidget(labelTotalMemoria);
    ui->statusBar->addPermanentWidget(labelUso);
        ui->statusBar->addPermanentWidget(labelUsoMemoria);
    ui->statusBar->addPermanentWidget(labelProcesso);
        ui->statusBar->addPermanentWidget(labelMemoriaProcesso);

    //inicia a janela ja com uso de memoria e cpu
    updateStatus();
    //inicia timer update janela principal
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete video;
}

void MainWindow::on_actionSair_triggered()
{
    //mensagem ara confirmar a saida
    QMessageBox msgBox;
    msgBox.setText("Sair");
    msgBox.setInformativeText("Voce tem certeza que quer sair?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Question);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Yes){
        this->close();
    } else{
        return;
    }
}

void MainWindow::on_actionAbrir_Camera_triggered()
{
    qDebug("Criando objeto Video");
    video = new Video;


    if(video->open()){
        QMessageBox::information(this, "Ae!", "Camera aberta!");
        connect(video, SIGNAL(sendQImage(QImage)), this, SLOT(imageReceived(QImage)));
    } else{
        QMessageBox::warning(this, "Erro", "Nao foi possivel abrir a camera.");
    }
}

void MainWindow::on_actionListar_Portas_triggered()
{    
    qDebug("Abrindo Dialog Listar Portas");
    //serial.exec();
    video->abreSerial();
    QMessageBox::information(this, "Conexao Aberta", "Conexao Aberta.");
}

void MainWindow::on_actionTestar_Conexao_triggered()
{
    /*if(serial.isOpen()){
       char dados = 9;

       qDebug("enviando sinal serial");
       qDebug() << serial.write(&dados, 8);

       //serial.write(&dados, 512);

       QThread::msleep(1000);
      } else {
        QMessageBox::warning(this, "Erro!", "Nao ha nenhuma porta aberta!");
    }*/
}

void MainWindow::on_actionFechar_Conexao_triggered()
{
    qDebug("Fechando Conexão Serial");
    //serial.fechaConexao();
    //QMessageBox::information(this, "Conexao Fechada", "Conexao Fechada.");
    video->fechaSerial();
    QMessageBox::information(this, "Conexao Fechada", "Conexao Fechada.");
}

void MainWindow::on_actionFechar_Camera_triggered()
{
    video->close();
}

void MainWindow::on_actionTestar_Camera_triggered()
{
    video->play();
}

void MainWindow::imageReceived(QImage image)
{
    ui->imagem->setPixmap(QPixmap::fromImage(image));
    //qDebug("Pixamp Atualizado");
    ui->imagem->show();
}

/**
 * @brief MainWindow::updateStatus
 *
 * Esta funcao atualiza os valores sendo mostrados na janela princiapal.
 */
void MainWindow::updateStatus()
{
    totalMemoria = mp.getTotalMemory();
    usoMemoria = mp.getMemoryUsage();
    memoriaProcesso = mp.getMemoryUsageProcess();
    //usoCpu = mp.getUsoCpu();
    processoCpu = mp.getCpuProcesso();

    double pctUso = usoMemoria * 100 / totalMemoria;
    double pctProcesso = memoriaProcesso * 100 / totalMemoria;

    labelTotalMemoria->setText(QString::number(totalMemoria) );
    labelUsoMemoria->setText(QString::number(pctUso) + "%");
    labelMemoriaProcesso->setText(QString::number(pctProcesso) + "%");
    //labelUsoCpu->setText(QString::number(usoCpu) + "%");
    labelProcessoCpu->setText(QString::number(processoCpu) + "%");

    ui->statusBar->update();
}

void MainWindow::databaseInputs(QString *host, QString *db, QString *login, QString *senha){
    QString temp;
    bool ok;
    //host
    temp = QInputDialog::getText(this, tr("Dados Database"),
                                            tr("Host:"), QLineEdit::Normal,
                                           "exemple.com", &ok);
    if (ok && !temp.isEmpty()) *host = temp;

    //database
    temp = QInputDialog::getText(this, tr("Dados Database"),
                                            tr("Database:"), QLineEdit::Normal,
                                            "u3209476", &ok);
    if (ok && !temp.isEmpty()) *db = temp;

    //login
    temp = QInputDialog::getText(this, tr("Dados Database"),
                                            tr("Login:"), QLineEdit::Normal,
                                            "u3209476", &ok);
    if (ok && !temp.isEmpty()) *login = temp;

    //senha
    temp = QInputDialog::getText(this, tr("Dados Database"),
                                            tr("Senha:"), QLineEdit::Normal,
                                            "", &ok);
    if (ok && !temp.isEmpty()) *senha = temp;
}

void MainWindow::on_actionCarregar_triggered()
{
    QString host, banco, login, senha;

    //databaseInputs(&host, &banco, &login, &senha);

    //database db(host, banco, login, senha);
    database db("guilhermo.com.br", "u3209476_", "u3209476_", "senhabancodedados");

    if(!db.isConnected()) return;

    paths.clear(); names.clear(); labels.clear();

    db.retrieveFaces(&this->paths, &this->names, &this->labels);

    for(size_t i = 0; i < paths.size(); i++){
        QString url;
        url = "http://www.guilhermo.com.br/projeto/sistema/" + QString::fromStdString(paths[i]);
        urls.push_back(url);
        urls_.push_back(false);
    }

    /*for(size_t i = 0; i < paths.size(); i++){
        //qDebug() << "path: " << QString::fromStdString(paths[i]) << " label: " << QString::fromStdString(names[i]);
        qDebug() << "names: " << QString::fromStdString(names[i]) << " label: " << labels[i];
    }*/
    faces.clear();

    for(size_t i = 0; i < paths.size(); i++){
        m_pImgCtrl = new FileDownloader(urls[i], this);
        connect(m_pImgCtrl, SIGNAL (downloaded()), this, SLOT (loadImage()));
    }

    qDebug() <<  "faces.size(): " << faces.size();

}

void MainWindow::on_actionSobre_triggered()
{
    //QMessageBox::about(this, "Perdao pelo vacilo", "Tentei fazer uma janelinha legal. Mas eu juro que não sei por que esta por** nao funciona :)");
    Sobre sobre;
    sobre.exec();
}

void MainWindow::loadImage()
{
    //qDebug() << "loadImage()";
    QImage image;
    cv::Mat mat;
    image.loadFromData(m_pImgCtrl->downloadedData());
    qDebug() << image.isNull();
    mat = cvtQImage2CvMat(image);
    if(mat.empty()){ qDebug() << "empty"; return;}
    //cv::resize(mat, mat, cv::Size(640, 480));
    cv::cvtColor(mat, mat, CV_BGR2GRAY);
    faces.push_back(mat);
    /*ui->imagem->setPixmap(QPixmap::fromImage(cvtCvMat2QImage(mat)));
    ui->imagem->show();*/
    qDebug() <<  "faces.size(): " << faces.size();
}

void MainWindow::on_actionTestar_imagens_triggered()
{
    for(size_t i = 0; i < faces.size(); i++){
        qDebug() << "testando mat " << i;
        cv::imshow("teste", faces[i]);
        cv::waitKey(500);
    }
}

void MainWindow::on_actionTreinar_triggered()
{
    video->train(faces, names, labels);
}

void MainWindow::on_actionTestar_Laser_triggered()
{
    video->testarLaser();
}

void MainWindow::on_actionSalvar_triggered()
{
    video->save("modelo_reconhecimento.mr");
}

void MainWindow::on_actionCarregar_2_triggered()
{
    video->load("modelo_reconhecimento.mr");
}
