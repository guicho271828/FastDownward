#include "novelty.h"

#include "../option_parser.h"
#include "../plugin.h"
#include "combinations.h"

#include <vector>
#include <unordered_map>
#include <map>
#include <unordered_set>
#include <set>
#include <iostream>
using namespace std;

namespace Novelty {
    Novelty::Novelty(const Options &opts) : Heuristic(opts) {}

    Novelty::~Novelty() {}

    void Novelty::initialize() {
        cout << "Initializing novelty heuristics..." << endl;
    }
    class Novelty::is_novel {
        bool answer;
    public:
        explicit is_novel() : answer(false) {};
        operator bool() const {return answer;}
        template <class It>
        bool operator()(It first, It last){
            set<pair<VariableProxy,int>> atom_set(first,last);
            auto it = db.find(atom_set);
            if (it == db.end()){
                db.insert(atom_set);
                answer = true;
                return true;
            }else{
                return false;
            }
        }
    };

    int Novelty::compute_heuristic(const GlobalState &global_state) {
        const State state = convert_global_state(global_state);
        vector<pair<VariableProxy,int>> pairs(task_proxy.get_variables().size());
        for (VariableProxy var : task_proxy.get_variables()) {
            pairs.push_back(
                make_pair(var,
                          state[var].get_value()));}
        auto check_novelty = ;
        int w = 0;
        while (! for_each_combination(pairs.begin(), pairs.begin() + w, pairs.end(), is_novel())){
            w++;
        }
        return w;
    }
    
    static Heuristic *_parse(OptionParser &parser) {
        parser.document_synopsis("Novelty hash function", "");
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
            return new Novelty(opts);
    }


static Plugin<Heuristic> _plugin("novelty", _parse);
}
