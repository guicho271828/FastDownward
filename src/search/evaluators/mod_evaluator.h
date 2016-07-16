#ifndef EVALUATORS_MOD_EVALUATOR_H
#define EVALUATORS_MOD_EVALUATOR_H

#include "../scalar_evaluator.h"

class Options;


namespace ModEvaluator {
class ModEvaluator : public ScalarEvaluator {
    ScalarEvaluator *evaluator;
    int w;

public:
    explicit ModEvaluator(const Options &opts);
    ModEvaluator(ScalarEvaluator *eval, int mod);
    virtual ~ModEvaluator() override;

    virtual bool dead_ends_are_reliable() const override;
    virtual EvaluationResult compute_result(
        EvaluationContext &eval_context) override;
    virtual void get_involved_heuristics(std::set<Heuristic *> &hset) override;
};
}

#endif
