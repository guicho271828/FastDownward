#pragma once

#include "../heuristic.h"
#include "../utils/rng.h"

namespace Novelty {
class Novelty : public Heuristic {
protected:
    std::set<std::pair<VariableProxy,int> > db;
    virtual void initialize();
    virtual int compute_heuristic(const GlobalState &state);
    class is_novel;
public:
    Novelty(const Options &options);
    ~Novelty();
};
}


