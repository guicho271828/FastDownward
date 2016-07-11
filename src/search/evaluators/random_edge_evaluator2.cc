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
    RandomEdgeEvaluator2(const Options &opts) : ScalarEvaluator(), state_db(-1), edge_db(-1) {}
    EvaluationResult compute_result(EvaluationContext &ctx) {
        EvaluationResult result;
        auto current = ctx.get_state();
        int &state_var = state_db[current];
        if (state_var < 0){
            state_var = g_rng.next32();
        }
        auto op = ctx.get_space()->search_node_infos[current].creating_operator;
        int &edge_var = edge_db[op];
        if (edge_var < 0){
            edge_var = g_rng.next32();
        }
        
        result.set_h_value(state_var ^ edge_var);
        return result;
    }
    
    static ScalarEvaluator *_parse(OptionParser &parser) {
        parser.document_synopsis("RandomEdge evaluator",
                                 "Calculates the random value for a node, based on the edge it comes from.");

        Options opts = parser.parse();

        if (parser.dry_run())
            return 0;
        else
            return new RandomEdgeEvaluator2(opts);
    }

    static Plugin<ScalarEvaluator> _plugin("random_edge", _parse);
}

