/*
* Projeto Robotica
* School projet for my Robotics class. This program provides face
* recognition and communication with serial ports.
*
* serialport.cpp
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

#include "serialport.h"
#include "ui_serialport.h"

#include <qDebug>
#include <QMessageBox>

SerialPort::SerialPort(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SerialPort)
{
    qDebug("Criando QDialog Serial Port");
    ui->setupUi(this);
    listaPortas();
    connect(this, SIGNAL(portaSelecionada(QString,qint32)), this, SLOT(abrePorta(QString,qint32)));
}

SerialPort::~SerialPort()
{
    delete ui;
}

void SerialPort::listaPortas()
{
    QList<QSerialPortInfo> lista;
    lista.clear();
    ui->comboBoxPortas->clear();

    lista = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &porta, lista)
    {
        ui->comboBoxPortas->addItem(porta.portName());
    }
}

void SerialPort::on_comboBoxPortas_currentIndexChanged(const QString &arg1)
{
    QSerialPortInfo porta(arg1);
    QList<qint32> lista = porta.standardBaudRates();

    ui->_labelDescricao->setText(porta.description());
    ui->_labelFabricante->setText(porta.manufacturer());
    ui->_labelLocalizacao->setText(porta.systemLocation());
    ui->_labelNome->setText(porta.portName());
    ui->_labelProduto->setText(QString::number(porta.productIdentifier()));
    ui->_labelVendedor->setText(QString::number(porta.vendorIdentifier()));
    foreach (const qint32 &it, lista)
    {
        ui->comboBoxBaud->addItem(QString::number(it));
    }
}

void SerialPort::on_buttonBox_accepted()
{
    QString nome = ui->_labelNome->text();
    qint32 baudrate = ui->comboBoxBaud->currentText().toInt();

    emit portaSelecionada(nome, baudrate);
}

void SerialPort::abrePorta(QString nome,qint32 baudrate)
{
    porta.setPortName(nome);
    porta.setBaudRate(baudrate);
    if(!porta.open(QIODevice::ReadWrite)){
        QMessageBox::critical(this, "Erro!", "Esta porta nao pode ser aberta!");
    }
    return;
}

qint64 SerialPort::write(const char *data, qint64 max)
{
    return porta.write(data, max);
}
