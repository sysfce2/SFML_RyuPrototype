#pragma once

#include <vector>
#include "command.h"

class CommandQueue
{
    public:
        void push(const Command& command);
        Command pop();
        bool isEmpty() const;

    private:
        std::vector<Command> mQueue;
};