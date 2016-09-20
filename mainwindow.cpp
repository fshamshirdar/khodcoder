#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent), ui(new Ui::Form), toggleStart(true)
{
    ui->setupUi(this);

    gp = new GeneticProgramming();
    gp->selectionType = DEFAULT_SELECTION_TYPE;
    gp->parentSelectionType = DEFAULT_PARENT_SELECTION_TYPE;

    ui->ResetButton->setEnabled(false);
    ui->OutputView->setReadOnly(true);
    ui->CodeView->setReadOnly(true);

    // initial values:
    ui->DistancePenalty->setValue(DEFAULT_DISTANCE_PENALTY);
    ui->NumberOfChilds->setValue(DEFAULT_NUM_CHILDREN);
    ui->PopulationSize->setValue(DEFAULT_POP_SIZE);
    ui->MutationRate->setValue(DEFAULT_MUTATION_RATE);
    ui->MinCodeSize->setValue(DEFAULT_MIN_PROGRAM_SIZE);
    ui->MaxCodeSize->setValue(DEFAULT_MAX_PROGRAM_SIZE);
    ui->DisplayRate->setValue(DEFAULT_DISPLAY_RATE);

    ui->Selection->addItem("Q-Tounament");
    ui->Selection->addItem("rouletteWheel");
    ui->Selection->addItem("SUS");
    ui->Selection->addItem("Sorting");
    ui->Selection->addItem("Default");

    ui->Parents->addItem("Q-Tounament");
    ui->Parents->addItem("rouletteWheel");
    ui->Parents->addItem("SUS");
    ui->Parents->addItem("Sorting");

    ui->Selection->setCurrentIndex(DEFAULT_SELECTION_TYPE);
    ui->Parents->setCurrentIndex(DEFAULT_PARENT_SELECTION_TYPE);

    // Inputs
    connect(ui->InputText, SIGNAL(textChanged(QString)), this, SLOT(inputTextChanged(QString)));
    connect(ui->StartButton, SIGNAL(clicked()), this, SLOT(startGP()));
    connect(ui->ResetButton, SIGNAL(clicked()), this, SLOT(resetGP()));
    // Parameters
    connect(ui->DistancePenalty, SIGNAL(valueChanged(int)), this, SLOT(distancePenaltyCallback(int)));
    connect(ui->LengthPenalty, SIGNAL(valueChanged(double)), this, SLOT(lengthPenaltyCallback(double)));
    connect(ui->PopulationSize, SIGNAL(valueChanged(int)), this, SLOT(populationSizeCallback(int)));
    connect(ui->MinCodeSize, SIGNAL(valueChanged(int)), this, SLOT(minCodeSizeCallback(int)));
    connect(ui->MaxCodeSize, SIGNAL(valueChanged(int)), this, SLOT(maxCodeSizeCallback(int)));
    connect(ui->MutationRate, SIGNAL(valueChanged(double)), this, SLOT(mutationRateCallback(double)));
    connect(ui->DisplayRate, SIGNAL(valueChanged(int)), this, SLOT(displayRateCallback(int)));
    connect(ui->NumberOfChilds, SIGNAL(valueChanged(int)), this, SLOT(numOfChildsCallback(int)));
    // Selection
    connect(ui->Selection, SIGNAL(currentIndexChanged(int)), this, SLOT(selectionCallback(int)));
    connect(ui->Parents, SIGNAL(currentIndexChanged(int)), this, SLOT(parentsCallback(int)));

    // GP
    connect(gp, SIGNAL(displayResult(Cromosoma,ulong)), this, SLOT(updateGP(Cromosoma, ulong)));
    connect(gp, SIGNAL(finished(Cromosoma)), this, SLOT(finished(Cromosoma)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete gp;
}

void MainWindow::startGP()
{
    if (toggleStart) {
        ui->CodeView->clear();
        ui->OutputView->setText("");
        ui->ResetButton->setEnabled(false);
        ui->StartButton->setEnabled(false);
        toggleStart = false;

        gp->setTarget(targetText);
        gp->start();

        ui->ResetButton->setEnabled(true);
        ui->StartButton->setEnabled(true);
        ui->StartButton->setText("Stop");
    } else {
        gp->stop();

        ui->CodeView->clear();
        ui->OutputView->clear();
        ui->ResetButton->setEnabled(false);
        ui->StartButton->setEnabled(true);
        ui->StartButton->setText("Start");
        toggleStart = true;
    }
}

void MainWindow::resetGP()
{
    ui->CodeView->clear();
    ui->OutputView->setText("");
    ui->ResetButton->setEnabled(false);
    gp->stop();
    gp->start();
    ui->ResetButton->setEnabled(true);
}

void MainWindow::updateGP(Cromosoma bestCromosoma, ulong iterations)
{
    ui->CodeView->clear();
    ui->CodeView->appendPlainText(bestCromosoma.program);
    ui->OutputView->setText(bestCromosoma.output);
    ui->Iterations->setText(QString::number(iterations));
    ui->MaxScore->setText(QString::number(bestCromosoma.fitnessScore));
}

void MainWindow::finished(Cromosoma finalCromosoma)
{
    ui->CodeView->appendPlainText(finalCromosoma.program);
    ui->ResultLabel->setText(finalCromosoma.output);
    ui->MaxScore->setText(QString::number(finalCromosoma.fitnessScore));
}

void MainWindow::inputTextChanged(QString targetText)
{
    this->targetText = targetText;
}

void MainWindow::distancePenaltyCallback(int distancePenalty)
{
    gp->distancePenalty = distancePenalty;
}

void MainWindow::lengthPenaltyCallback(double lengthPenalty)
{
    gp->lengthPenalty = lengthPenalty;
}

void MainWindow::populationSizeCallback(int populationSize)
{
    gp->populationSize = populationSize;
}

void MainWindow::minCodeSizeCallback(int minCodeSize)
{
    gp->minCodeSize = minCodeSize;
}

void MainWindow::maxCodeSizeCallback(int maxCodeSize)
{
    gp->maxCodeSize = maxCodeSize;
}

void MainWindow::mutationRateCallback(double mutationRate)
{
    gp->mutationRate = mutationRate;
}

void MainWindow::displayRateCallback(int displayRate)
{
    gp->displayRate = displayRate;
}

void MainWindow::numOfChildsCallback(int numOfChilds)
{
    gp->numChildrens = numOfChilds;
}

void MainWindow::selectionCallback(int index)
{
    gp->selectionType = (GeneticParameters::SelectionType)index;
}

void MainWindow::parentsCallback(int index)
{
    gp->parentSelectionType = (GeneticParameters::SelectionType)index;
}
