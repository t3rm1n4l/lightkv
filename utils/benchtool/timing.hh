/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */

#ifndef TIMING_HH
#define TIMING_HH 1

#include <sys/time.h>
#include <iomanip>

using namespace std;

#define SHOW_TIME 1

// Timing class for profiling code blocks
class Timing {

public:
    Timing(std::string name, bool autodisplay=true): total(0), stopped(false), display_at_end(autodisplay) {
#ifdef SHOW_TIME
        blockname = name;
        start();
#endif
    }

    ~Timing() {
#ifdef SHOW_TIME
        stop();
        if (display_at_end) {
            display();
        }
#endif
    }

    void start() {
#ifdef SHOW_TIME
        time_start = getTime();
        stopped = false;
#endif
    }

    void stop() {
#ifdef SHOW_TIME
        if (!stopped){
            total += getTime() - time_start;
            stopped = true;
        }
#endif
    }

    double getTotalSeconds() {
        return (double)total/1000000;
    }

    void reset() {
#ifdef SHOW_TIME
        total = 0;
#endif
    }

    void display() {
#ifdef SHOW_TIME
        cout.setf(ios::fixed);
        cout<<"Timing: "<<blockname<<" took "<<setprecision(3)<<getTotalSeconds()<<" seconds"<<endl;
#endif
    }

private:
    uint64_t getTime() {
        struct timeval time;
        gettimeofday(&time, NULL);
        return time.tv_sec*1000000 + time.tv_usec;
    }
    std::string blockname;
    uint64_t total;
    uint64_t time_start;
    bool stopped;
    bool display_at_end;
};

#endif
