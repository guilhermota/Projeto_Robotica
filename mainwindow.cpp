/*
* Projeto Robotica
* School projet for my Robotics class. This program provides face
* recognition and communication with serial ports.
*
* mainwindow.cpp
*
* Copyright (c) 2015 Guilhermo T. Agranionih
*
* Author: Guilhermo T. Agranionih  <guilhermoagranionih@gmail.com>
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without restriction,
* including without limitation the rights to use, copy, modify, merge,
* publish, distribute, sublicense, and/or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
* BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
* ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

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
