#include "random_edge_evaluator2.h"

#include "../option_parser.h"
#include "../evaluation_context.h"
#include "../search_space.h"
#include "../plugin.h"
#include "../global_state.h"

#include <vector>
#include <cassert>


// GlobalState current_state = goal_state;
// assert(path.empty());
// for (;;) {
//     const SearchNodeInfo &info = search_node_infos[current_state];
// const GlobalOperator *op = info.creating_operator;

namespace RandomEdgeEvaluator2 {
    RandomEdgeEvaluator2(const Options &opts) :
        ScalarEvaluator(), state_db(-1), edge_db(-1),
        bound((int)(numeric_limits<int>::max() * opts.get<double>("threashold"))) {}
    EvaluationResult compute_result(EvaluationContext &ctx) {
        EvaluationResult result;
        auto current = ctx.get_state();
        int &state_value = state_db[current];
        if (state_value < 0){
            state_value = g_rng.next32();
        }
        auto op = ctx.get_space()->search_node_infos[current].creating_operator;
        int &edge_value = edge_db[op];
        if (edge_value < 0){
            edge_value = g_rng.next32();
        }
        int value = state_value ^ edge_value;

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
            "threashold",
            "Threashold for random value. Mapped into 32bit integer space and "
            "the random value above the threashold is treated as infinite (pruned). "
            "Any value below 1.0 makes the algorithm incomplete.",
            "1.0",
            Bounds("0.0", "1.0"));
        Options opts = parser.parse();

        if (parser.dry_run())
            return 0;
        else
            return new RandomEdgeEvaluator2(opts);
    }

    static Plugin<ScalarEvaluator> _plugin("random_edge", _parse);
}

