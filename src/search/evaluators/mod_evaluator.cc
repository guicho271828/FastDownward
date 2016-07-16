#include "mod_evaluator.h"

#include "../evaluation_context.h"
#include "../evaluation_result.h"
#include "../option_parser.h"
#include "../plugin.h"

#include <cstdlib>
#include <sstream>


namespace ModEvaluator {
ModEvaluator::ModEvaluator(const Options &opts)
    : evaluator(opts.get<ScalarEvaluator *>("eval")),
      w(opts.get<int>("mod")) {
}

ModEvaluator::ModEvaluator(ScalarEvaluator *eval, int mod)
    : evaluator(eval), w(mod) {
}


ModEvaluator::~ModEvaluator() {
}

bool ModEvaluator::dead_ends_are_reliable() const {
    return evaluator->dead_ends_are_reliable();
}

EvaluationResult ModEvaluator::compute_result(
    EvaluationContext &eval_context) {
    // Note that this produces no preferred operators.
    EvaluationResult result;
    int h_val = eval_context.get_heuristic_value_or_infinity(evaluator);
    if (h_val != EvaluationResult::INFTY) {
        // TODO: Check for overflow?
        h_val = h_val % w;
    }
    result.set_h_value(h_val);
    return result;
}

void ModEvaluator::get_involved_heuristics(std::set<Heuristic *> &hset) {
    evaluator->get_involved_heuristics(hset);
}

static ScalarEvaluator *_parse(OptionParser &parser) {
    parser.document_synopsis(
        "Mod evaluator",
        "Take the remainder of the heuristic value divided by mod.");
    parser.add_option<ScalarEvaluator *>("eval", "scalar evaluator");
    parser.add_option<int>("mod", "mod");
    Options opts = parser.parse();
    if (parser.dry_run())
        return 0;
    else
        return new ModEvaluator(opts);
}

static Plugin<ScalarEvaluator> _plugin("mod", _parse);
}
