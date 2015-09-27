#include "sobre.h"
#include "ui_sobre.h"

/**
 * @brief Sobre::Sobre
 * Cria janela de "Sobre"
 */
Sobre::Sobre(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sobre)
{
    ui->setupUi(this);
}

Sobre::~Sobre()
{
    delete ui;
}
