#include "dialog_addprogram.h"
#include "engine.h"
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

//Constructor.
dialog_addprogram::dialog_addprogram(QWidget *parent) : QDialog(parent)
{
    //Initialize the variables:
    acceptButton = new QPushButton(tr("&Accept"));//acceptButton has the text Accept and keyboard shortcut Alt + A.
    cancelButton = new QPushButton(tr("&Cancel"));//cancelButton has the text Cancel and keyboard shortcut Alt + C.
    verticalLayout = new QVBoxLayout;
    horizontalLayout = new QHBoxLayout;
    programsBox = new QGroupBox;
    boxLayout = new QVBoxLayout;
    nMaxPrograms = 50;//The maximum number of programs Margaret can monitor is 50.

    //Initialize the pointers to nullptr:
    for(int i = 0;i < nMaxPrograms;i++){
        check[i] = nullptr;
    }

    updateOpenPrograms();//Fills the QCheckbox boxLayout with the names of currently open programs.

    programsBox->setTitle(tr("Select the programs you want to monitor"));//Assigns the text that appears above the QCheckbox.
    programsBox->setLayout(boxLayout);//Links the boxLayout filled with programs to programsBox.

    //Add the widgets to the layout:

    //Since cancelButton and acceptButton are arranged horizontally, they are added to horizontalLayout:
    horizontalLayout->addWidget(cancelButton);
    horizontalLayout->addWidget(acceptButton);

    //Since programsBox and horizontalLayout are arranged vertically, they are added to verticalLayout:
    verticalLayout->addWidget(programsBox);
    verticalLayout->addLayout(horizontalLayout);

    setLayout(verticalLayout);//The dialog has a verticalLayout layout.

    setAttribute(Qt::WA_DeleteOnClose);//The WA_DeleteOnClose flag is activated. When the dialog is closed, all its memory will be released.

    connect(acceptButton,SIGNAL(clicked()),this,SLOT(accept()));//When acceptButton is clicked, the SLOT accept is executed.
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close())); // When cancelButton is clicked, the SLOT close is executed.

    setWindowTitle(tr("Add program")); // The title of the dialog window is Add program.
}
void dialog_addprogram::updateOpenPrograms(){
    clear_windowInfoList();//Before filling the windowInfoList, we must clear it so that there are no repetitions of the same program.

    EnumWindows(EnumWindowsProc, NULL);//We fill the windowInfoList.

    std::vector<WindowInfo> windowInfoList = get_windowInfoList();//A copy of windowInfoList is made.

    nPrograms = windowInfoList.size();//The number of programs to display is the size of the windowInfoList.

    QString buttonName[nPrograms];//The buttonName array will store the names that will be assigned to the QCheckBoxes in the dialog.
    std::wstring program;//Temporary name for each program.

    for(int i = 0;i < nPrograms;i++){//For each program in windowInfoList:
        program = processName(windowInfoList[i].processName);//program is the name of the program process in windowInfoList.
        buttonName[i] = QString::fromStdWString(program);//Casting from WString to QString, thanks to a method of the QString class.
    }

    //We will create as many QCheckBoxes as there are programs:

    for(int i = 0;i < nPrograms;i++){//For each program, its own QCheckBox is created and added to the layout.
        check[i] = new QCheckBox(buttonName[i]);//Memory is reserved for the QCheckBox with the desired name.
        boxLayout->addWidget(check[i]);//We add this to the boxLayout.
    }
}

//SLOT that saves the selected programs in a file.
void dialog_addprogram::accept(){
    QFile readFile("programs.dat");//The desired programs to monitor will be stored in the programs.dat file.
    if(!readFile.open(QIODevice::ReadOnly)){//If the file cannot be opened in read-only mode:
        //The error is reported by console:
        qDebug()<<"dialog_addprogram: Could not open the file";
        qDebug()<<qPrintable(readFile.errorString());
    }

    QList<QString> program;//program is a list of QString that will store all the programs to be saved.
    QString pr;//pr will store the program to be saved in each iteration.

    QDataStream input(&readFile);//input allows us to read the file line by line.
    input.setVersion(QDataStream::Qt_6_7);//The Qt_6_7 version is used for this.

    int i = 0;//Iterator i.
    bool emptyString = false;//If we find an empty string, it will be true.

    while(i < nMaxPrograms && !emptyString){//As long as we have not exceeded the maximum number of programs or found an empty string:
        input>>pr;//pr is the line read from the file.
        if(pr == ""){//If pr is an empty string, we assign true to emptyString:
            emptyString = true;
        }else{//If not:
            program.push_back(pr);//It is added to the list of programs in the file.
            i++;//The iterator is incremented.
        }
    }
    readFile.close();//Closes the file.

    QFile writeFile("programs.dat");//We will open programs.dat in write-only mode.
    if(!writeFile.open(QIODevice::WriteOnly)){//If the file cannot be opened in write-only mode:
        //The error is reported by the console:
        qDebug()<<"dialog_addprogram: Could not open the file";
        qDebug()<<qPrintable(writeFile.errorString());

        return;//End execution, without being able to write the list of programs.
    }

    QDataStream output(&writeFile);
    output.setVersion(QDataStream::Qt_6_7);

    //Iterate through each check:
    for(int i = 0;i < nPrograms;i++){
        if(check[i]->isChecked() && !program.contains(check[i]->text())){//If check is checked and its name is not in the file:
            program.push_back(check[i]->text());//The name is added to the list of programs in the file.
        }
    }

    for(int i = 0;i < program.size();i++){//Iterate through the program list:
        output<<program[i];//Write each program to the output (in the programs.dat file).
    }

    writeFile.close();//Close the file.
    close();//Close the dialog.
}
