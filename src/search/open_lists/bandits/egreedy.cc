
#include "egreedy.h"

#include "../../utils/rng.h"

#include "../debug.h"

#include <cassert>
#include <memory>
#include <vector>

using namespace std;
using namespace bandit;
using Utils::ExitCode;

template<class Entry>
EGreedyOpenList<Entry>::EGreedyOpenList(const Options &opts)
    : BanditOpenList<Entry>(opts),
      epsilon(opts.get<double>("epsilon")),
      pull_once(opts.get<bool>("pull_once")) {
    scores.resize(this->open_lists.size(), 0);
    pulls.resize(this->open_lists.size(), 0);
    best_values.resize(this->open_lists.size(),
                       numeric_limits<int>::max());
    first_values.resize(this->open_lists.size(),
                        numeric_limits<int>::max());
}

template<class Entry>
void EGreedyOpenList<Entry>::reward(EvaluationContext &eval_context) {
    int i = this->last_pulled;
    if (!pull_once || just_pulled){
        ++pulls[i];
        just_pulled = false;
    }
    const auto &sublist = this->open_lists[i];
    int key = eval_context.get_heuristic_value(sublist->evaluator);
    if(best_values[i] > key){
        vector<double> expected;
        for (int j = 0; j < this->open_lists.size(); ++j){
            expected.push_back(lever_score(j));
        }
        auto h = static_cast<Heuristic *>(sublist->evaluator);
        if (h){
            cout << " rewarded " << h->get_description()
                 << " (" << best_values[i] << " -> " << key << ")"
                 << " S " << scores
                 << " P " << pulls
                 << " E " << expected
                 << endl;
        }
        switch (this->reward_type){
        case RewardType::NORMAL:
            if (best_values[i] == numeric_limits<int>::max()){
                first_values[i] = key;
                best_values[i] = key;
            }
            scores[i]+=best_values[i] - key;
            best_values[i] = key;
            break;
        case RewardType::RELATIVE:
            if (best_values[i] == numeric_limits<int>::max()){
                first_values[i] = key;
                best_values[i] = key;
            }
            scores[i]+=(double)(best_values[i] - key)/(double)(first_values[i]);
            best_values[i] = key;
            break;
        case RewardType::ONE: 
            best_values[i] = key;
            scores[i]+=1.0;
        }
    }
}

template<class Entry>
double EGreedyOpenList<Entry>::lever_score(int j) {
    return scores[j]/pulls[j];
}


template<class Entry>
size_t EGreedyOpenList<Entry>::pull() {
    just_pulled = true;
    if (g_rng() < epsilon) {
        vector<int> indices;
        for (int i = 0; i < this->open_lists.size(); ++i) {
            if (!this->open_lists[i]->empty()){
                indices.push_back(i);
            }
        }
        assert(!indices.empty());
        return *(g_rng.choose(indices));
    }else{
        int best = -1;
        double best_mean;
        for (size_t i = 0; i < this->open_lists.size(); ++i) {
            if (!this->open_lists[i]->empty()){
                double mean = lever_score(i);
                if (best == -1 || mean > best_mean){
                    best = i;
                    best_mean = mean;
                }
            }
        }
        return best;
    }
}

// ------------------- options -----------------------

static shared_ptr<OpenListFactory> _parse(OptionParser &parser) {
    parser.document_synopsis("EGreedy bandit open list",
                             "alternates between several open lists based on e-greedy bandit.");
    parser.add_list_option<ScalarEvaluator *>("evals", "heuristics");
    parser.add_option<double>(
        "epsilon",
        "probability for choosing the next open list randomly",
        "0.2",
        Bounds("0.0", "1.0"));
    
    Options opts = add_bandit_option_to_parser(parser);
    if (parser.dry_run())
        return nullptr;
    else
        return make_shared<EGreedyOpenListFactory>(opts);
}

static PluginShared<OpenListFactory> _plugin("eg", _parse);

