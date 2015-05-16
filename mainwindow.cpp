#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    serial.exec();
}


void MainWindow::on_actionTestar_Conexao_triggered()
{
    if(serial.isOpen()){
       char dados = 13;

       qDebug("enviando sinal serial");
       qDebug() << serial.write(&dados, 8);

       //serial.write(&dados, 512);

       QThread::msleep(1000);
      } else {
        QMessageBox::warning(this, "Erro!", "Nao ha nenhuma porta aberta!");
    }
}

//TODO Criar esta janela

void MainWindow::on_actionSobre_triggered()
{

}

void MainWindow::on_actionFechar_Conexao_triggered()
{
    serial.fechaConexao();
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

//TODO Colocar funcoes de cpu e memoria em outro arquivo


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

