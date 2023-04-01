#ifndef DIALOGO_ANADIRPROGRAMA_H
#define DIALOGO_ANADIRPROGRAMA_H

#include <QDialog>
#include <QCheckBox>

class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;
class QGroupBox;

class dialogo_anadirprograma : public QDialog
{
    Q_OBJECT

public:
    dialogo_anadirprograma(QWidget *parent = nullptr);
    void actualizarProgramasAbiertos();

private:
    QPushButton *aceptarButton;
    QPushButton *cancelarButton;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *programasBox;
    QVBoxLayout *boxLayout;
    QCheckBox *check[50];

private slots:

protected:
    void closeEvent(QCloseEvent *event);
};

#endif // DIALOGO_ANADIRPROGRAMA_H
