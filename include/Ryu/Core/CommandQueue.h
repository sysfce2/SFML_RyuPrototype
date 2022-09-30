#pragma once

#include <vector>
#include <Ryu/Core/Command.h>

//namespace ryu{
class CommandQueue
{
    public:
        void push(const Command& command);
        Command pop();
        bool isEmpty() const;

    private:
        std::vector<Command> mQueue;
};

//} /// namespace ryu