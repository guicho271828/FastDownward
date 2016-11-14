/* -*- mode : c++ -*- */
#ifndef HEURISTICS_NOVELTY_HEURISTIC_H
#define HEURISTICS_NOVELTY_HEURISTIC_H

#include "../heuristic.h"
#include "../evaluation_context.h"

// #include "SAPPOROBDD/ZBDD.h"

#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace NoveltyHeuristic {
    using Key = std::vector<int>;
    using Conjunction = std::vector<FactProxy>;
    using DB  = std::unordered_set<Conjunction>;
    class NoveltyHeuristic : public Heuristic {
        int atoms;
        std::vector<ScalarEvaluator *> subevaluators;
        EvaluationContext* tmp_context;
        // struct info {
        //     bool initialized = false;
        //     Key key;
        //     int value;
        // };
        // PerStateInformation<info> db;
    
        std::unordered_map<Key, DB> plateaus;
        Key key(EvaluationContext &eval_context);
        void collect_tuples(int remaining,
                            Conjunction::iterator begin,
                            std::vector<Conjunction> &cs,
                            Conjunction &stack,
                            Conjunction &direct_state);
    protected:
        virtual void initialize();
        virtual int compute_heuristic(const GlobalState &global_state);
    public:
        virtual EvaluationResult compute_result(
            EvaluationContext &eval_context); // override
        NoveltyHeuristic(const Options &options);
        ~NoveltyHeuristic();
    };
}

#endif
