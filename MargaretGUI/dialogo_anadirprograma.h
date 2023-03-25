#ifndef DIALOGO_ANADIRPROGRAMA_H
#define DIALOGO_ANADIRPROGRAMA_H

#include <QDialog>

class QLabel;
class QPushButton;

class dialogo_anadirprograma : public QDialog
{
    Q_OBJECT

public:
    dialogo_anadirprograma();

private:
    QLabel *anadirLabel;
    QPushButton *aceptarButton;
    QPushButton *cancelarButton;
};

#endif // DIALOGO_ANADIRPROGRAMA_H
