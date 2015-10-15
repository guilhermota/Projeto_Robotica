#include "mainwindow.h"
#include "ui_mainwindow.h"

/**
 * @brief MainWindow::MainWindow
 * Inicialização do objeto Janela Principal
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow){
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

MainWindow::~MainWindow(){
    delete ui;
    delete video;
}

/**
 * @brief MainWindow::on_actionSair_triggered
 * Fecha o programa
 */
void MainWindow::on_actionSair_triggered(){
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

/**
 * @brief MainWindow::on_actionSair_triggered
 * Cria objeto Video
 */
void MainWindow::on_actionAbrir_Camera_triggered(){
    qDebug("Criando objeto Video");
    video = new Video;
    connect(video, SIGNAL(emiteConfianca(double)), this, SLOT(escreveConfianca(double)));


    if(video->open()){
        QMessageBox::information(this, "Ae!", "Camera aberta!");
        connect(video, SIGNAL(sendQImage(QImage)), this, SLOT(imageReceived(QImage)));
    } else{
        QMessageBox::warning(this, "Erro", "Nao foi possivel abrir a camera.");
    }
}

/**
 * @brief MainWindow::on_actionListar_Portas_triggered
 * Abre porta serial
 */
void MainWindow::on_actionListar_Portas_triggered(){
    qDebug("Abrindo Dialog Listar Portas");
    QMessageBox::information(this, "Conexao Aberta", "Conexao Aberta.");
}


/**
 * @brief MainWindow::on_actionFechar_Camera_triggered
 * Para execução do objeto Video
 */
void MainWindow::on_actionFechar_Camera_triggered(){
    video->close();
}

/**
 * @brief MainWindow::on_actionTestar_Camera_triggered
 * Inicia reprodução do video e reconhecimento
 */
void MainWindow::on_actionTestar_Camera_triggered(){
    video->play();
}

/**
 * @brief MainWindow::imageReceived
 * Atualiza vídeo na interface gráfica
 */
void MainWindow::imageReceived(QImage image){
    ui->imagem->setPixmap(QPixmap::fromImage(image));
    //qDebug("Pixamp Atualizado");
    ui->imagem->show();
}

/**
 * @brief MainWindow::updateStatus
 * Esta funcao atualiza os valores sendo mostrados na janela princiapal.
 */
void MainWindow::updateStatus(){
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

/**
 * @brief MainWindow::databaseInputs
 * Lê as informações necessárias para conectar ao banco de dados
 */
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

/**
 * @brief MainWindow::on_actionCarregar_triggered
 * Faz download das imagens dos usuarios cadastrados no banco de dados
 */
void MainWindow::on_actionCarregar_triggered(){
    QString host, banco, login, senha;

    //databaseInputs(&host, &banco, &login, &senha);

    //database db(host, banco, login, senha);
    //database db("guilhermo.com.br", "u3209476_", "u3209476_", "senhabancodedados");
    database db("127.0.0.1", "u3209476_db", "root", "usbw");

    if(!db.isConnected()) return;

    paths.clear(); names.clear(); labels.clear();

    db.retrieveFaces(&this->paths, &this->names, &this->labels);

    for(size_t i = 0; i < paths.size(); i++){
        QString url;
        //url = "http://www.guilhermo.com.br/projeto/sistema/" + QString::fromStdString(paths[i]);
        url = "http://localhost:8080/Projeto/projeto/sistema/" + QString::fromStdString(paths[i]);
        urls.push_back(url);
        urls_.push_back(false);
    }

    for(size_t i = 0; i < paths.size(); i++){
        qDebug() << "path: " << QString::fromStdString(paths[i]) << " label: " << QString::fromStdString(names[i]);
        qDebug() << "names: " << QString::fromStdString(names[i]) << " label: " << labels[i];
    }

    faces.clear();

    for(size_t i = 0; i < paths.size(); i++){
        m_pImgCtrl = new FileDownloader(urls[i], this);
        connect(m_pImgCtrl, SIGNAL (downloaded()), this, SLOT (loadImage()));
    }

    qDebug() <<  "faces.size(): " << faces.size();

}

/**
 * @brief MainWindow::on_actionCarregar_local_triggered()
 * Carrega imagens dos usuarios cadastrados no banco de dados
 */
void MainWindow::on_actionCarregar_local_triggered()
{
    //databaseInputs(&host, &banco, &login, &senha);
    database db("127.0.0.1", "u3209476_db", "root", "usbw");

    if(!db.isConnected()) return;

    paths.clear(); names.clear(); labels.clear();

    db.retrieveFaces(&this->paths, &this->names, &this->labels);

    for(size_t i = 0; i < paths.size(); i++){
        QString url;
        url = "C:/Users/guilhermo/Desktop/USBWebserver v8.6/root/Projeto/projeto/sistema/" + QString::fromStdString(paths[i]);
        urls.push_back(url);
        urls_.push_back(false);
    }

    for(size_t i = 0; i < paths.size(); i++){
        qDebug() << "path: " << QString::fromStdString(paths[i]) << " label: " << QString::fromStdString(names[i]);
        qDebug() << "names: " << QString::fromStdString(names[i]) << " label: " << labels[i];
    }

    faces.clear();

    for(size_t i = 0; i < paths.size(); i++){
        qDebug() << "path: " << QString::fromStdString(paths[i]) << " " << i;
        cv::Mat mat = cv::imread(urls[i].toStdString());
        if(mat.empty()){ qDebug() << "empty"; return;}
        faces.push_back(mat);
    }

    qDebug() <<  "faces.size(): " << faces.size();
}

/**
 * @brief MainWindow::loadImage
 * Carrega imagens e as converte para cv::Mat
 */
void MainWindow::loadImage(){
    //qDebug() << "loadImage()";
    QImage image;
    cv::Mat mat;
    image.loadFromData(m_pImgCtrl->downloadedData());
    if(image.isNull()){qDebug() << "null"; return;}
    mat = cvtQImage2CvMat(image);
    if(mat.empty()){qDebug() << "empty"; return;}
    faces.push_back(mat);
    qDebug() <<  "faces.size(): " << faces.size();
}

/**
 * @brief MainWindow::on_actionSobre_triggered
 * Abre janela "Sobre"
 */
void MainWindow::on_actionSobre_triggered(){
    //QMessageBox::about(this, "Perdao pelo vacilo", "Tentei fazer uma janelinha legal. Mas eu juro que não sei por que esta por** nao funciona :)");
    Sobre sobre;
    sobre.exec();
}

/**
 * @brief MainWindow::on_actionTestar_imagens_triggered
 * Mostra imagens carregadas
 */
void MainWindow::on_actionTestar_imagens_triggered(){
    cv::namedWindow("teste");
    for(size_t i = 0; i < faces.size(); i++){
        qDebug() << "testando mat " << i;
        cv::imshow("teste", faces[i]);
        cv::waitKey(2000);
    }
}

/**
 * @brief MainWindow::on_actionTreinar_triggered(){
 * Treina algortimo de reconhecimento
 */
void MainWindow::on_actionTreinar_triggered(){
    video->train(faces, names, labels);
}

/**
 * @brief MainWindow::on_actionTestar_Laser_triggered
 * Testa algoritmo de detecção do laser
 */
void MainWindow::on_actionTestar_Laser_triggered(){
    video->testarLaser();
}

/**
 * @brief MainWindow::on_actionSalvar_triggered
 * Salva treinamento do algortimo de reconhecimento
 */
void MainWindow::on_actionSalvar_triggered(){
    video->save("modelo_reconhecimento2.mr");
}

/**
 * @brief  MainWindow::on_actionCarregar_2_triggered
 * Carrega treinamento do algortimo de reconhecimento
 */
void MainWindow::on_actionCarregar_2_triggered(){
    video->load("modelo_reconhecimento2.mr");
}

/**
 * @brief MainWindow::on_doubleSpinBox_valueChanged
 * Altera threshold do reconhecimento
 */
void MainWindow::on_doubleSpinBox_valueChanged(double arg1){
    video->setThreshold(arg1);
}

/**
 * @brief MainWindow::escreveConfianca
 * Atualiza informação da confianca do reconhecimento na GUI
 */
void MainWindow::escreveConfianca(double confianca)
{
    ui->doubleSpinBox_2->setValue(confianca);
}

