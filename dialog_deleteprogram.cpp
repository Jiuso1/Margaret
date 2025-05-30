#include "dialog_deleteprogram.h"
#include "engine.h"//Always include only .h.
#include "windowinfo.h"

#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QCheckBox>
#include <string>
#include <windows.h>
#include <QString>
#include <QFile>

dialog_deleteprogram::dialog_deleteprogram(QWidget *parent) : QDialog(parent)
{
    acceptButton = new QPushButton(tr("&Accept"));
    cancelButton = new QPushButton(tr("&Cancel"));
    verticalLayout = new QVBoxLayout;
    horizontalLayout = new QHBoxLayout;
    programsBox = new QGroupBox;
    boxLayout = new QVBoxLayout;
    nMaxPrograms = 50;//Let's say 50 for example.
    nPrograms = 0;//We initialize nPrograms to 0.
    for(int i = 0;i < nMaxPrograms;i++){
        check[i] = nullptr;//It is very important to initialize the pointers to nullptr, otherwise they will not be initialized and methods will be executed without having reserved memory despite the if(pointer == nullptr) check.
    }

    updateProgramsFile();

    programsBox->setTitle(tr("Select the programs you want to delete"));
    programsBox->setLayout(boxLayout);

    horizontalLayout->addWidget(cancelButton);
    horizontalLayout->addWidget(acceptButton);
    verticalLayout->addWidget(programsBox);
    verticalLayout->addLayout(horizontalLayout);

    setLayout(verticalLayout);

    setAttribute(Qt::WA_DeleteOnClose);//As you set this attribute to the dialog, when it is closed it will be destroyed.

    connect(acceptButton,SIGNAL(clicked()),this,SLOT(accept()));
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(close()));

    setWindowTitle(tr("Delete program"));
}
void dialog_deleteprogram::updateProgramsFile(){
    int i = 0;
    bool emptyString = false;//If we find an empty string, it will be true.
    QString pr;
    QList<QString> program;
    QFile fileRead("programs.dat");
    if(!fileRead.open(QIODevice::ReadOnly)){
        qDebug()<<"dialog_deleteprogram: Could not open the file";
        qDebug()<<qPrintable(fileRead.errorString());
        return;
    }
    QDataStream input(&fileRead);
    input.setVersion(QDataStream::Qt_6_7);
    while(!emptyString && i < nMaxPrograms){
        input>>pr;
        if(pr == ""){//If string is empty.
            emptyString = true;
        }else{//We will only push the string if it is not empty.
            program.push_back(pr);
            i++;
        }
    }

    nPrograms = program.size();//nPrograms will be worth the number of program elements.

    QString buttonName[nPrograms];//This array will store the names to be assigned to the checkable buttons in the dialog.

    for(int i = 0;i < nPrograms;i++){
        buttonName[i] = program[i];//At each button we will assign the name of the program.
    }

    //We will create as many checkboxes as there are programs:
    for(int i = 0;i < nPrograms;i++){//For each program, I will create its own check button and assign it to the layout.
        if(check[i] == nullptr){//We only reserve memory if we haven't done it before. We reserve it with the specified text.
            check[i] = new QCheckBox(buttonName[i]);
            boxLayout->addWidget(check[i]);
        }else{//If there is already reserved memory, we simply change the text.
            check[i]->setText(buttonName[i]);
        }
    }
    for(int i = nPrograms;i < nMaxPrograms;i++){//This for is to remove the remaining checkboxes outside the current number of programs.
        if(check[i] != nullptr){//If the checkbox has reserved memory (if it doesn't we don't have to do anything).
            check[i]->hide();//We hide it. If we don't hide it it will continue appearing in the dialog, in spite of eliminating it of the layout (it would look ugly in addition).
            boxLayout->removeWidget(check[i]);//We remove it from the layout.
        }
    }
}
void dialog_deleteprogram::accept(){
    /*
    program is a list of QString. It will store the programs that will be
    overwritten in the file (i.e. all programs that will remain after deleting the desired programs).
    deleting the desired programs).
    */
    QList<QString> program;

    for(int i = 0;i < nPrograms;i++){
        if(!check[i]->isChecked()){//If it is not checked we add it to the file.
            program.push_back(check[i]->text());
        }
    }

    QFile fileWriteFile("programs.dat");
    if(!fileWriteFile.open(QIODevice::WriteOnly)){
        qDebug()<<"dialog_deleteprogram: Could not open the file";
        qDebug()<<qPrintable(fileWriteFile.errorString());
        return;
    }
    QDataStream output(&fileWriteFile);
    output.setVersion(QDataStream::Qt_6_7);

    for(int i = 0;i < program.size();i++){
        output<<program[i];
    }

    fileWriteFile.close();
    close();
}
