#ifndef DIALOG_ADDPROGRAM_H
#define DIALOG_ADDPROGRAM_H

#include <QDialog>
#include <QCheckBox>

//Previous declarations:
class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;
class QGroupBox;

class dialog_addprogram : public QDialog //dialog_addprogram inherits from QDialog.
{
    Q_OBJECT //We have SIGNAL and SLOT, so the Q_OBJECT macro is necessary.

public:
    dialog_addprogram(QWidget *parent = nullptr);//Constructor.
    void updateOpenPrograms();//Adds or refreshes the programs to the dialog.

private:
    QPushButton *acceptButton;//Button that, when pressed, adds the selected programs to the dialog.
    QPushButton *cancelButton;//Button that, when pressed, closes the dialog.
    QVBoxLayout *verticalLayout;//Layout that arranges the list of programs and the dialog buttons vertically.
    QHBoxLayout *horizontalLayout;//Layout that arranges the layout buttons horizontally.
    QGroupBox *programsBox;//Group of program names.
    QVBoxLayout *boxLayout;//Layout where program names are stored. This layout will be linked to programBox.
    QCheckBox *check[50];//Array of 50 checkboxes. To be changed.
    int nPrograms;//Number of programs to be displayed in the checkboxes.
    int nMaxPrograms;//Maximum number of programs that can be displayed in the checkbox.

private slots:
    void accept();//Executed when the user clicks the acceptButton button.
};

#endif // DIALOG_ADDPROGRAM_H
