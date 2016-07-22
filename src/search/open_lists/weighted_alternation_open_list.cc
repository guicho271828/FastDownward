
#include "open_list.h"
#include "alternation_open_list.h"
#include "weighted_alternation_open_list.h"


#include "../option_parser.h"
#include "../plugin.h"

#include "../utils/memory.h"
#include "../utils/system.h"

#include <cassert>
#include <memory>
#include <vector>

using namespace std;
using Utils::ExitCode;


template<class Entry>
class WeightedAlternationOpenList : public AlternationOpenList<Entry> {
    vector<int> priorities;
    vector<int> weights;
    unsigned int counter;
protected:
    // std::vector<std::unique_ptr<OpenList<Entry>>> open_lists;
    // const int boost_amount;
    // int max_weight;
public:
    explicit WeightedAlternationOpenList(const Options &opts);
    virtual ~WeightedAlternationOpenList() override = default;

    virtual void boost_preferred() override;
    virtual Entry remove_min(vector<int> *key = nullptr) override;
};


template<class Entry>
WeightedAlternationOpenList<Entry>::WeightedAlternationOpenList(const Options &opts)
    : AlternationOpenList<Entry>(opts),
      weights(opts.get_list<int>("weights")),
      counter(0){
    priorities.resize(this->open_lists.size(), 0);
    weights.resize(this->open_lists.size(), 0);
}

template<class Entry>
Entry WeightedAlternationOpenList<Entry>::remove_min(vector<int> *key) {
    if (key) {
        cerr << "not implemented -- see msg639 in the tracker" << endl;
        Utils::exit_with(ExitCode::UNSUPPORTED);
    }

    bool selected = false;
    bool retried = false;
RETRY:
    for (size_t i = 0; i < this->open_lists.size(); ++i) {
        ++counter;
        if (counter == this->open_lists.size())
            counter = 0;
        if (this->open_lists[counter]->empty())
            continue;
        if (priorities[counter]==0)
            continue;
        selected = true;
        break;
    }
    if (!selected){
        priorities = weights;
        if (! retried){
            retried = true;
            goto RETRY;
        } else {
            Utils::exit_with(ExitCode::UNSOLVABLE);
        }
    }
    assert(!this->open_lists[counter]->empty());
    --priorities[counter];
    return this->open_lists[counter]->remove_min(nullptr);
}

template<class Entry>
void WeightedAlternationOpenList<Entry>::boost_preferred() {
    for (size_t i = 0; i < this->open_lists.size(); ++i)
        if (this->open_lists[i]->only_contains_preferred_entries())
            priorities[i] += this->boost_amount;
}


WeightedAlternationOpenListFactory::WeightedAlternationOpenListFactory(const Options &options)
    : options(options) {
}

unique_ptr<StateOpenList>
WeightedAlternationOpenListFactory::create_state_open_list() {
    return Utils::make_unique_ptr<WeightedAlternationOpenList<StateOpenListEntry>>(options);
}

unique_ptr<EdgeOpenList>
WeightedAlternationOpenListFactory::create_edge_open_list() {
    return Utils::make_unique_ptr<WeightedAlternationOpenList<EdgeOpenListEntry>>(options);
}

static shared_ptr<OpenListFactory> _parse(OptionParser &parser) {
    parser.document_synopsis("WeightedAlternation open list",
                             "alternates between several open lists.");
    parser.add_list_option<shared_ptr<OpenListFactory>>(
        "sublists",
        "open lists between which this one alternates");
    parser.add_list_option<int>(
        "weights",
        "list of weights for each open lists");
    parser.add_option<int>(
        "boost",
        "boost value for contained open lists that are restricted "
        "to preferred successors",
        "0");

    Options opts = parser.parse();
    opts.verify_list_non_empty<shared_ptr<OpenListFactory>>("sublists");
    if (parser.dry_run())
        return nullptr;
    else
        return make_shared<WeightedAlternationOpenListFactory>(opts);
}

static PluginShared<OpenListFactory> _plugin("walt", _parse);
