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
    foreach (const qint32 &it, lista){
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
    const char *dados = data;
    qDebug() << "executando serial.write()" << "data "<< dados << " max" << max;
    return porta.write(dados, max);
}
