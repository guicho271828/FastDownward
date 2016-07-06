/* -*- mode : c++ -*- */
#pragma once

#include "../scalar_evaluator.h"
#include <vector>
#include <utility>
#include "../per_state_information.h"
#include "../global_state.h"
#include "../global_operator.h"
#include "../utils/rng.h"

class Heuristic;
class Options;

namespace RandomEdgeEvaluator {
    class RandomEdgeEvaluator : public ScalarEvaluator {
        std::unordered_map<std::pair<StateID,const GlobalOperator*>,int> cache;

    public:
        explicit RandomEdgeEvaluator() = default;
        virtual ~RandomEdgeEvaluator() override = default;

        virtual EvaluationResult compute_result(
            EvaluationContext &eval_context) override;

        virtual void get_involved_heuristics(std::set<Heuristic *> &) override {}
    };
}


