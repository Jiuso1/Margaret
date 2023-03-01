#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

//#include <QMainWindow>//Vamos a usar QDialog porque es el que nos funciona.
#include <QDialog>

class QCloseEvent;
class QLabel;
class QPushButton;
class QVBoxLayout;
class QDate;
class QLocale;
class QString;


class VentanaPrincipal : public QDialog
{
    Q_OBJECT

public:
    VentanaPrincipal();

protected:
    void closeEvent(QCloseEvent *event);

private slots:


private:
    QLabel *fechaLabel;//Label que almacena la fecha. Formato: "A día "<<dia<<" de "<<mes.
    QPushButton *anadirPr;//Botón que abre el diálogo para añadir un programa.
    QPushButton *eliminarPr;//Botón que abre el diálogo para eliminar un programa.
    QVBoxLayout *verticalLayout;//Layout en el que incluiremos los diferentes widgets.
    QDate *fecha;
    QLocale *transformador;
    QString *fechaCadena;
};

#endif // VENTANAPRINCIPAL_H
