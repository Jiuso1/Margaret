#ifndef MONITOR_H
#define MONITOR_H

#include <QThread>
#include <vector>

#include <QMap>//Incluimos la clase porque tiene plantillas y tenemos problemas para hacer su declaración previa.

//Declaraciones previas:
class QString;
class VentanaPrincipal;
class QLocale;
class QDate;

class Monitor : public QThread  //La clase Monitor es clase hija de QThread. El monitor es un hilo independiente a la respuesta de la interfaz.
{
private:
    QStringList programaArchivo;//Almacena el conjunto de programas a monitorizar. Este conjunto se almacena en programas.dat.
    VentanaPrincipal *vPrincipal;//Ventana principal. El monitor poblará y actualizará la ventana principal.
    QLocale *transformador;//Permite transformar int a QString usando el método toString().
    int nMaxProgramas;//Número máximo de programas que se pueden monitorizar.
    int nProgramas;//Número de programas deseados a monitorizar.
    std::uint64_t tiempoPrograma[50];//En vez de 50 debería ser nMaxProgramas.
    //tiempoPrograma[i] almacena el tiempo de ejecución en segundos del programa i.
    int espera;//Tiempo de espera del monitor en segundos. Cuanto menor sea, mayor precisión de cálculo pero mayor consumo de recursos.
    QDate *fecha;//Fecha actual.
    QMap<QString,std::uint64_t> mapaContador;//Es el mapa que nos ha pasado la ventana principal. Mapa<programa,contador>.

public:
    Monitor(VentanaPrincipal *v = nullptr);//Constructor
    Monitor(VentanaPrincipal *v, const QMap<QString,std::uint64_t> &mapaContador);//Constructor que pasa los contadores. Este se ejecuta cuando nos encontramos en el mismo día que en la iteración anterior.
    int get_nProgramas();//Retorna el número de programas deseados a monitorizar.
    bool guardarContador();//Guarda el mapa de contadores en un archivo, y retorna true si lo ha conseguido.

protected:
    void run();//Método que se ejecuta concurrentemente con la respuesta de la GUI. Cuenta el tiempo de las aplicaciones.
};

#endif // MONITOR_H
