/*
* Projeto Robotica
* School projet for my Robotics class. This program provides face
* recognition and communication with serial ports.
*
* mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMessageBox>
#include <qDebug>
#include <QLabel>

#include "serialport.h"

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

    void on_actionAbrir_Video_triggered();

    void on_actionListar_Portas_triggered();

    void on_actionTestar_Conexao_triggered();

    void updateStatus();

    void on_actionSobre_triggered();

private:
    Ui::MainWindow *ui;
    SerialPort serial;

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
