#ifndef BRAINFUCK_H
#define BRAINFUCK_H

#include <QString>

#define MAX_CYCLES    1000
#define MAX_TAPE_SIZE 1000

class BrainFuck
{
public:
    BrainFuck();
    QString run(const QString& prgrm);

private:
    unsigned char tape[MAX_TAPE_SIZE];

    int instructionPointer;
    int tapePointer;
    int totalCycles;
    bool errorHappened;

    QString program;
    QString output;

    void doCommand(const QChar& command);
    int loop(bool isStart);
    bool checkSyntax();
    void reset();

};

#endif // BRAINFUCK_H

