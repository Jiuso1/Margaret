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

VentanaPrincipal::VentanaPrincipal()
{
    verticalLayout = new QVBoxLayout;
    fechaLabel = new QLabel;
    fecha = new QDate;
    fechaCadena = new QString;
    fechaEstilo = new QString;
    transformador = new QLocale;
    centralWidget = new QWidget;
    dialogoAdd = nullptr;//Importantísimo inicializarlo a nullptr.
    tabla = new QTableWidget;
    i = 0;
    nColumnas = 2;
    programa = new QStringList;
    contador = new QStringList;

    int dia = 0,mes = 0,anio = 0, nProgramasDeseados = 0;

    QTableWidgetItem *item = new QTableWidgetItem;

    QFont fontProgramas;//Declaramos una fuente para los programas.
    fontProgramas.setBold(true);//Ponemos la fuente negrita.

    fecha->currentDate().getDate(&anio,&mes,&dia);

    *fechaCadena = "<h1><center>" + transformador->toString(dia) + "/" + transformador->toString(mes) + "/" + transformador->toString(anio) + "</center></h1>";
    *fechaEstilo = "color: #2d93ca; font: lighter;";

    fechaLabel->setText(*fechaCadena);
    fechaLabel->setStyleSheet(*fechaEstilo);

    verticalLayout->addWidget(fechaLabel);
    verticalLayout->addSpacing(10);
    verticalLayout->addWidget(tabla);

    createActions();
    createMenus();

    centralWidget->setLayout(verticalLayout);
    setCentralWidget(centralWidget);
    setWindowTitle("Margaret");
    setWindowIcon(QIcon("./moon.png"));

    monitor = new Monitor(this);
    monitor->start();

    tabla->showGrid();
    tabla->setColumnCount(nColumnas);
    nProgramasDeseados = monitor->get_nProgramas();
    tabla->setRowCount(nProgramasDeseados);//El número de filas de la tabla será igual al nº de programas deseados a monitorizar.
    tabla->setSelectionMode(QAbstractItemView::NoSelection);//Desactivamos la posibilidad de seleccionar en la tabla.
    tabla->setMinimumWidth(sizeHint().width());//Pondremos un ancho mínimo recomendado por Qt.
    tabla->setMinimumHeight(sizeHint().height());
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//Las columnas se estirarán automáticamente para cubrir el contenido.

    //CSS de la tabla:
    QString tablaEstilo = "background-color: #e0f7ff; font-size: 14pt; font-weight: 350;";
    tabla->setStyleSheet(tablaEstilo);

    //Añadimos e inicializamos las cadenas a 0:
    for(int i = 0;i < tabla->rowCount();i++){
        contador->append("0");
    }

    //Llenamos el horizontal header:
    item->setText("Programa");//item está inicializado.
    tabla->setHorizontalHeaderItem(0, item);
    item = new QTableWidgetItem;//Creamos otro QTableWidgetItem al que apuntaremos con item. Este tendrá otro texto.
    item->setText("Tiempo");
    tabla->setHorizontalHeaderItem(1, item);

    //Ocultamos el vertical header:
    tabla->verticalHeader()->hide();

    //Llenamos la columna de procesos:
    for(int i = 0;i < tabla->rowCount();i++){
        item = new QTableWidgetItem;
        item->setText(programa->at(i));
        item->setFlags(Qt::ItemIsEnabled);//Con esta flag se puede seleccionar el item pero no alterarlo.
        item->setFont(fontProgramas);
        tabla->setItem(i, 0, item);//Por cada fila en la columna 0 pondremos un item con el nombre de un proceso.
    }
}

//Abriremos el diálogo de añadir programas.
void VentanaPrincipal::anadirPrograma(){
    dialogoAdd = new dialogo_anadirprograma(this);
    dialogoAdd->show();
}

void VentanaPrincipal::eliminarPrograma(){
    dialogoRemove = new dialogo_eliminarprograma(this);
    dialogoRemove->show();
}

void VentanaPrincipal::createActions(){
    anadirProgramaAction = new QAction("&Añadir programa",this);
    eliminarProgramaAction = new QAction("&Eliminar programa",this);
    aboutQtAction = new QAction("Acerca de &Qt",this);
    ayudaMargaretAction = new QAction("&Documentación de Margaret",this);

    connect(anadirProgramaAction,SIGNAL(triggered()),this,SLOT(anadirPrograma()));
    connect(eliminarProgramaAction,SIGNAL(triggered()),this,SLOT(eliminarPrograma()));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void VentanaPrincipal::createMenus(){
    programasMenu = menuBar()->addMenu(tr("&Programas"));
    programasMenu->addAction(anadirProgramaAction);
    programasMenu->addAction(eliminarProgramaAction);
    ayudaMenu = menuBar()->addMenu(tr("&Ayuda"));
    ayudaMenu->addAction(ayudaMargaretAction);
    ayudaMenu->addAction(aboutQtAction);
}

void VentanaPrincipal::setFecha(QDate *fecha){
    int dia = 0,mes = 0,anio = 0;
    fecha->currentDate().getDate(&anio,&mes,&dia);
    *fechaCadena = "<center><font color='black' size=5 face=arial><b>" + transformador->toString(dia) + "/" + transformador->toString(mes) + "/" + transformador->toString(anio) + "</b></font></center>";
}

void VentanaPrincipal::setPrograma(const QStringList &programa){
    *(this->programa) = programa;
}

void VentanaPrincipal::setContador(const QStringList &contador){
    *(this->contador) = contador;
    for(int i = 0;i < tabla->rowCount();i++){
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(contador.at(i));
        item->setFlags(Qt::ItemIsEnabled);//Con esta flag se puede seleccionar el item pero no alterarlo.
        tabla->setItem(i, 1, item);//Por cada fila en la columna 1 pondremos un item con el contador del proceso.
    }
}

void VentanaPrincipal::closeEvent(QCloseEvent *event){
    //Guardamos los contadores:

}
