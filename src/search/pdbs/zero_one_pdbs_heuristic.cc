#include "zero_one_pdbs_heuristic.h"

#include "pattern_database.h"
#include "pattern_generator.h"
#include "util.h"

#include "../evaluation_context.h"
#include "../option_parser.h"
#include "../plugin.h"
#include "../utilities.h"

#include <vector>

using namespace std;

ZeroOnePDBsHeuristic::ZeroOnePDBsHeuristic(
    const Options &opts,
    const vector<int> &op_costs)
    : Heuristic(opts) {
    vector<int> operator_costs;
    OperatorsProxy operators = task_proxy.get_operators();

    // If no operator costs are specified, use default operator costs.
    if (op_costs.empty()) {
        operator_costs.reserve(operators.size());
        for (OperatorProxy op : operators)
            operator_costs.push_back(op.get_cost());
    } else {
        assert(op_costs.size() == operators.size());
        operator_costs = op_costs;
    }

    shared_ptr<PatternCollectionGenerator> pattern_generator =
        opts.get<shared_ptr<PatternCollectionGenerator>>("patterns");
    PatternCollection pattern_collection = pattern_generator->generate(task);
    shared_ptr<Patterns> patterns = pattern_collection.get_patterns();

    //Timer timer;
    approx_mean_finite_h = 0;
    pattern_databases.reserve(patterns->size());
    for (const vector<int> &pattern : *patterns) {
        unique_ptr<PatternDatabase> pdb = make_unique_ptr<PatternDatabase>(
            task_proxy, pattern, false, operator_costs);

        /* Set cost of relevant operators to 0 for further iterations
           (action cost partitioning). */
        for (OperatorProxy op : operators) {
            if (pdb->is_operator_relevant(op))
                operator_costs[op.get_id()] = 0;
        }

        approx_mean_finite_h += pdb->compute_mean_finite_h();

        pattern_databases.push_back(move(pdb));
    }
    //cout << "All or nothing PDB collection construction time: " <<
    //timer << endl;
}

ZeroOnePDBsHeuristic::~ZeroOnePDBsHeuristic() {
}

int ZeroOnePDBsHeuristic::compute_heuristic(const GlobalState &global_state) {
    /*
      Because we use cost partitioning, we can simply add up all
      heuristic values of all patterns in the pattern collection.
    */
    State state = convert_global_state(global_state);
    int h_val = 0;
    for (const auto &pdb : pattern_databases) {
        int pdb_value = pdb->get_value(state);
        if (pdb_value == numeric_limits<int>::max())
            return DEAD_END;
        h_val += pdb_value;
    }
    return h_val;
}

void ZeroOnePDBsHeuristic::dump() const {
    for (const auto &pdb : pattern_databases) {
        cout << pdb->get_pattern() << endl;
    }
}

static Heuristic *_parse(OptionParser &parser) {
    parser.document_synopsis(
        "Zero-One PDB",
        "The zero/one pattern database heuristic is simply the sum of the "
        "heuristic values of all patterns in the pattern collection. In contrast "
        "to the canonical pattern database heuristic, there is no need to check "
        "for additive subsets, because the additivity of the patterns is "
        "guaranteed by action cost partitioning. This heuristic uses the most "
        "simple form of action cost partitioning, i.e. if an operator affects "
        "more than one pattern in the collection, its costs are entirely taken "
        "into account for one pattern (the first one which it affects) and set "
        "to zero for all other affected patterns.");
    parser.document_language_support("action costs", "supported");
    parser.document_language_support("conditional effects", "not supported");
    parser.document_language_support("axioms", "not supported");
    parser.document_property("admissible", "yes");
    parser.document_property("consistent", "yes");
    parser.document_property("safe", "yes");
    parser.document_property("preferred operators", "no");

    parser.add_option<shared_ptr<PatternCollectionGenerator>>(
        "patterns",
        "pattern generation method",
        "combo()");
    Heuristic::add_options_to_parser(parser);

    Options opts = parser.parse();
    if (parser.dry_run())
        return 0;

    return new ZeroOnePDBsHeuristic(opts);
}

static Plugin<Heuristic> _plugin("zopdbs", _parse);
