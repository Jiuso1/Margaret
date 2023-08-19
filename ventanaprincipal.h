#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include "monitor.h"
#include "dialogo_anadirprograma.h"
#include "dialogo_eliminarprograma.h"

//Declaraciones previas:
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

class VentanaPrincipal : public QMainWindow //VentanaPrincipal es una ventana principal (hereda de QMainWindow).
{
    Q_OBJECT //Tenemos SIGNAL y SLOT, es necesaria la macro Q_OBJECT.

public:
    VentanaPrincipal();//Constructor.
    void createActions();//Crea las acciones de la aplicación.
    void createMenus();//Crea los menús, añadiéndoles acciones.
    void setFecha(QDate *fecha);//Asigna a fechaLabel el QDate pasado por parámetro, siguiendo el formato Día/Mes/Año.
    void setPrograma(const QStringList &programa);//Asigna al QStringList programa el QStringList pasado por parámetro.
    void setContador(const QStringList &contador);//Asigna al QStringList contador el QStringList pasado por parámetro.
    bool guardarContador();//Guarda los contadores en un archivo. Para ello llama a un método del monitor.
    QMap<QString,unsigned long long int> leerContador();//Lee un mapa<programa,contador> desde un archivo y lo retorna. Si el día del archivo es diferente al día actual retorna un mapa vacío.
    void reservarMemoria();//Reserva memoria para los atributos punteros distintos a menús y acciones.

protected:
    void closeEvent(QCloseEvent *event);//Se ejecuta al cerrar la aplicación. Guarda los contadores en un archivo.

private slots:
    void anadirPrograma();//Abre el diálogo dialogo_anadirprograma.
    void eliminarPrograma();//Abre el diálogo dialogo_eliminarprograma.
    void aboutMargaret();//Abre una ventana de información sobre Margaret.

private:
    QLabel *fechaLabel;//Muestra la fecha actual.
    QVBoxLayout *verticalLayout;//Dispone verticalmente a fechaLabel y a tabla.
    QDate *fecha;//Almacena la fecha actual del sistema usando el método currentDate().
    QLocale *transformador;//Permite transformar int a QString usando el método toString().
    QString *fechaCadena;//Almacena la fecha actual.
    QWidget *centralWidget;//Es central y contiene el layout principal.
    QAction *anadirProgramaAction;//Está enlazado con el slot anadirPrograma().
    QAction *eliminarProgramaAction;//Está enlazado con el slot eliminarPrograma().
    QAction *aboutQtAction;//Está enlazado con el slot aboutQt().
    QAction *ayudaMargaretAction;//PENDIENTE POR HACER.
    QAction *aboutMargaretAction;//Está enlazado con el slot aboutMargaret().
    QMenu *programasMenu;//Contiene anadirProgramaAction y eliminarProgramaAction.
    QMenu *ayudaMenu;//Contiene aboutQtAction y ayudaMargaretAction.
    dialogo_anadirprograma *dialogoAdd;//Pregunta qué programas añadir.
    dialogo_eliminarprograma *dialogoRemove;//Pregunta qué programas eliminar.
    Monitor *monitor;//Habla con el sistema operativo y calcula los tiempos.
    QTableWidget *tabla;//Muestra los datos.
    int nColumnas;//Almacena el número de columnas.
    QStringList *programa;//Almacena los programas deseados a monitorizar.
    QStringList *contador;//Almacena los contadores respectivos a cada programa deseado a monitorizar.
};

#endif // VENTANAPRINCIPAL_H
