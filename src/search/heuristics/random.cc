#include "random.h"

#include "../option_parser.h"
#include "../plugin.h"

#include <iostream>
using namespace std;

namespace RandomHeuristics {
RandomHeuristics::RandomHeuristics(const Options &opts)
    : Heuristic(opts),
      db(-1)
{
}

RandomHeuristics::~RandomHeuristics() {
}

void RandomHeuristics::initialize() {
    cout << "Initializing random heuristics..." << endl;
}

int RandomHeuristics::compute_heuristic(const GlobalState &global_state) {
    int &var = db[global_state];
    if (var < 0){
        var = g_rng.next32();
    }
    return var;
}

static Heuristic *_parse(OptionParser &parser) {
    parser.document_synopsis("Zobrist hash function", "");
    parser.document_language_support("action costs", "ignored by design");
    parser.document_language_support("conditional effects", "supported");
    parser.document_language_support("axioms", "supported");
    parser.document_property("admissible", "no");
    parser.document_property("consistent", "no");
    parser.document_property("safe", "yes");
    parser.document_property("preferred operators", "no");

    Heuristic::add_options_to_parser(parser);
    Options opts = parser.parse();
    if (parser.dry_run())
        return 0;
    else
        return new RandomHeuristics(opts);
}


static Plugin<Heuristic> _plugin("random", _parse);
}
