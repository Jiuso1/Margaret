#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include "dialogo_anadirprograma.h"

class QCloseEvent;
class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QDate;
class QLocale;
class QString;
class QWidget;

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT

public:
    VentanaPrincipal();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void anadirPrograma();//Se llamará a este slot cuando pulsemos en el botón anadirPr.

private:
    QLabel *fechaLabel;//Label que almacena la fecha. Formato: "A día "<<dia<<" de "<<mes.
    QPushButton *anadirPr;//Botón que abre el diálogo para añadir un programa.
    QPushButton *eliminarPr;//Botón que abre el diálogo para eliminar un programa.
    QVBoxLayout *verticalLayout;//Layout en el que incluiremos los diferentes widgets.
    QDate *fecha;//Con este objeto obtendremos la fecha actual del sistema usando el método currentDate().
    QLocale *transformador;//Nos permitirá pasar de int a QString.
    QString *fechaCadena;//Almacenará la cadena que le pasaremos a fechaLabel para que la muestre.
    QLabel *listaProgramas;//Almacenará el conjunto de programas a monitorizar junto con sus contadores respectivos y junto con el icono de los programas. Todo ello se realizará con el formato HTML.
    dialogo_anadirprograma *dialogoAdd;
    QHBoxLayout *horizontalLayout;//Layout en el que incluiremos la lista de programas junto con sus respectivos contadores.
    QLabel *contadoresProgramas;//Almacenará el conjunto de contadores asociados a listaProgramas.
    QWidget *centralWidget;//Este widget lo pondremos en el centro de nuestra ventana QMainWindow.
};

#endif // VENTANAPRINCIPAL_H
