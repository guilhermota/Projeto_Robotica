#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serialport.h"

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

private:
    Ui::MainWindow *ui;
    SerialPort serial;
};

#endif // MAINWINDOW_H
