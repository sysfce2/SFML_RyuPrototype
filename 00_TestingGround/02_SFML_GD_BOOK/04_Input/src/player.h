#pragma once

class CommandQueue;

namespace sf{
    class Event;
}

class Player
{

    public:
        void handleEvent(const sf::Event& event,
                         CommandQueue& commands);

        void handleRealtimeInput(CommandQueue& commands);

    private:
        void setActions();

};