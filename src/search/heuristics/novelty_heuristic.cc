#include "novelty_heuristic.h"

#include "../global_state.h"
#include "../option_parser.h"
#include "../plugin.h"
#include "../task_tools.h"
#include "../evaluation_context.h"
#include "../evaluation_result.h"
#include "../utils/logging.h"
#include <cstddef>
#include <limits>
#include <utility>

using namespace std;

namespace NoveltyHeuristic {
    using Key = std::vector<int>;
    NoveltyHeuristic::NoveltyHeuristic(const Options &opts)
        : Heuristic(opts),
          atoms(opts.get<int>("atoms")),
          subevaluators(opts.get_list<ScalarEvaluator *>("evals")) {
    }

    NoveltyHeuristic::~NoveltyHeuristic() {
    }

    void NoveltyHeuristic::initialize() {
        cout << "Initializing novelty heuristic..." << endl;
    }

    Key NoveltyHeuristic::key(EvaluationContext &eval_context){
        Key values;
        values.reserve(subevaluators.size());
        // Collect component values.
        for (ScalarEvaluator *subevaluator : subevaluators) {
            values.push_back(eval_context.get_heuristic_value_or_infinity(subevaluator));
        }
        return values;
    }

    EvaluationResult NoveltyHeuristic::compute_result(EvaluationContext &eval_context) {
        tmp_context = &eval_context;
        auto result = Heuristic::compute_result(eval_context);
        tmp_context = nullptr;
        return result;
    }
    int NoveltyHeuristic::compute_heuristic(const GlobalState &global_state) {
        State state = convert_global_state(global_state);
        Key key;
        key.reserve(subevaluators.size());
        // Collect component values.
        for (ScalarEvaluator *subevaluator : subevaluators) {
            key.push_back(tmp_context->get_heuristic_value_or_infinity(subevaluator));
        }
        // cout << key << endl;
        auto &plateau = plateaus[key];

        auto vars = task_proxy.get_variables();
        Conjunction direct_state;
        direct_state.reserve(vars.size());
        Conjunction stack;
        stack.reserve(vars.size());
        for (VariableProxy var : vars) {
            FactProxy fact = state[var];
            direct_state.push_back(fact);
        }
        // cout << direct_state << endl;
        for (int i = 0; i < atoms; i++){
            vector<Conjunction> conjunctions;
            collect_tuples(i, direct_state.begin(), conjunctions, stack, direct_state);
            // cout << conjunctions << endl;
            bool flag = false;
            for (auto &c : conjunctions){
                if(plateau.find(c)==plateau.end()){
                    // the conjunction is new
                    flag = true;
                }
            }
            // cout << "before(" << plateau.size() << "pairs): " << plateau << endl;
            plateau.insert(conjunctions.begin(),conjunctions.end());
            // cout << "after(" << plateau.size() << "pairs): " << plateau << endl;
            // cout << "flag: " << flag << " i:" <<i<< endl;
            if (flag){
                return i;
            }
        }
        return atoms;
    }
    
    void NoveltyHeuristic::collect_tuples (int remaining,
                                           Conjunction::iterator begin,
                                           vector<Conjunction> &cs,
                                           Conjunction &stack,
                                           Conjunction &direct_state){
        for (auto it=begin; it!=direct_state.end(); it++){
            FactProxy fact = *it;
            stack.push_back(fact);
            if (remaining == 0){
                cs.push_back(stack);
            }else{
                collect_tuples(remaining-1,it+1,cs,stack,direct_state);
            }
            stack.pop_back();
        }
        return;
    }

    static Heuristic *_parse(OptionParser &parser) {
        parser.document_synopsis("Novelty heuristic",
                                 "Returns cost of cheapest action for "
                                 "non-goal states, "
                                 "0 for goal states");
        parser.document_language_support("action costs", "supported");
        parser.document_language_support("conditional effects", "supported");
        parser.document_language_support("axioms", "supported");
        parser.document_property("admissible", "yes");
        parser.document_property("consistent", "yes");
        parser.document_property("safe", "yes");
        parser.document_property("preferred operators", "no");
    
        parser.add_list_option<ScalarEvaluator *>("evals",
                                                  "at least one scalar evaluator");
        parser.add_option<int>("atoms","max number of atom pairs to consider","2");
        // no
        // opts.verify_list_non_empty<ScalarEvaluator *>("evals");

        Heuristic::add_options_to_parser(parser);
        Options opts = parser.parse();
        if (parser.dry_run())
            return 0;
        else
            return new NoveltyHeuristic(opts);
    }

    static Plugin<Heuristic> _plugin("novelty", _parse);
}
