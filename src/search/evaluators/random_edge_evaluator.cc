#include "random_edge_evaluator.h"

#include "../option_parser.h"
#include "../evaluation_context.h"
#include "../search_space.h"
#include "../plugin.h"
#include "../global_state.h"

#include <vector>
#include <cassert>
using namespace std;


// GlobalState current_state = goal_state;
// assert(path.empty());
// for (;;) {
//     const SearchNodeInfo &info = search_node_infos[current_state];
// const GlobalOperator *op = info.creating_operator;

namespace RandomEdgeEvaluator {
    RandomEdgeEvaluator::RandomEdgeEvaluator(const Options &opts) :
        ScalarEvaluator(), state_db(-1),
        bound((int)(numeric_limits<int>::max() * opts.get<double>("threshold"))) {}
    EvaluationResult RandomEdgeEvaluator::compute_result(EvaluationContext &ctx) {
        EvaluationResult result;
        auto current = ctx.get_state();
        int &state_value = state_db[current];
        if (state_value < 0){
            state_value = g_rng(std::numeric_limits<int>::max());
        }
        auto op = ctx.get_space()->search_node_infos[current].creating_operator;

        auto it = edge_db.find(op);
        int edge_value;
        if (it == edge_db.end()){
            edge_value = g_rng(std::numeric_limits<int>::max());
            edge_db[op] = edge_value;
        }else{
            edge_value = it->second;
        }

        int value = abs(state_value ^ edge_value);
        
        if (value > bound){
            value = EvaluationResult::INFTY;
        }
        
        result.set_h_value(value);
        return result;
    }
    
    static ScalarEvaluator *_parse(OptionParser &parser) {
        parser.document_synopsis("RandomEdge evaluator",
                                 "Calculates the random value for a node, based on the edge it comes from.");
        parser.add_option<double>(
            "threshold",
            "Threshold for random value. Mapped into 32bit integer space and "
            "the random value above the threshold is treated as infinite (pruned). "
            "Any value below 1.0 makes the algorithm incomplete.",
            "1.0",
            Bounds("0.0", "1.0"));
        Options opts = parser.parse();

        if (parser.dry_run())
            return 0;
        else
            return new RandomEdgeEvaluator(opts);
    }

    static Plugin<ScalarEvaluator> _plugin("random_edge", _parse);
}

