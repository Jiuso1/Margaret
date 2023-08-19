#include "ventanaprincipal.h"
#include "dialogo_anadirprograma.h"
#include <QCloseEvent>
#include <QDebug>
#include <QLabel>
#include <QVBoxLayout>
#include <QDate>
#include <QString>
#include <QLocale>
#include <QHBoxLayout>
#include <QWidget>
#include <QAction>
#include <QApplication>
#include <QMenuBar>
#include <QTableWidget>
#include <QStringList>
#include <QHeaderView>
#include <QMap>
#include <QFile>

VentanaPrincipal::VentanaPrincipal()
{
    reservarMemoria();//Reserva memoria a los atributos punteros distintos a menús y acciones.

    nColumnas = 2;//El número de columnas de la tabla siempre será 2 (columna "Programa" y columna "Tiempo").

    int dia = 0,mes = 0,anio = 0, nProgramasDeseados = 0;

    QTableWidgetItem *item = new QTableWidgetItem;//Reservamos memoria al puntero de la tabla.

    QFont fontProgramas;//Declaramos una fuente para los programas.
    fontProgramas.setBold(true);//Ponemos dicha fuente negrita.

    QMap<QString,unsigned long long int> mapaContador;//Almacena el mapa de contadores del archivo.

    QString fechaEstilo;//Almacena el CSS de la fecha.

    fecha->currentDate().getDate(&anio,&mes,&dia);//dia, mes y anio tienen los valores actuales de la fecha.

    *fechaCadena = "<h1><center>" + transformador->toString(dia) + "/" + transformador->toString(mes) + "/" + transformador->toString(anio) + "</center></h1>";//Convertimos los tipo int a QString con transformador. Seguiremos el formato dia/mes/año. Como queremos la fecha centrada, añadimos HTML para centrarla.
    fechaEstilo = "color: #2d93ca; font: lighter;";//El color de la fuente es azul y la línea es delgada.

    fechaLabel->setText(*fechaCadena);//Le asignamos al label la fecha.
    fechaLabel->setStyleSheet(fechaEstilo);//Le añadimos estilo aL label.

    //Añadimos fechaLabel y tabla al verticalLayout, con cierta separación:
    verticalLayout->addWidget(fechaLabel);
    verticalLayout->addSpacing(10);
    verticalLayout->addWidget(tabla);

    createActions();//Creamos las acciones.
    createMenus();//Creamos los menús.

    centralWidget->setLayout(verticalLayout);//El layout de centralWidget es verticalLayout.
    setCentralWidget(centralWidget);//El widget central de VentanaPrincipal es centralWidget.
    setWindowTitle("Margaret");//El título de la ventana es Margaret.
    setWindowIcon(QIcon("./moon.png"));//El icono de la ventana es la imagen moon.png (almacenada en la carpeta de build).

    mapaContador = leerContador();//Le asignamos a mapaContador el mapa leído del archivo.
    if(mapaContador.size() == 0){
        monitor = new Monitor(this);//Si no hay contadores para este día, no le pasemos contadores al monitor.
    }else{
        monitor = new Monitor(this,mapaContador);//Si sí hay contadores para este día, le pasamos los contadores al monitor.
    }

    monitor->start();//Arranca el monitor en otro hilo.

    //La tabla tendrá las siguientes propiedades:
    tabla->showGrid();//Celdillas visibles.
    tabla->setColumnCount(nColumnas);//Número de columnas igual a nColumnas.
    nProgramasDeseados = monitor->get_nProgramas();//nProgramasDeseados vale el nº de programas deseados a monitorizar.
    tabla->setRowCount(nProgramasDeseados);//Número de filas igual a nProgramasDeseados.
    tabla->setSelectionMode(QAbstractItemView::NoSelection);//Desactivamos la posibilidad de seleccionar en la tabla.
    tabla->setMinimumWidth(sizeHint().width());//Anchura mínima recomendada por Qt.
    tabla->setMinimumHeight(sizeHint().height());//Altura mínima recomendada por Qt.
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//Las columnas se estiran automáticamente para ver el contenido.

    //CSS de la tabla:
    QString tablaEstilo = "background-color: #e0f7ff; font-size: 14pt; font-weight: 350;";//El color de fondo es azul claro, el tamaño de fuente es de 14pt y el peso de la fuente es de 350 (línea delgada).
    tabla->setStyleSheet(tablaEstilo);//Le asignamos a la tabla el CSS.

    //Añadimos tantos contadores como filas tenga la tabla, y los inicializamos a 0:
    for(int i = 0;i < tabla->rowCount();i++){
        contador->append("0");
    }

    //Llenamos el horizontal header de la tabla usando item y lo ocultamos:
    item->setText("Programa");//Le asignamos al texto del item "Programa".
    tabla->setHorizontalHeaderItem(0, item);//Añadimos el item como primero del horizontal header.
    item = new QTableWidgetItem;//Creamos otro QTableWidgetItem al que apuntaremos con item. Este tendrá otro texto.
    item->setText("Tiempo");//Le asignamos al texto del item "Tiempo".
    tabla->setHorizontalHeaderItem(1, item);//Añadimos el item como segundo del horizontal header.
    tabla->verticalHeader()->hide();//Ocultamos el vertical header.

    //Llenamos la columna de procesos:
    for(int i = 0;i < tabla->rowCount();i++){//Por cada fila de la tabla:
        item = new QTableWidgetItem;//Creamos un nuevo QTableWidgetItem al que apuntamos.
        item->setText(programa->at(i));//El texto del item es el programa deseado i.
        item->setFlags(Qt::ItemIsEnabled);//Activamos en el item esta flag. Así se puede seleccionar el item pero no alterar su contenido.
        item->setFont(fontProgramas);//La fuente del item es fontProgramas (negrita).
        tabla->setItem(i, 0, item);//Ponemos el item en la fila correspondiente y en la columna 0.
    }

    tabla->setCurrentCell(-1,-1);//Ponemos la selección por defecto a una celda que no se ve.
}

//SLOT que abre el diálogo de añadir programas.
void VentanaPrincipal::anadirPrograma(){
    dialogoAdd = new dialogo_anadirprograma(this);//El puntero apuntará al diálogo.
    //VentanaPrincipal será padre del diálogo al pasarle this.
    dialogoAdd->show();//Mostramos el diálogo.
}

//SLOT que abre el diálogo de eliminar programas.
void VentanaPrincipal::eliminarPrograma(){
    dialogoRemove = new dialogo_eliminarprograma(this);//El puntero apuntará al diálogo.
    //VentanaPrincipal será padre del diálogo al pasarle this.
    dialogoRemove->show();//Mostramos el diálogo.
}

//Reserva memoria para las acciones, les asigna texto y realiza la conexión SIGNAL-SLOT.
void VentanaPrincipal::createActions(){
    //Creamos las siguientes acciones:
    anadirProgramaAction = new QAction("&Añadir programa",this);//Acción "Añadir programa" con atajo de teclado A.
    eliminarProgramaAction = new QAction("&Eliminar programa",this);//Acción "Eliminar programa" con atajo de teclado E.
    aboutQtAction = new QAction("Acerca de &Qt",this);//Acción "Acerca de Qt" con atajo de teclado Q.
    ayudaMargaretAction = new QAction("&Documentación de Margaret",this);//Acción "Documentación de Margaret" con atajo de teclado D.
    ayudaMargaretAction->setVisible(false);//Ocultamos la acción "Documentación de Margaret" porque no tenemos lista la documentación todavía.

    //Cuando una acción se pulsa, emite la señal triggered. Sabiendo esto
    //realizamos la conexión SIGNAL-SLOT:
    connect(anadirProgramaAction,SIGNAL(triggered()),this,SLOT(anadirPrograma()));//Al pulsar añadirProgramaAction se ejecuta el SLOT anadirPrograma.
    connect(eliminarProgramaAction,SIGNAL(triggered()),this,SLOT(eliminarPrograma()));//Al pulsar eliminarProgramaAction se ejecuta el SLOT eliminarPrograma.
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));//Al pulsar aboutQtAction se ejecuta el SLOT aboutQt. Como este SLOT se encuentra en qApp y no en VentanaPrincipal, le pasamos qApp y no this.
}

//Crea los menús añadiéndolos a la menuBar de la ventana principal.
void VentanaPrincipal::createMenus(){
    programasMenu = menuBar()->addMenu(tr("&Programas"));//Crea el menú "Programas" con atajo de teclado P y lo añade a la menuBar.
    //programasMenu apuntará al menú "Programas".
    //Añadimos las acciones "Añadir programa" y "Eliminar programa" al menú "Programas":
    programasMenu->addAction(anadirProgramaAction);
    programasMenu->addAction(eliminarProgramaAction);
    ayudaMenu = menuBar()->addMenu(tr("&Ayuda"));//Crea el menú "Ayuda" con atajo de teclado A y lo añade a la menuBar.
    //Añadimos las acciones "Acerca de Qt" y "Documentación de Margaret" al menú "Ayuda":
    ayudaMenu->addAction(ayudaMargaretAction);
    ayudaMenu->addAction(aboutQtAction);
}

//Cambia la fecha de VentanaPrincipal por la fecha dada por parámetro.
void VentanaPrincipal::setFecha(QDate *fecha){
    int dia = 0,mes = 0,anio = 0;//Declaración de variables.
    fecha->getDate(&anio,&mes,&dia);//dia, mes y anio tendrán los valores enteros de la fecha pasada por parámetro.
    *fechaCadena = "<h1><center>" + transformador->toString(dia) + "/" + transformador->toString(mes) + "/" + transformador->toString(anio) + "</center></h1>";//fechaCadena se actualiza con los valores enteros y usando HTML.
    fechaLabel->setText(*fechaCadena);//Actualiza el label con la cadena actual.
}

//Cambia la lista de programas deseados de VentanaPrincipal por la dada por parámetro.
void VentanaPrincipal::setPrograma(const QStringList &programa){
    *(this->programa) = programa;//Al contenido del puntero programa se le asigna el parámetro.
}

//Cambia la lista de contadores de VentanaPrincipal por la dada por parámetro y actualiza la tabla.
void VentanaPrincipal::setContador(const QStringList &contador){
    *(this->contador) = contador;//Al contenido del puntero contador se le asigna el parámetro.
    for(int i = 0;i < tabla->rowCount();i++){//Por cada fila de la tabla:
        QTableWidgetItem *item = new QTableWidgetItem;//Se crea un item.
        item->setText(contador.at(i));//El texto del item es el contador. Tres posibles formatos: SS s|MM min SS s|HH h MM min SS s
        item->setFlags(Qt::ItemIsEnabled);//Activamos en el item esta flag. Así se puede seleccionar el item pero no alterar su contenido.
        tabla->setItem(i, 1, item);//En la columna 1 de cada fila se instala un item con el contador del proceso.
    }
}

//Guarda los contadores de todos los procesos de la tabla en un archivo.
bool VentanaPrincipal::guardarContador(){
    return monitor->guardarContador();//El monitor guarda los contadores.
}

//Lee los contadores del archivo y retorna el mapa<Proceso,Contador>.
QMap<QString,unsigned long long int> VentanaPrincipal::leerContador(){
    QMap<QString,unsigned long long int> mapaContador;//Mapea cada programa con su contador.

    QDate *fecha = new QDate;//Contiene la fecha actual usando currentDate().

    int diaArchivo = 0, mesArchivo = 0, anhoArchivo = 0, dia = 0, mes = 0, anho = 0;//Creamos dos variables por unidad de tiempo: una para el archivo y otra actual.

    QFile archivoLectura("contadores.dat");//archivoLectura se vincula al archivo "contadores.dat".
    QDataStream entrada(&archivoLectura);//entrada se vincula a archivoLectura.
    if(!archivoLectura.open(QIODevice::ReadOnly)){//Abrimos el archivo en solo lectura. Si falla su lectura:
        //Escribimos por consola los errores:
        qDebug()<<"No se pudo abrir el archivo";
        qDebug()<<qPrintable(archivoLectura.errorString());
    }else{//Si no falla la lectura:
        entrada.setVersion(QDataStream::Qt_6_4);//Leemos el archivo según la versión de Qt 6.4.
        //El formato del archivo es: |dia|mes|año|mapa|
        entrada>>diaArchivo>>mesArchivo>>anhoArchivo;//Leemos las tres variables escritas en el archivo.
        fecha->currentDate().getDate(&anho, &mes, &dia);//Asignamos a las variables pasadas por parámetro el valor correspondiente de la fecha actual.
        if(diaArchivo == dia && mesArchivo == mes && anhoArchivo == anho){//Si la fecha del archivo coincide con la fecha actual, rescatamos los contadores.
            entrada>>mapaContador;//Guardamos el mapa del archivo en mapaContador.
        }
    }

    delete fecha;//Eliminamos la fecha.

    return mapaContador;//Retornamos el mapa del archivo.
}

//Se ejecuta al cerrarse la aplicación. Guarda el mapa de contadores en el archivo.
void VentanaPrincipal::closeEvent(QCloseEvent *event){
    guardarContador();//Guarda el mapa de contadores en el archivo.
}

//Reserva memoria a los punteros atributos de VentanaPrincipal distintos a menús y acciones.
void VentanaPrincipal::reservarMemoria(){
    //Reservamos memoria a los punteros:
    verticalLayout = new QVBoxLayout;
    fechaLabel = new QLabel;
    fecha = new QDate;
    fechaCadena = new QString;
    transformador = new QLocale;
    centralWidget = new QWidget;
    dialogoAdd = nullptr;//dialogoAdd igual nunca se abre, no le reservaremos memoria por defecto, lo inicializamos a nullptr.
    tabla = new QTableWidget;
    programa = new QStringList;
    contador = new QStringList;
}
