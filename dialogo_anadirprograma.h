#ifndef DIALOGO_ANADIRPROGRAMA_H
#define DIALOGO_ANADIRPROGRAMA_H

#include <QDialog>
#include <QCheckBox>

//Declaraciones previas:
class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;
class QGroupBox;

class dialogo_anadirprograma : public QDialog //dialogo_anadirprograma hereda de QDialog.
{
    Q_OBJECT //Tenemos SIGNAL y SLOT, es necesaria la macro Q_OBJECT.

public:
    dialogo_anadirprograma(QWidget *parent = nullptr);//Constructor.
    void actualizarProgramasAbiertos();//Añade o refresca los programas al diálogo.

private:
    QPushButton *aceptarButton;//Botón que al ser pulsado añade los programas seleccionados en el diálogo.
    QPushButton *cancelarButton;//Botón que al ser pulsado cierra el diálogo.
    QVBoxLayout *verticalLayout;//Layout que dispone verticalmente a la lista de programas y a los botones del diálogo.
    QHBoxLayout *horizontalLayout;//Layout que dispone horizontalmente a los botones del layout entre sí.
    QGroupBox *programasBox;//Grupo de nombres de programas.
    QVBoxLayout *boxLayout;//Layout donde se almacenan los nombres de programas. Este layout se vinculará con programasBox.
    QCheckBox *check[50];//Array de 50 checkboxes. Por cambiar.
    int nProgramas;//Número de programas que se mostrarán en los checkbox.
    int nMaxProgramas;//Número máximo de programas que se pueden mostrar en el checkbox.

private slots:
    void aceptar();//Se ejecuta cuando el usuario pulsa el botón aceptarButton.
};

#endif // DIALOGO_ANADIRPROGRAMA_H
