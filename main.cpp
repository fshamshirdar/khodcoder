#include <QApplication>
#include "mainwindow.h"
#include "GP.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    qRegisterMetaType<Cromosoma>();

    MainWindow* mainWindow = new MainWindow();
    mainWindow->show();

//    GeneticProgramming* gp = new GeneticProgramming();
//    gp->setTarget("bita");
//    gp->start();

    return app.exec();
}
