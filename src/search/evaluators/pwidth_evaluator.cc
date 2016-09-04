#include "pwidth_evaluator.h"

#include "../option_parser.h"
#include "../evaluation_context.h"
#include "../plugin.h"
#include "../global_state.h"
#include "../search_space.h"
#include "../successor_generator.h"

#include <vector>
#include <cassert>


namespace PwidthEvaluator {
    using namespace std;
    // PwidthEvaluator::PwidthEvaluator(const Options &opts) : ScalarEvaluator(opts) {
    // }
    // PwidthEvaluator::PwidthEvaluator() {
    // }
    EvaluationResult PwidthEvaluator::compute_result(EvaluationContext &ctx) {
        EvaluationResult result;
        result.set_h_value(pwidth(ctx));
        return result;
    }
    
    int PwidthEvaluator::pwidth(EvaluationContext &eval_context){
        const GlobalState &current = eval_context.get_state();
        if(current.get_id() == g_initial_state().get_id()){
            return 0;
        }else{
            assert(eval_context.get_space());
            StateID parent_id = eval_context.get_space()->parent(current);
            if (parent_id == StateID::no_state){
                return 0;
            } else {
                vector<const GlobalOperator *> applicable_ops;
                const GlobalState &parent = g_state_registry->lookup_state(parent_id);
                g_successor_generator->generate_applicable_ops(
                    parent, applicable_ops);
                return applicable_ops.size();
            }
        }
    }
    
    static ScalarEvaluator *_parse(OptionParser &parser) {
        parser.document_synopsis("Pwidth evaluator",
                                 "Calculates the pwidth of the sub-evaluators. "
                                 "Pwidth is 0 when sub-evaluators value are different "
                                 "between the given state and its parent.");

        // Options opts = parser.parse();

        if (parser.dry_run()){
            return 0;
        }
        else
        {
            return new PwidthEvaluator();
            // return new PwidthEvaluator(opts);
        }
    }

    static Plugin<ScalarEvaluator> _plugin("pwidth", _parse);
}

