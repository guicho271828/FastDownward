/* -*- mode : c++ -*- */
#pragma once

#include "../scalar_evaluator.h"
#include <vector>
#include <utility>
#include <map>
#include "../per_state_information.h"
#include "../global_state.h"
#include "../global_operator.h"
#include "../utils/rng.h"

class Heuristic;
class Options;

namespace RandomEdgeEvaluatorXor {
    class RandomEdgeEvaluatorXor : public ScalarEvaluator {
        PerStateInformation<int> state_db;
        std::map<const GlobalOperator*,int> edge_db;
        int bound;
    public:
        explicit RandomEdgeEvaluatorXor(const Options &options);
        virtual ~RandomEdgeEvaluatorXor() override = default;

        virtual EvaluationResult compute_result(
            EvaluationContext &eval_context) override;

        virtual void get_involved_heuristics(std::set<Heuristic *> &) override {}
    };
}


