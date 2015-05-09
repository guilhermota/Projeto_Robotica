#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <qDebug>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSair_triggered()
{
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

void MainWindow::on_actionAbrir_Video_triggered()
{

}

void MainWindow::on_actionListar_Portas_triggered()
{    
    qDebug("Abrindo Dialog Listar Portas");
    serial.exec();
}

void MainWindow::on_actionTestar_Conexao_triggered()
{

}
