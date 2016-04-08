/* -*- mode : c++ -*- */

#pragma once

#include "open_list_factory.h"

#include "../option_parser_util.h"


/*
  Open list indexed by a single int, using FIFO tie-breaking.

  Implemented as a map from int to deques.
*/

template<class Entry>
class StandardScalarOpenList : public OpenList<Entry> {
    typedef std::deque<Entry> Bucket;

    std::map<int, Bucket> buckets;
    int size;

protected:
    virtual void do_insertion(EvaluationContext &eval_context,
                              const Entry &entry) override;

public:
    ScalarEvaluator *evaluator;
    explicit StandardScalarOpenList(const Options &opts);
    StandardScalarOpenList(ScalarEvaluator *eval,
                           bool preferred_only);
    virtual ~StandardScalarOpenList() override = default;

    virtual Entry remove_min(std::vector<int> *key = nullptr) override;
    virtual bool empty() const override;
    virtual void clear() override;
    virtual void get_involved_heuristics(std::set<Heuristic *> &hset) override;
    virtual bool is_dead_end(
        EvaluationContext &eval_context) const override;
    virtual bool is_reliable_dead_end(
        EvaluationContext &eval_context) const override;
};

template<class Entry>
StandardScalarOpenList<Entry>::StandardScalarOpenList(const Options &opts)
    : OpenList<Entry>(opts.get<bool>("pref_only"),QueueType(opts.get_enum("queue_type"))),
      size(0),
      evaluator(opts.get<ScalarEvaluator *>("eval")) {
}

template<class Entry>
StandardScalarOpenList<Entry>::StandardScalarOpenList(
    ScalarEvaluator *evaluator, bool preferred_only)
    : OpenList<Entry>(preferred_only),
      size(0),
      evaluator(evaluator) {
}


class StandardScalarOpenListFactory : public OpenListFactory {
    Options options;
public:
    explicit StandardScalarOpenListFactory(const Options &options);
    virtual ~StandardScalarOpenListFactory() override = default;

    virtual std::unique_ptr<StateOpenList> create_state_open_list() override;
    virtual std::unique_ptr<EdgeOpenList> create_edge_open_list() override;
};


