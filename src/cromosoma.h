#ifndef CROMOSOMA_H
#define CROMOSOMA_H

#include <QMetaType>
#include <QString>
#include "GPParameters.h"
#include "brainfuck.h"
#include "utils.h"

class Cromosoma
{
public:
//    Cromosoma();
//    Cromosoma(const Cromosoma& other);
//    ~Cromosoma();

    void generateRandomCromosoma();
    double calculateFitness(const QString &program);
    void mutate();
    bool equals(const Cromosoma& other) const;
    void operator=(const Cromosoma& other);

public:
    QString program;
    QString output;
    double fitnessScore;

private:
    QChar getRandomInstruction();
    void addNewInstruction(int index);
    void removeInstruction(int index);
    void mutateInstruction(int index);
    void crossoverInstruction(int index);

private:
    BrainFuck brainfuck;
};

Q_DECLARE_METATYPE(Cromosoma)

#endif // CROMOSOMA_H
