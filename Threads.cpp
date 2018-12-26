
#include "Threads.h"

void Threads::addThread(thread *t) {
    this->threads.push_back(t);
}

Threads::~Threads() {
    for (auto &it : this->threads) {
        it->join();
        delete it;
    }
}