#include "brainfuck.h"

BrainFuck::BrainFuck()
{
    this->reset();
}

QString BrainFuck::run(const QString& program)
{
    this->reset();
    this->program = program;

    if (! this->checkSyntax())
        return "Error: Bad Syntax";

    while(this->instructionPointer < this->program.length())
    {
        if(this->totalCycles > MAX_CYCLES || this->errorHappened)
            return "Error: Max Cycles or Wrong Loop!";

        doCommand(this->program[this->instructionPointer]);

        this->instructionPointer ++;
        this->totalCycles ++;
    }

    return this->output;
}

void BrainFuck::doCommand(const QChar& command)
{
    switch (command.toAscii()) {
    case '>':
        this->tapePointer ++;
        if(this->tapePointer >= MAX_TAPE_SIZE)
            this->errorHappened = true;
        break;
    case '<':
        this->tapePointer --;
        if(this->tapePointer < 0)
            this->errorHappened = true;
        break;
    case '+':
        this->tape[this->tapePointer] ++;
        break;
    case '-':
        this->tape[this->tapePointer] --;
        break;
    case '.':
        this->output += this->tape[this->tapePointer];
        break;
    case '[':
        if (! this->tape[this->tapePointer])
            this->instructionPointer = loop(true) - 1;
        break;
    case ']':
        if (this->tape[this->tapePointer])
            this->instructionPointer = loop(false);
        break;
    case '#':  // This was only used for debugging purposes, and is not an actual command.
//        std::stringstream int_val;
//        int_val << static_cast<int>(this->tape[this->tapePointer]);  // Need to cast char to int or else the ASCII value will be used.
//        this->output += int_val.str();  // Then add the string representation of the number to the output.
        break;
    default:
        break;
    }
}

int BrainFuck::loop(bool isStart)
{
    int direction = (isStart) ? 1 : -1;
    int numberOfNestedLoop = 1;
    int tempInstructionPointer = this->instructionPointer + direction;
    while(numberOfNestedLoop > 0) {
        if (tempInstructionPointer < 0 || tempInstructionPointer >= this->program.length()) {
            this->errorHappened = true;
            return -1;
        }

        QChar c = this->program[tempInstructionPointer];
        if (c == '[') {
            if (isStart)
                numberOfNestedLoop += 1;
            else
                numberOfNestedLoop -= 1;
        } else if (c == ']') {
            if (isStart) {
                numberOfNestedLoop -= 1;
            } else {
                numberOfNestedLoop += 1;
            }
        }

        tempInstructionPointer += direction;
    }
    return tempInstructionPointer;
}

bool BrainFuck::checkSyntax()
{
    for(int i = 0; i < this->program.length(); ++i)
    {
        this->instructionPointer = i;

        if (this->program[i] == '[') {
            this->loop(true);
        } else if (this->program[i] == ']') {
            this->loop(false);
        }

        if (this->errorHappened)
            return false;
    }

    this->instructionPointer = 0;
    return true;
}

void BrainFuck::reset()
{
    this->instructionPointer = 0;
    this->tapePointer = 0;
    this->program = "";
    this->output = "";
    this->errorHappened = false;
    this->totalCycles = 0;

    for(unsigned i = 0; i < MAX_TAPE_SIZE; ++i)
        this->tape[i] = 0;
}
