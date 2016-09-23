#include "cromosoma.h"
/*
Cromosoma::Cromosoma()
{
}

Cromosoma::Cromosoma(const Cromosoma& other)
{
    this->program = other.program;
    this->output = other.output;
    this->fitnessScore = other.fitnessScore;
}

Cromosoma::~Cromosoma()
{
}*/

void Cromosoma::generateRandomCromosoma()
{
    int programSize = Utils::random(DEFAULT_MIN_PROGRAM_SIZE, DEFAULT_MAX_PROGRAM_SIZE);

    program = "";
    for (int i = 1; i <= programSize; ++i)
        program += getRandomInstruction();
}

double Cromosoma::calculateFitness(const QString& targetOutput)
{
    double score = 0;
    double maxScore = targetOutput.length() * DEFAULT_DISTANCE_PENALTY;

    output = brainfuck.run(program);

    if (output.startsWith("Error:")) // Error happened
        return DEFAULT_ERROR_SCORE;

    QString minProgram = (output.length() < targetOutput.length()) ? output : targetOutput;
    QString maxProgram = (output.length() >= targetOutput.length()) ? output : targetOutput;

    for (int i = 0; i < maxProgram.length(); ++i) {
        int outputChar = (i < minProgram.length()) ? minProgram[i].toAscii() : (maxProgram[i].toAscii() + DEFAULT_DISTANCE_PENALTY);
        score += abs(outputChar - maxProgram[i].toAscii());
    }

    score += (program.length() * DEFAULT_LENGTH_PENALTY);
    fitnessScore = maxScore - score;

    return fitnessScore;
}

void Cromosoma::mutate()
{
    for (int i = 0; i < program.length(); ++i) {
        if (DEFAULT_MUTATION_RATE >= Utils::random(0.0, 1.0)) {
            int mutation_type = Utils::random(1, DEFAULT_NUM_MUTATIONS_TYPES);

            switch(mutation_type)
            {
            case 1:
                mutateInstruction(i);
                break;
            case 2:
                addNewInstruction(i);
                break;
            case 3:
                removeInstruction(i);
                break;
            case 4:
                crossoverInstruction(i);
            default:
                break;
            }
        }
    }
}

QChar Cromosoma::getRandomInstruction()
{
    int index = Utils::random(0, NUM_INSTRUCTIONS - 1);
    return INSTRUCTIONS[index];
}

void Cromosoma::addNewInstruction(int index)
{
    if ((program.length() + 1) <= DEFAULT_MAX_PROGRAM_SIZE)
        program.insert(index, getRandomInstruction());
}

void Cromosoma::removeInstruction(int index)
{
    if ((program.length() - 2) >= DEFAULT_MIN_PROGRAM_SIZE)
        program.remove(index, 1);
}

void Cromosoma::mutateInstruction(int index)
{
    program[index] = getRandomInstruction();
}

void Cromosoma::crossoverInstruction(int index)
{
    int randomIndex = Utils::random(0, program.length());
    QChar temp = program[index];
    program[index] = program[randomIndex];
    program[randomIndex] = temp;
}

bool Cromosoma::equals(const Cromosoma& other) const
{
    return (program.compare(other.program) == 0);
}

void Cromosoma::operator=(const Cromosoma& other)
{
    program = other.program;
    output = other.output;
    fitnessScore = other.fitnessScore;
}
