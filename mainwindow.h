#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include <qDebug>
#include <QLabel>
#include <QThread>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QInputDialog>
#include <QDir>

#include "serialport.h"
#include "video.h"
#include "medidorperformance.h"
#include "database.h"
#include "sobre.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionSair_triggered();

    void on_actionAbrir_Camera_triggered();

    void on_actionListar_Portas_triggered();

    void on_actionTestar_Conexao_triggered();

    void updateStatus();

    void on_actionSobre_triggered();

    void on_actionFechar_Conexao_triggered();

    void on_actionFechar_Camera_triggered();

    void on_actionTestar_Camera_triggered();

    void imageReceived(QImage image);

    void on_actionCarregar_triggered();

    void databaseInputs(QString *host, QString *db, QString *login, QString *senha);

private:
    Ui::MainWindow *ui;
    SerialPort serial;
    Video *video;
    MedidorPerformance mp;
    DWORDLONG totalMemoria;
    DWORDLONG usoMemoria;
    SIZE_T memoriaProcesso;
    double usoCpu;
    double processoCpu;

    QTimer *timer;

    QLabel *labelUsoCpu, *labelCpuProcesso, *labelUsoMemoria, *labelTotalMemoria, *labelMemoriaProcesso,
            *labelCpuUso, *labelProcessoCpu, *labelUso, *labelTotal, *labelProcesso;

    std::vector<std::string> paths, labels;
};

#endif // MAINWINDOW_H
