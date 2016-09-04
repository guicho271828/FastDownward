#include "width_evaluator.h"

#include "../option_parser.h"
#include "../evaluation_context.h"
#include "../plugin.h"
#include "../global_state.h"
#include "../successor_generator.h"

#include <vector>
#include <cassert>


namespace WidthEvaluator {
    using namespace std;
    // WidthEvaluator::WidthEvaluator(const Options &opts) : ScalarEvaluator(opts) {
    // }
    // WidthEvaluator::WidthEvaluator() {
    // }
    EvaluationResult WidthEvaluator::compute_result(EvaluationContext &ctx) {
        EvaluationResult result;
        result.set_h_value(width(ctx));
        return result;
    }
    
    int WidthEvaluator::width(EvaluationContext &eval_context){
        vector<const GlobalOperator *> applicable_ops;
        g_successor_generator->generate_applicable_ops(
            eval_context.get_state(), applicable_ops);
        return applicable_ops.size();
    }
    
    static ScalarEvaluator *_parse(OptionParser &parser) {
        parser.document_synopsis("Width evaluator",
                                 "Calculates the width of the sub-evaluators. "
                                 "Width is 0 when sub-evaluators value are different "
                                 "between the given state and its parent.");

        // Options opts = parser.parse();

        if (parser.dry_run()){
            return 0;
        }
        else
        {
            return new WidthEvaluator();
            // return new WidthEvaluator(opts);
        }
    }

    static Plugin<ScalarEvaluator> _plugin("width", _parse);
}

