#ifndef DIALOGO_ELIMINARPROGRAMA_H
#define DIALOGO_ELIMINARPROGRAMA_H

#include <QDialog>
#include <QCheckBox>

class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;
class QGroupBox;

class dialogo_eliminarprograma : public QDialog
{
    Q_OBJECT
public:
    dialogo_eliminarprograma(QWidget *parent = nullptr);
    void actualizarProgramasArchivo();

private:
    QPushButton *aceptarButton;
    QPushButton *cancelarButton;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *programasBox;
    QVBoxLayout *boxLayout;
    QCheckBox *check[50];
    int nProgramas;//Número de programas que se mostrarán en los checkbox.
    int nMaxProgramas;

private slots:
    void aceptar();
};

#endif // DIALOGO_ELIMINARPROGRAMA_H
