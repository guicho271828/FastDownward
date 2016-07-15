#include "random.h"

#include "../option_parser.h"
#include "../plugin.h"

#include <iostream>
#include <limits>
#include <cassert>
using namespace std;

namespace RandomHeuristics {
RandomHeuristics::RandomHeuristics(const Options &opts)
    : Heuristic(opts),
      bound((int)(numeric_limits<int>::max() * opts.get<double>("threshold"))),
      db(-1)
{
}

RandomHeuristics::~RandomHeuristics() {
}

void RandomHeuristics::initialize() {
    cout << "Initializing random heuristics..." << endl;
    cout << "Bound is " << bound << endl;
    cout << "Max integer is " << numeric_limits<int>::max() << endl;
}

int RandomHeuristics::compute_heuristic(const GlobalState &global_state) {
    int &var = db[global_state];
    if (var < 0){
        var = g_rng(std::numeric_limits<int>::max());
        assert(var > 0);
        if (var > bound){
            var = EvaluationResult::INFTY;
        }
    }
    return var;
}

static Heuristic *_parse(OptionParser &parser) {
    parser.document_synopsis("Random value function", "");
    parser.document_language_support("action costs", "ignored by design");
    parser.document_language_support("conditional effects", "supported");
    parser.document_language_support("axioms", "supported");
    parser.document_property("admissible", "no");
    parser.document_property("consistent", "no");
    parser.document_property("safe", "no when threshold is below 1.0");
    parser.document_property("preferred operators", "no");

    parser.add_option<double>(
        "threshold",
        "Threshold for random value. Mapped into 32bit integer space and "
        "the random value above the threshold is treated as infinite (pruned). "
        "Any value below 1.0 makes the algorithm incomplete.",
        "1.0",
        Bounds("0.0", "1.0"));
    Heuristic::add_options_to_parser(parser);

    Options opts = parser.parse();
    if (parser.dry_run())
        return 0;
    else
        return new RandomHeuristics(opts);
}


static Plugin<Heuristic> _plugin("random", _parse);
}
