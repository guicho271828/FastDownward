/* -*- mode : c++ -*- */
#pragma once

#include "../scalar_evaluator.h"
#include <vector>
#include "../per_state_information.h"
#include "../global_state.h"

class Heuristic;
class Options;

namespace WidthEvaluator {
    class WidthEvaluator : public ScalarEvaluator {
        
        int width(EvaluationContext &eval_context);

    public:
        // explicit WidthEvaluator(const Options &opts);
        virtual ~WidthEvaluator() override = default;

        virtual EvaluationResult compute_result(
            EvaluationContext &eval_context) override;

        virtual void get_involved_heuristics(std::set<Heuristic *> &) override {}
    };
}

