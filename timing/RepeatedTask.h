#pragma once

#include <thread>
#include "TimeStamp.hpp"

class RepeatedTask
{
    public:
        virtual ~RepeatedTask();

        void init(float frequency);
        void execute();
        virtual void step()=0;

    protected:
        float frequency;
        bool over;
        std::thread *thread;
};

