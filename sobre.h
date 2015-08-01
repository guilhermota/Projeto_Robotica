//TODO ACHAR O PROBLEMA DESTA CARAIBA :@

#ifndef SOBRE_H
#define SOBRE_H

#include <QDialog>

namespace Ui {
class Sobre;
}

class Sobre : public QDialog
{
    Q_OBJECT

public:
    explicit Sobre(QWidget *parent = 0);
    ~Sobre();

private:
    Ui::Sobre *ui;
};

#endif // SOBRE_H
