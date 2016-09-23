#include "GP.h"

GeneticProgramming::GeneticProgramming() : q_(8), isRunning(false)
{
    cromosomas = QVector<Cromosoma>(DEFAULT_POP_SIZE);
    distancePenalty = DEFAULT_DISTANCE_PENALTY;
    numChildrens = DEFAULT_NUM_CHILDREN;
    populationSize = DEFAULT_POP_SIZE;
    minCodeSize = DEFAULT_MIN_PROGRAM_SIZE;
    maxCodeSize = DEFAULT_MAX_PROGRAM_SIZE;
    displayRate = DEFAULT_DISPLAY_RATE;
    mutationRate = DEFAULT_MUTATION_RATE;
    lengthPenalty = DEFAULT_LENGTH_PENALTY;

    selectionType = DEFAULT_SELECTION_TYPE;
    parentSelectionType = DEFAULT_PARENT_SELECTION_TYPE;
}

GeneticProgramming::~GeneticProgramming()
{
}

void GeneticProgramming::initPopulation()
{
    for (int i = 0; i < populationSize; ++i) {
        cromosomas[i].generateRandomCromosoma();
    }
}

void GeneticProgramming::calculatePopulationScore()
{
    cromosomas[0].calculateFitness(targetOutput);
    bestScore = cromosomas[0].fitnessScore;
    worstScore = cromosomas[0].fitnessScore;

    for (int i = 0; i < populationSize; ++i) {
        cromosomas[i].calculateFitness(targetOutput);

        if (cromosomas[i].fitnessScore >= bestScore) {
            bestCromosoma = cromosomas[i];
            bestScore = cromosomas[i].fitnessScore;
            bestIndex = i;
        } else if (cromosomas[i].fitnessScore <= worstScore) {
            worstScore = cromosomas[i].fitnessScore;
            worstIndex = i;
        }
    }
}

double GeneticProgramming::populationTotalScore(const QVector<Cromosoma> newCromosomas)
{
    double total = 0;
    for (int i = 0; i < newCromosomas.size(); ++i)
        total += newCromosomas[i].fitnessScore;

    return total;
}

bool GeneticProgramming::programExists(const Cromosoma& cromosoma)
{
    for (unsigned i = 0; i < populationSize; ++i) {
        if (cromosoma.equals(cromosomas[i]))
            return true;
    }
    return false;
}

void GeneticProgramming::replaceProgram(const Cromosoma& parent, const Cromosoma &child)
{
    for (int i = 0; i < populationSize; ++i) {
        if (parent.equals(cromosomas[i])) {
            cromosomas[i] = child;
            break;
        }
    }
}

int GeneticProgramming::rouletteWheelSelection(const QVector<Cromosoma> newCromosoma)
{
    QVector<double> programRouletteWheel(newCromosoma.size());
    double totalScore = populationTotalScore(newCromosoma);
    double randValue = Utils::random(0.0, 1.0);

    for (int i = 0; i < newCromosoma.size(); ++i) {
        double prevChance = ((i - 1) < 0) ? 0 : programRouletteWheel[i - 1];

        programRouletteWheel[i] = (newCromosoma[i].fitnessScore / totalScore) + (prevChance);
        if (programRouletteWheel[i] >= randValue - 0.001)
            return i;
    }

    return 0;
}

int GeneticProgramming::SUSSelection(const QVector<Cromosoma> newCromosoma, int numOfChildren, int n)
{
    QVector<double> programRouletteWheel(newCromosoma.size());
    double totalScore = populationTotalScore(newCromosoma);
    double randValue = Utils::random(0.0, 1.0 - (1.0 / (numOfChildren)));

    for (int i = 0; i < newCromosoma.size(); ++i) {
        double prevChance = ((i - 1) < 0) ? 0 : programRouletteWheel[i - 1];

        programRouletteWheel[i] = (newCromosoma[i].fitnessScore / totalScore) + (prevChance);
        randValue += n * (1.0 / (numOfChildren));

        if (programRouletteWheel[i] >= randValue - 0.001)
            return i;
    }

    return 0;
}

int GeneticProgramming::sortingSelection(QVector<Cromosoma> newCromosoma)
{
    for (int i = 0; i < newCromosoma.size(); ++i) {
        for (int j = i+1; j < newCromosoma.size(); ++j) {
            if (newCromosoma[i].fitnessScore > newCromosoma[j].fitnessScore) {
                Cromosoma temp = newCromosoma[i];
                newCromosoma[i] = newCromosoma[j];
                newCromosoma[j] = temp;
            }
        }
    }

    // Pi = 2*i / u(u-1)
    QVector<double> programRouletteWheel(newCromosoma.size());
    double randValue = Utils::random(0.0, 1.0);

    for (int i = 0; i < newCromosoma.size(); ++i) {
        double prevChance = ((i - 1) < 0) ? 0 : programRouletteWheel[i - 1];
        programRouletteWheel[i] = (2*i / (double)(newCromosoma.size()*(newCromosoma.size() - 1))) + (prevChance);
        if (programRouletteWheel[i] >= randValue - 0.001)
            return i;
    }

    return 0;
}

int GeneticProgramming::QTournament(QVector<Cromosoma> newCromosoma, int q)
{
    int index = Utils::random(0, newCromosoma.size()-1);
    int maxIndex = index;
    bool skip;
    for (int i = 1; i < q; ++i) {
        skip = false;
        index = Utils::random(0, newCromosoma.size()-1);
        if (newCromosoma[index].fitnessScore > newCromosoma[maxIndex].fitnessScore) {
            maxIndex = index;
        }
    }
    return maxIndex;
}

Cromosoma GeneticProgramming::selectParent(const Cromosoma& cromosoma)
{
    int n = 0;
    QVector<Cromosoma> selectionCromosoma;
    for (int i = 0; i < cromosomas.size(); ++i) {
        if (! cromosomas[i].equals(cromosoma))
            selectionCromosoma.append(cromosomas[i]);
        else
            n ++;
    }

    int index;
    if (parentSelectionType == GeneticParameters::rouletteWheel)
        index = rouletteWheelSelection(selectionCromosoma);
    else if (parentSelectionType == GeneticParameters::SUS)
        index = SUSSelection(selectionCromosoma, numChildrens, n);
    else if (parentSelectionType == GeneticParameters::Sorting)
        index = sortingSelection(selectionCromosoma);
    else if (parentSelectionType == GeneticParameters::QTournamentType)
        index = QTournament(selectionCromosoma, q_);
    else
        qt_assert("Unknown Selection Type", "GP.cpp", 0);
    return cromosomas[index];
}

void GeneticProgramming::selectSurvival(const Cromosoma& parent1, const Cromosoma& parent2, const Cromosoma children[])
{
    if (selectionType == GeneticParameters::Default) {
        replaceProgram(parent1, children[0]);
        replaceProgram(parent2, children[1]);
        if (! programExists(bestCromosoma))
            cromosomas[worstIndex] = bestCromosoma;
        return;
    }

    QVector<Cromosoma> survivalCromosomas(populationSize+numChildrens);
    for (int i = 0; i < populationSize+numChildrens; ++i) {
        if (i < DEFAULT_POP_SIZE) {
            survivalCromosomas[i] = cromosomas[i];
        } else {
            survivalCromosomas[i] = children[i-populationSize];
        }
    }

    int n = 0;
    int index;
    for (int j = 0; j < populationSize-1; ++j) {
        if (selectionType == GeneticParameters::QTournamentType)
            index = QTournament(survivalCromosomas, 4);
        else if (selectionType == GeneticParameters::rouletteWheel)
            index = rouletteWheelSelection(survivalCromosomas);
        else if (selectionType == GeneticParameters::SUS)
            index = SUSSelection(survivalCromosomas, DEFAULT_POP_SIZE, n);
        else if (selectionType == GeneticParameters::Sorting)
            index = sortingSelection(survivalCromosomas);
        else
            qt_assert("Unknown Selection Type", "GP.cpp", 0);
        cromosomas[j] = survivalCromosomas[index];
        survivalCromosomas.remove(index);
        n ++;
    }
}

void GeneticProgramming::crossover(const Cromosoma& parent1, const Cromosoma& parent2, Cromosoma children[])
{
    Cromosoma minParent = (parent1.program.length() < parent2.program.length()) ? parent1 : parent2;
    Cromosoma maxParent = (parent1.program.length() >= parent2.program.length()) ? parent1 : parent2;

    int crosspoint = Utils::random(1, minParent.program.length()-1);

    QString leftMinParent = minParent.program.mid(0, crosspoint);
    QString rightMinParent = minParent.program.mid(crosspoint);

    QString leftMaxParent = maxParent.program.mid(0, crosspoint);
    QString rightMaxParent = maxParent.program.mid(crosspoint);

    children[0].program = leftMinParent+rightMaxParent;
    children[1].program = leftMaxParent+rightMinParent;
}

void GeneticProgramming::setTarget(QString targetOutput)
{
    this->targetOutput = targetOutput;
}

#include <iostream>
void GeneticProgramming::run()
{
    if (targetOutput.isEmpty())
        return;

    isRunning = true;

    qsrand(QTime::currentTime().msec());
    initPopulation();

    generations = 0;

    while (isRunning) {
        calculatePopulationScore();

        Cromosoma parent1 = selectParent();
        Cromosoma parent2 = selectParent(parent1);

        Cromosoma children[DEFAULT_NUM_CHILDREN];
        crossover(parent1, parent2, children);

        for (int i = 0; i < numChildrens; ++i) {
            children[i].mutate();
        }
        selectSurvival(parent1, parent2, children);

        if (! (generations % displayRate)) {
            bestCromosoma.calculateFitness(targetOutput);
            emit displayResult(bestCromosoma, generations);
//            std::cout << bestCromosoma.program.toStdString() << ": " << bestCromosoma.output.toStdString() << std::endl;
            if (bestCromosoma.output == targetOutput)
            {
                emit finished(bestCromosoma);
                break;
            }
        }

        generations ++;
    }
}

void GeneticProgramming::stop()
{
    isRunning = false;
    sleep(1);
}
