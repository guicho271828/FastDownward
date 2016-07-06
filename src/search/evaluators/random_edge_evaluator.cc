#include "random_edge_evaluator.h"

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

namespace RandomEdgeEvaluator {
    EvaluationResult RandomEdgeEvaluator::compute_result(EvaluationContext &ctx) {
        EvaluationResult result;
        auto current = ctx.get_state();
        auto key = std::make_pair(
            current.get_id(),
            ctx.get_space()->search_node_infos[current].creating_operator);
        auto it = cache.find(key);
        if (it == cache.end()){
            int r = g_rng.next32();
            cache[key] = r;
            result.set_h_value(r);
        }else{
            result.set_h_value(it->second);
        }
        return result;
    }
    
    static ScalarEvaluator *_parse(OptionParser &parser) {
        parser.document_synopsis("RandomEdge evaluator",
                                 "Calculates the random value for a node, based on the edge it comes from.");

        Options opts = parser.parse();

        if (parser.dry_run())
            return 0;
        else
            return new RandomEdgeEvaluator;
    }

    static Plugin<ScalarEvaluator> _plugin("random_edge", _parse);
}

