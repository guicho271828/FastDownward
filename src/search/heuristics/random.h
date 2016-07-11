/* -*- mode : c++ -*- */
#pragma once

#include "../heuristic.h"
#include "../utils/rng.h"
#include "../per_state_information.h"

namespace RandomHeuristics {
    class RandomHeuristics : public Heuristic {
        int bound;
    protected:
        PerStateInformation<int> db;
        virtual void initialize();
        virtual int compute_heuristic(const GlobalState &state);
    public:
        RandomHeuristics(const Options &options);
        ~RandomHeuristics();
    };
}


