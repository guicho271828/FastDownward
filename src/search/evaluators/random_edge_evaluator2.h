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

namespace RandomEdgeEvaluator2 {
    class RandomEdgeEvaluator2 : public ScalarEvaluator {
        PerStateInformation<int> state_db;
        std::map<const GlobalOperator*,int> edge_db;

    public:
        explicit RandomEdgeEvaluator2(const Options &options);
        virtual ~RandomEdgeEvaluator2() override = default;

        virtual EvaluationResult compute_result(
            EvaluationContext &eval_context) override;

        virtual void get_involved_heuristics(std::set<Heuristic *> &) override {}
    };
}


