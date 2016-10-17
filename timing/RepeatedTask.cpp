#include "RepeatedTask.h"
#include "sleep.h"

RepeatedTask::~RepeatedTask()
{
    over = true;
    thread->join();
    delete thread;
}

void RepeatedTask::init(float frequency_)
{
    frequency = frequency_;
    over = false;
    thread = new std::thread(&RepeatedTask::execute, this);
}

void RepeatedTask::execute()
{
    double dt = 1/frequency;
    double next = now()+dt;

    while (!over) {
        // Sleeping
        if (now()-next > 0) {
            ms_sleep(now()-next);
        }
        next = now()+dt;

        // Ticking
        step();
    }
}
