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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(updateStatus()));

    labelUsoMemoria = new QLabel(this);
    labelTotalMemoria = new QLabel(this);
    labelMemoriaProcesso = new QLabel(this);
    labelUsoCpu = new QLabel(this);
    labelProcessoCpu = new QLabel(this);
    labelTotal = new QLabel("Uso Total: ", this);
    labelUso = new QLabel("Uso Atual: ", this);
    labelProcesso = new QLabel("Memoria Processo: ", this);
    labelCpuUso = new QLabel("Uso CPU: ", this);
    labelCpuProcesso = new QLabel("Processo CPU: ", this);

    ui->statusBar->addPermanentWidget(labelCpuUso);
        ui->statusBar->addPermanentWidget(labelUsoCpu);
    ui->statusBar->addPermanentWidget(labelCpuProcesso);
        ui->statusBar->addPermanentWidget(labelProcessoCpu);

    ui->statusBar->addPermanentWidget(labelTotal);
        ui->statusBar->addPermanentWidget(labelTotalMemoria);
    ui->statusBar->addPermanentWidget(labelUso);
        ui->statusBar->addPermanentWidget(labelUsoMemoria);
    ui->statusBar->addPermanentWidget(labelProcesso);
        ui->statusBar->addPermanentWidget(labelMemoriaProcesso);
    init();
    updateStatus();
    timer->start(1000);
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

DWORDLONG MainWindow::getTotalMemory()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;

    return totalVirtualMem;
}

DWORDLONG MainWindow::getMemoryUsage()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

    return virtualMemUsed;
}

SIZE_T MainWindow::getMemoryUsageProcess()
{
    PROCESS_MEMORY_COUNTERS pmc;

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.WorkingSetSize;

    return virtualMemUsedByMe;
}

void MainWindow::init()
{
    GetSystemInfo(&sysInfo);
    numProcessors = sysInfo.dwNumberOfProcessors;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&lastCPU, &ftime, sizeof(FILETIME));

    self = GetCurrentProcess();
    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
    memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));

    /*PdhOpenQuery(NULL, NULL, &cpuQuery);
    PdhAddCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    PdhCollectQueryData(cpuQuery);*/
}

double MainWindow::getUsoCpu()
{
   /* PDH_FMT_COUNTERVALUE counterVal;

    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    return counterVal.doubleValue;*/
    return 100;
}

double MainWindow::getCpuProcesso()
{
    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (sys.QuadPart - lastSysCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart);
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return percent * 100;
}

void MainWindow::updateStatus()
{
    totalMemoria = getTotalMemory();
    usoMemoria = getMemoryUsage();
    memoriaProcesso = getMemoryUsageProcess();
    usoCpu = getUsoCpu();
    processoCpu = getCpuProcesso();

    double pctUso = usoMemoria * 100 / totalMemoria;
    double pctProcesso = memoriaProcesso * 100 / totalMemoria;

    labelTotalMemoria->setText(QString::number(totalMemoria) );
    labelUsoMemoria->setText(QString::number(pctUso) + "%");
    labelMemoriaProcesso->setText(QString::number(pctProcesso) + "%");
    labelUsoCpu->setText(QString::number(usoCpu) + "%");
    labelProcessoCpu->setText(QString::number(processoCpu) + "%");

    ui->statusBar->update();
}
