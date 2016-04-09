
#include "softmax.h"

#include "../../utils/rng.h"

#include "../debug.h"

#include <cassert>
#include <memory>
#include <vector>
#include <map>

using namespace std;
using namespace bandit;
using Utils::ExitCode;

template<class Entry>
SoftmaxOpenList<Entry>::SoftmaxOpenList(const Options &opts)
    : BanditOpenList<Entry>(opts),
      t(opts.get<double>("t")),
      pull_once(opts.get<bool>("pull_once")) {
    scores.resize(this->open_lists.size(), 0);
    pulls.resize(this->open_lists.size(), 0);
    best_values.resize(this->open_lists.size(),
                       numeric_limits<int>::max());
    first_values.resize(this->open_lists.size(),
                        numeric_limits<int>::max());
}

template<class Entry>
void SoftmaxOpenList<Entry>::reward(EvaluationContext &eval_context) {
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
            break;
        default:
            assert(false);
        }
    }
}

template<class Entry>
double SoftmaxOpenList<Entry>::lever_score(int j) {
    if (this->open_lists[j]->empty()){
        return 0;
    }else{
        if (pulls[j]==0){
            return numeric_limits<double>::infinity();
        }else{
            return exp((scores[j]/pulls[j])/t);
        }
    }
}


template<class Entry>
size_t SoftmaxOpenList<Entry>::pull() {
    just_pulled = true;
    vector<double> pr(this->open_lists.size());
    map<double,int> pr_map;
    vector<int> infinities;
    infinities.reserve(this->open_lists.size());

    double cumulative = 0.0;
    for (size_t i = 0; i < this->open_lists.size(); ++i) {
        double boltzmann = lever_score(i);
        if (boltzmann == numeric_limits<double>::infinity()){
            infinities.push_back(i);
        }
        cumulative += boltzmann;
        pr[i] = cumulative;
        pr_map[cumulative] = i;
    }
    assert(cumulative>0.0);
    if (!infinities.empty()){
        return *(g_rng.choose(infinities));
    }else{
        // all finitie
        uniform_real_distribution<double> distribution(0.0, cumulative);
        double picked = distribution(rng);
        auto it = pr_map.upper_bound(picked);
        assert(it!=pr_map.end());
        return it->second;
    }
}

// ------------------- options -----------------------

static shared_ptr<OpenListFactory> _parse(OptionParser &parser) {
    parser.document_synopsis("Softmax bandit open list",
                             "alternates between several open lists based on e-greedy bandit.");
    parser.add_list_option<ScalarEvaluator *>("evals", "heuristics");
    parser.add_option<double>("t", "temparature", "100");
    
    Options opts = add_bandit_option_to_parser(parser);
    if (parser.dry_run())
        return nullptr;
    else
        return make_shared<SoftmaxOpenListFactory>(opts);
}

static PluginShared<OpenListFactory> _plugin("softmax", _parse);

