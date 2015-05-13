#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QDialog>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QList>
#include <QString>

namespace Ui {
class SerialPort;
}

class SerialPort : public QDialog
{
    Q_OBJECT

public:
    explicit SerialPort(QWidget *parent = 0);
    ~SerialPort();

    qint64 write(const char *data, qint64 max);

    bool isOpen(){return porta.isOpen();}

    void fechaConexao(){porta.close();}

private slots:
    void on_comboBoxPortas_currentIndexChanged(const QString &arg1);

    void on_buttonBox_accepted();

    void abrePorta(QString nome, qint32 baudrate);

signals:
    void portaSelecionada(QString nome, qint32 baudrate);

private:
    Ui::SerialPort *ui;

    void listaPortas();

    QSerialPort porta;

};

#endif // SERIALPORT_H
