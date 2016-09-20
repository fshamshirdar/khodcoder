#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QThread>
#include "ui_form.h"
#include "GP.h"
#include "brainfuck.h"

namespace Ui {
    class Form;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

public slots:
    void inputTextChanged(QString);
    void updateGP(Cromosoma, ulong);
    void finished(Cromosoma);
    void startGP();
    void resetGP();
    void distancePenaltyCallback(int);
    void lengthPenaltyCallback(double);
    void populationSizeCallback(int);
    void minCodeSizeCallback(int);
    void maxCodeSizeCallback(int);
    void mutationRateCallback(double);
    void displayRateCallback(int);
    void numOfChildsCallback(int);
    void selectionCallback(int);
    void parentsCallback(int);

private:
    GeneticProgramming* gp;
    QString targetText;

    bool toggleStart;
    Ui::Form *ui;
};

#endif // MAINWINDOW_H
