#ifndef DIALOG_DELETEPROGRAM_H
#define DIALOG_DELETEPROGRAM_H

#include <QDialog>
#include <QCheckBox>

class QLabel;
class QPushButton;
class QVBoxLayout;
class QHBoxLayout;
class QCheckBox;
class QGroupBox;

class dialog_deleteprogram : public QDialog
{
    Q_OBJECT
public:
    dialog_deleteprogram(QWidget *parent = nullptr);
    void updateProgramsFile();

private:
    QPushButton *acceptButton;
    QPushButton *cancelButton;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QGroupBox *programsBox;
    QVBoxLayout *boxLayout;
    int nPrograms;//Number of programs to be displayed in the checkboxes.
    int nMaxPrograms;
    QCheckBox *check[50];//We would have to set it as a function of nMaxPrograms.

private slots:
    void accept();
};

#endif // DIALOG_DELETEPROGRAM_H
