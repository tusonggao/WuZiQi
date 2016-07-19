#ifndef COMMONWFUNC_HH
#define COMMONWFUNC_HH

#include <thread>
#include <iostream>
#include <ctime>

class Random
{
public:
    Random()
    {
        srand((int)time(0));
    }

public:
    int getRandNum(int N)
    {
        return rand()%N;
    }



};


class TimeCounter
{
public:
    TimeCounter()
    {
    }

public:
    void start()
    {
        m_start = clock();
    }

    void end()
    {
        m_end = clock();
    }

    double getDurationTime() // in seconds
    {
        return double(m_end - m_start)/CLOCKS_PER_SEC;
    }

private:
    time_t m_start;
    time_t m_end;
};

inline void SleepForMilliSeconds(const long n)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(n));
}

inline void PrintCurrentThreadID()
{
    std::cout << "Current Thread ID is: " << std::this_thread::get_id() << std::endl;
}

#endif // COMMONWFUNC_HH
