/* -*- mode : c++ -*- */
#ifndef OPEN_LISTS_TIEBREAKING_OPEN_LIST_H
#define OPEN_LISTS_TIEBREAKING_OPEN_LIST_H

#include "open_list_factory.h"

#include "../option_parser_util.h"

#include <deque>
#include <map>
#include <vector>

template<class Entry>
using Bucket = std::deque<Entry>;

template<class Entry>
using Buckets = std::map<const std::vector<int>, Bucket<Entry>>;

template<class Entry>
class TieBreakingOpenList : public OpenList<Entry> {

    std::vector<ScalarEvaluator *> evaluators;
    /*
      If allow_unsafe_pruning is true, we ignore (don't insert) states
      which the first evaluator considers a dead end, even if it is
      not a safe heuristic.
    */
    bool allow_unsafe_pruning;

    int dimension() const;

protected:
    Buckets<Entry> buckets;
    int size;
    virtual void do_insertion(EvaluationContext &eval_context,
                              const Entry &entry) override;
public:
    explicit TieBreakingOpenList(const Options &opts);
    virtual ~TieBreakingOpenList() override = default;

    virtual Entry remove_min(std::vector<int> *key = nullptr) override;
    virtual bool empty() const override;
    virtual void clear() override;
    virtual void get_involved_heuristics(std::set<Heuristic *> &hset) override;
    virtual bool is_dead_end(
        EvaluationContext &eval_context) const override;
    virtual bool is_reliable_dead_end(
        EvaluationContext &eval_context) const override;
    virtual typename Buckets<Entry>::iterator get_bucket(std::vector<int> *key);
};



class TieBreakingOpenListFactory : public OpenListFactory {
    Options options;
public:
    explicit TieBreakingOpenListFactory(const Options &options);
    virtual ~TieBreakingOpenListFactory() override = default;

    virtual std::unique_ptr<StateOpenList> create_state_open_list() override;
    virtual std::unique_ptr<EdgeOpenList> create_edge_open_list() override;
};

#endif
