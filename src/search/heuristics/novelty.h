#pragma once

#include "../heuristic.h"
#include "../utils/rng.h"

namespace Novelty {
class Novelty : public Heuristic {
protected:
    std::vector<std::vector<unsigned int> > map;
    virtual void initialize();
    virtual int compute_heuristic(const GlobalState &state);
public:
    Novelty(const Options &options);
    ~Novelty();
};
}


