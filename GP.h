#ifndef GP_H
#define GP_H

#include <QThread>
#include <QString>
#include <QVector>
#include <QTime>
#include "GPParameters.h"
#include "cromosoma.h"
#include "brainfuck.h"
#include "utils.h"

class GeneticProgramming : public QThread
{
    Q_OBJECT

public:
    GeneticProgramming();
    ~GeneticProgramming();
    void setTarget(QString targetOutput);
    void run();
    void stop();

signals:
    void displayResult(Cromosoma, unsigned long);
    void finished(Cromosoma);

private:
    void initPopulation();
    void calculatePopulationScore();
    double populationTotalScore(const QVector<Cromosoma> newCromosomas);

    int SUSSelection(const QVector<Cromosoma> newCromosomas, int numOfChildren = DEFAULT_NUM_CHILDREN, int n = 0);
    int QTournament(const QVector<Cromosoma> newCromosomas, int q);
    int rouletteWheelSelection(const QVector<Cromosoma> newCromosomas);
    int sortingSelection(const QVector<Cromosoma> newCromosomas);

    Cromosoma selectParent(const Cromosoma& cromosoma = Cromosoma());
    void selectSurvival(const Cromosoma& parent1, const Cromosoma& parent2, const Cromosoma children[]);

    QString mutate(QString child);
    void crossover(const Cromosoma& parent1, const Cromosoma& parent2, Cromosoma children[]);
    bool programExists(const Cromosoma& program);
    void replaceProgram(const Cromosoma& parent, const Cromosoma &child);

private:
    bool isRunning;
    QString targetOutput;

    unsigned long generations;
    QVector<Cromosoma> cromosomas;
    Cromosoma bestCromosoma;

    double worstScore;
    double bestScore;
    int worstIndex;
    int bestIndex;

    int q_;

public: // Parameters
    int distancePenalty;
    int numChildrens;
    int populationSize;
    int minCodeSize;
    int maxCodeSize;
    int displayRate;
    double mutationRate;
    double lengthPenalty;

    GeneticParameters::SelectionType selectionType;
    GeneticParameters::SelectionType parentSelectionType;
};

#endif // GENETICPROGRAMMING_H
