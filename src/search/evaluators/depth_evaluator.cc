#include "depth_evaluator.h"

#include "../option_parser.h"
#include "../evaluation_context.h"
#include "../search_space.h"
#include "../plugin.h"

#include <vector>


namespace DepthEvaluator {
    DepthEvaluator::DepthEvaluator(const Options &opts)
        : subevaluators(opts.get_list<ScalarEvaluator *>("evals")) {
    }

    Key DepthEvaluator::key(EvaluationContext &eval_context){
        Key values;
        values.reserve(subevaluators.size());
        // Collect component values.
        for (ScalarEvaluator *subevaluator : subevaluators) {
            values.push_back(eval_context.get_heuristic_value_or_infinity(subevaluator));
        }
        return values;
    }

    EvaluationResult DepthEvaluator::compute_result(EvaluationContext &ctx) {
        EvaluationResult result;
        auto &info = db[ctx.get_state()];

        if(!info.initialized){
            info.initialized = true;
            info.key = key(ctx);
            info.depth = depth(ctx);
        }
        
        result.set_h_value(info.depth);
        return result;
    }
    
    int DepthEvaluator::depth(EvaluationContext &eval_context){
        const GlobalState &current = eval_context.get_state();
        return
            depth(
                current,
                eval_context.get_space()->parent(current));
    }

    int DepthEvaluator::depth(const GlobalState &current,
                              const GlobalState &parent){
        if(current.get_id() == g_initial_state().get_id()){
            return 0;
        }else{ 
            if ( db[parent].key != db[current].key ){
                return 0;
            }else {
                return db[parent].depth+1;
            }
        }
    }
    
    static ScalarEvaluator *_parse(OptionParser &parser) {
        parser.document_synopsis("Depth evaluator",
                                 "Calculates the depth of the sub-evaluators. "
                                 "Depth is 0 when sub-evaluators value are different "
                                 "between the given state and its parent.");

        parser.add_list_option<ScalarEvaluator *>("evals",
                                                  "at least one scalar evaluator");
        Options opts = parser.parse();

        opts.verify_list_non_empty<ScalarEvaluator *>("evals");

        if (parser.dry_run())
            return 0;
        else
            return new DepthEvaluator(opts);
    }

    static Plugin<ScalarEvaluator> _plugin("depth", _parse);
}

