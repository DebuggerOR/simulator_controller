

#ifndef THREADS_H
#define THREADS_H

#include <thread>
#include <sys/types.h>
#include <vector>

using namespace std;


class Threads {
    vector<thread *> threads;

public:
    void addThread(thread *t);

    ~Threads();
};

#endif
