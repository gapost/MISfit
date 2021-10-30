#ifndef STOPWATCH_H
#define STOPWATCH_H

// for clock() and CLOCKS_PER_SEC
#include <time.h>

inline static double miliseconds(void)
{
    const double milisecs_per_tick = 1000.0 / CLOCKS_PER_SEC;
    return ( (double) clock() ) * milisecs_per_tick;
}

/**
	Performance Stopwatch  used for benchmarking codes.
	Measure seconds elapsed.
*/
class Stopwatch {
    private:
        int running_;
        double start_time_;
        double total_;

    public:
        Stopwatch() : running_(0), start_time_(0.0), total_(0.0) {}
        inline void start()
		{
			running_ = 1;
			total_ = 0.0;
			start_time_ = miliseconds();
		}
        inline double stop()
		{
			if (running_) 
			{
				total_ += (miliseconds() - start_time_); 
				running_ = 0;
			}
			return total_; 
		}
		inline double read()
		{
			if (running_)
			{
				stop();
				resume();
			}
			return total_;
		}
		inline void resume()
		{
			if (!running_)
			{
				start_time_ = miliseconds();
				running_ = 1;
			}
		}
		inline int running()  { return running_; }
};


#endif
    

            
