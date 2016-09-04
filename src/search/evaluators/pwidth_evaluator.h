/* -*- mode : c++ -*- */
#pragma once

#include "../scalar_evaluator.h"
#include <vector>
#include "../per_state_information.h"
#include "../global_state.h"

class Heuristic;
class Options;

namespace PwidthEvaluator {
    class PwidthEvaluator : public ScalarEvaluator {
        
        int pwidth(EvaluationContext &eval_context);

    public:
        // explicit PwidthEvaluator(const Options &opts);
        virtual ~PwidthEvaluator() override = default;

        virtual EvaluationResult compute_result(
            EvaluationContext &eval_context) override;

        virtual void get_involved_heuristics(std::set<Heuristic *> &) override {}
    };
}

