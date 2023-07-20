#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include "monitor.h"
#include "dialogo_anadirprograma.h"
#include "dialogo_eliminarprograma.h"

class QCloseEvent;
class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QDate;
class QLocale;
class QString;
class QWidget;
class QAction;
class QMenu;
class QTableWidget;

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT

public:
    VentanaPrincipal();
    void createActions();
    void createToolBars();
    void createMenus();
    void setlistaProgramas(QString lProgramas);//Lo usaremos en Monitor.
    void setcontadoresProgramas(QString cProgramas);
    void setFecha(QDate *fecha);//El monitor continuamente revisará si la fecha cambia. Si la fecha cambia, nos asignará una nueva fecha con este método.
    void setPrograma(const QStringList &programa);//Se asigna a la lista de programas deseados a monitorizar la lista de programas leída desde el archivo por el monitor.
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void anadirPrograma();//Se llamará a este slot cuando pulsemos en el botón indicado.
    void eliminarPrograma();//Abrirá el diálogo de eliminar programa.

private:
    QLabel *fechaLabel;//Label que almacena la fecha.
    QVBoxLayout *verticalLayout;//Layout en el que incluiremos los diferentes widgets.
    QDate *fecha;//Con este objeto obtendremos la fecha actual del sistema usando el método currentDate().
    QLocale *transformador;//Nos permitirá pasar de int a QString.
    QString *fechaCadena;//Almacenará la cadena que le pasaremos a fechaLabel para que la muestre.
    QString *fechaEstilo;
    QLabel *listaProgramas;//Almacenará el conjunto de programas a monitorizar junto con sus contadores respectivos y junto con el icono de los programas. Todo ello se realizará con el formato HTML.
    QHBoxLayout *horizontalLayout;//Layout en el que incluiremos la lista de programas junto con sus respectivos contadores.
    QLabel *contadoresProgramas;//Almacenará el conjunto de contadores asociados a listaProgramas.
    QWidget *centralWidget;//Este widget lo pondremos en el centro de nuestra ventana QMainWindow. Este contendrá el layout principal.
    QAction *anadirProgramaAction;//Este QAction será añadido a una QToolBar. Será responsable de abrir la ventana diálogo encargada de solicitar la información del nuevo programa en la lista.
    QAction *eliminarProgramaAction;//Homólogo al QAction de arriba, pero para abrir la ventana diálogo encargada de eliminar el programa deseado.
    QAction *aboutQtAction;//QAction encargado de abrir la información sobre la versión de Qt empleada.
    QAction *ayudaMargaretAction;
    QMenu *programasMenu;
    QMenu *ayudaMenu;
    dialogo_anadirprograma *dialogoAdd;
    dialogo_eliminarprograma *dialogoRemove;
    Monitor *monitor;//Es el encargado de hablar con el sistema operativo y de calcular los tiempos.
    QTableWidget *tabla;
    int i;//Se incrementará cada vez que se añada una celda de un programa deseado.
    int nColumnas;//Almacena el número de columnas.
    QStringList *programa;//Almacena los programas deseados a monitorizar.
};

#endif // VENTANAPRINCIPAL_H
