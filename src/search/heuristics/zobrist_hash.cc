#include "zobrist_hash.h"

#include "../option_parser.h"
#include "../plugin.h"

#include <iostream>
using namespace std;

namespace ZobristHash {
ZobristHash::ZobristHash(const Options &opts)
    : Heuristic(opts) {
}

ZobristHash::~ZobristHash() {
}

void ZobristHash::initialize() {
    cout << "Initializing zobrist hash..." << endl;
    map.resize(task_proxy.get_variables().size());
    int i = 0;
    for (VariableProxy var : task_proxy.get_variables()) {
        map[i].resize(var.get_domain_size());
        for (unsigned int &bitstring : map[i]){
            bitstring = g_rng.next32();
        }
        i++;
    }
}

int ZobristHash::compute_heuristic(const GlobalState &global_state) {
    const State state = convert_global_state(global_state);
    unsigned int r = 0;
    int i = 0;
    for (VariableProxy var : task_proxy.get_variables()) {
        FactProxy fact = state[var];
        r = r ^ map[i][fact.get_value()];
        i++;
    }
    return r;
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
        return new ZobristHash(opts);
}


static Plugin<Heuristic> _plugin("zobrist", _parse);
}
