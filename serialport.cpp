#include "serialport.h"
#include "ui_serialport.h"


/**
 * @brief SerialPort::SerialPort
 * Abre conexão com porta USB
 */
SerialPort::SerialPort(QWidget *parent) : QDialog(parent), ui(new Ui::SerialPort) {
    qDebug("Criando QDialog Serial Port");
    ui->setupUi(this);
    listaPortas();
    connect(this, SIGNAL(portaSelecionada(QString,qint32)), this, SLOT(abrePorta(QString,qint32)));
}

SerialPort::~SerialPort(){
    delete ui;
}

/**
 * @brief SerialPort::listaPortas
 * Lista portas disponíveis para conexão
 */
void SerialPort::listaPortas(){
    QList<QSerialPortInfo> lista;
    lista.clear();
    ui->comboBoxPortas->clear();

    lista = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &porta, lista)
    {
        ui->comboBoxPortas->addItem(porta.portName());
    }
}

/**
 * @brief SerialPort::on_comboBoxPortas_currentIndexChanged
 * Atualiza informações sobre a porta selecionada
 */
void SerialPort::on_comboBoxPortas_currentIndexChanged(const QString &arg1){
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

/**
 * @brief SerialPort::on_buttonBox_accepted
 * Emite sinal com as informações da porta selecionada para conexão
 */
void SerialPort::on_buttonBox_accepted(){
    QString nome = ui->_labelNome->text();
    qint32 baudrate = ui->comboBoxBaud->currentText().toInt();

    emit portaSelecionada(nome, baudrate);
}

/**
 * @brief SerialPort::abrePorta
 * Cria obejto da classe SeriaArduino conectando a porta selecionada
 */
void SerialPort::abrePorta(QString nome,qint32 baudrate){
    porta = new SerialArduino((char*)nome.toStdString().c_str());
}

/**
 * @brief SerialPort::write
 * Escreve na porta USB
 */
void SerialPort::write(char *data){
    qDebug() << "escreveu? " << porta->WriteData((char*)data, 8);
}
