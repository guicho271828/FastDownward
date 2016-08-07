#include "layered_open_list.h"

#include "open_list.h"

#include "../option_parser.h"
#include "../plugin.h"

#include "../utils/memory.h"

using namespace std;

template<class Entry>
LayeredOpenList<Entry>::LayeredOpenList(const Options &opts)
    : OpenList<Entry>(opts.get<bool>("pref_only"),QueueType(opts.get_enum("queue_type"))),
      allow_unsafe_pruning(opts.get<bool>("unsafe_pruning")),
      size(0) {
}

template<class Entry>
void LayeredOpenList<Entry>::do_insertion(
    EvaluationContext &eval_context, const Entry &entry) {
    get_sublist(eval_context, entry)
        .do_insertion(eval_context, entry);
    ++size;
}

template<class Entry>
Entry LayeredOpenList<Entry>::remove_min(vector<int> *key) {
    assert(size > 0);
    --size;
    OpenList<Entry>& list = find_min(key);
    Entry result = list.remove_min(key);
    // if (list.empty()){
    //     list.erase(it);
    // }
    return result;
}

