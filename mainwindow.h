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

#include "serialport.h"
#include "video.h"

#include "Windows.h"
#include "Psapi.h"
#include "tchar.h"
#include "Pdh.h"

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

private:
    Ui::MainWindow *ui;
    SerialPort serial;
    Video *video;

    DWORDLONG getTotalMemory();
    DWORDLONG getMemoryUsage();
    SIZE_T getMemoryUsageProcess();
    double getUsoCpu();
    double getCpuProcesso();
    void init();

    ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
    int numProcessors;
    HANDLE self;
    SYSTEM_INFO sysInfo;
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;
    /*PDH_HQUERY cpuQuery;
    PDH_HCOUNTER cpuTotal;*/

    DWORDLONG totalMemoria;
    DWORDLONG usoMemoria;
    SIZE_T memoriaProcesso;
    double usoCpu;
    double processoCpu;

    QTimer *timer;

    QLabel *labelUsoCpu, *labelCpuProcesso, *labelUsoMemoria, *labelTotalMemoria, *labelMemoriaProcesso,
            *labelCpuUso, *labelProcessoCpu, *labelUso, *labelTotal, *labelProcesso;
};

#endif // MAINWINDOW_H
