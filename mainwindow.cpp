#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //timer update janela principal
    timer = new QTimer(this);

    //conexoes signal slot
    connect(timer, SIGNAL(timeout()), this, SLOT(updateStatus()));

    //criacoes das labels na janela principal
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

    //adicionando labels na status bar
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

    //inicializa variaveis para uso de memoria e cpu
    init();
    //inicia a janela ja com uso de memoria e cpu
    updateStatus();
    //inicia timer update janela principal
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete video;
}

void MainWindow::on_actionSair_triggered()
{
    //mensagem ara confirmar a saida
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

void MainWindow::on_actionAbrir_Camera_triggered()
{
    qDebug("Criando objeto Video");
    video = new Video;

    if(video->open()){
        QMessageBox::information(this, "Ae!", "Camera aberta!");
    } else{
        QMessageBox::warning(this, "Erro", "Nao foi possivel abrir a camera.");
    }
}

void MainWindow::on_actionListar_Portas_triggered()
{    
    qDebug("Abrindo Dialog Listar Portas");
    serial.exec();
}


void MainWindow::on_actionTestar_Conexao_triggered()
{
    if(serial.isOpen()){
        for(int i = 0; i < 10; i++){
            char dados = 'f';

            qDebug("enviando sinal serial");
            qDebug() << serial.write(&dados, 8);

            QThread::msleep(1000);
        }
    } else {
        QMessageBox::warning(this, "Erro!", "Nao ha nenhuma porta aberta!");
    }
}

//TODO Colocar funcoes de cpu e memoria em outro arquivo

/**
 * @brief MainWindow::getTotalMemory
 *
 * Retorna valor total de memoria virtual.
 *
 * @return Memoria virtual total
 */
DWORDLONG MainWindow::getTotalMemory()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;

    return totalVirtualMem;
}

/**
 * @brief MainWindow::getMemoryUsage
 *
 * Retorna uso total da memoria virtual.
 *
 * @return Uso total da memoria virtual
 */
DWORDLONG MainWindow::getMemoryUsage()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

    return virtualMemUsed;
}

/**
 * @brief MainWindow::getMemoryUsageProcess
 *
 * Retorna memoria sendo utiliada por este programa.
 *
 * @return Memoria usada por este programa
 */
SIZE_T MainWindow::getMemoryUsageProcess()
{
    PROCESS_MEMORY_COUNTERS pmc;

    GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.WorkingSetSize;

    return virtualMemUsedByMe;
}

/**
 * @brief MainWindow::init
 *
 * Esta funcao inicializa as variaveis necessarias para
 * pegar o uso da CPU.
 *
 * @see MainWindow::getUsoCpu()
 * @see MainWindow::getCpuProcesso()
 */
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


/**
 * @brief MainWindow::getUsoCpu
 *
 * Esta funcao retorna o uso atual da CPU.
 *
 * @return Uso total atual da CPU.
 */
double MainWindow::getUsoCpu()
{
   /* PDH_FMT_COUNTERVALUE counterVal;

    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    return counterVal.doubleValue;*/
    return 100;
}

/**
 * @brief MainWindow::getCpuProcesso
 *
 * Esta funcao retorna o uso de CPU por este programa.
 *
 * @return Uso da CPU por este processo
 */
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


/**
 * @brief MainWindow::updateStatus
 *
 * Esta funcao atualiza os valores sendo mostrados na janela princiapal.
 */
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


//TODO Criar esta janela

void MainWindow::on_actionSobre_triggered()
{

}
