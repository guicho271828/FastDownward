#ifndef HEURISTICS_RANDOM_H
#define HEURISTICS_RANDOM_H

#include "../heuristic.h"
#include "../utils/rng.h"

namespace RandomHeuristics {
class RandomHeuristics : public Heuristic {
protected:
    PerStateInformation<int> db(-1);
    /* = PerStateInformation<int>(-1); */
    virtual void initialize();
    virtual int compute_heuristic(const GlobalState &state);
public:
    RandomHeuristics(const Options &options);
    ~RandomHeuristics();
};
}

#endif
