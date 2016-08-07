/* -*- mode : c++ -*- */
#pragma once

#include "open_list_factory.h"

#include "../option_parser_util.h"

template<class Entry>
class LayeredOpenList : public OpenList<Entry> {
    // Openlist that can be layered.
    // Can maintain a sublist
    
    /*
      If allow_unsafe_pruning is true, we ignore (don't insert) states
      which the first evaluator considers a dead end, even if it is
      not a safe heuristic.
    */
    bool allow_unsafe_pruning;

protected:
    int size;
    virtual OpenList<Entry>& get_sublist(EvaluationContext &eval_context, const Entry &entry);
    virtual void do_insertion(EvaluationContext &eval_context,
                              const Entry &entry) override;
public:
    explicit LayeredOpenList(const Options &opts);
    virtual ~LayeredOpenList() override = default;

    virtual Entry remove_min(std::vector<int> *key = nullptr) override;
    virtual const OpenList<Entry>& find_min(std::vector<int> *key = nullptr);
    virtual bool empty() const override;
    virtual void clear() override;
};

