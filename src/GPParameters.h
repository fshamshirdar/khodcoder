#ifndef GPPARAMETERS_H
#define GPPARAMETERS_H

#include <QChar>

#define DEFAULT_DISTANCE_PENALTY 255
#define DEFAULT_NUM_CHILDREN 2
#define DEFAULT_POP_SIZE 10
#define DEFAULT_MIN_PROGRAM_SIZE 10
#define DEFAULT_MAX_PROGRAM_SIZE 500
#define DEFAULT_MUTATION_RATE 0.01
#define DEFAULT_LENGTH_PENALTY 0.001
#define DEFAULT_DISPLAY_RATE 100
#define DEFAULT_ERROR_SCORE 1.0
#define DEFAULT_NUM_MUTATIONS_TYPES 4

#define DEFAULT_PARENT_SELECTION_TYPE GeneticParameters::QTournamentType
#define DEFAULT_SELECTION_TYPE GeneticParameters::Default

class GeneticParameters {
public:
    enum SelectionType {
        QTournamentType = 0,
        rouletteWheel,
        SUS,
        Sorting,
        Default
    };
};

static const QChar INSTRUCTIONS[] = {'+', '-', '>', '<', '[', ']', '.'};
static const int NUM_INSTRUCTIONS = (sizeof(INSTRUCTIONS) / sizeof(INSTRUCTIONS[0]));

#endif // GPPARAMETERS_H
