#ifndef OPEN_LISTS_FRACTAL_OPEN_LIST_H
#define OPEN_LISTS_FRACTAL_OPEN_LIST_H

#include "open_list_factory.h"
#include "open_list.h"

#include "../option_parser_util.h"

#include <deque>
#include <vector>
#include <set>
#include <memory>

/*
  Bucket-based implementation of an open list.

  Nodes with identical heuristic value are expanded in FIFO order.
*/


class ScalarEvaluator;

template<class Entry>
class FractalOpenList : public OpenList<Entry> {
    typedef std::deque<Entry> Bucket;
    std::vector<Bucket> buckets;
    mutable int lowest_bucket;
    int size;

    std::unique_ptr<OpenList<Entry>> sublist;
    ScalarEvaluator *evaluator;

protected:
    virtual void do_insertion(EvaluationContext &eval_context,
                              const Entry &entry) override;

public:
    explicit FractalOpenList(const Options &opts);
    virtual ~FractalOpenList() override = default;

    virtual Entry remove_min(std::vector<int> *key = nullptr) override;
    virtual bool empty() const override;
    virtual void clear() override;
    virtual void get_involved_heuristics(std::set<Heuristic *> &hset) override;
    virtual bool is_dead_end(
        EvaluationContext &eval_context) const override;
    virtual bool is_reliable_dead_end(
        EvaluationContext &eval_context) const override;
};

class FractalOpenListFactory : public OpenListFactory {
    Options options;
public:
    explicit FractalOpenListFactory(const Options &options);
    virtual ~FractalOpenListFactory() override = default;

    virtual std::unique_ptr<StateOpenList> create_state_open_list() override;
    virtual std::unique_ptr<EdgeOpenList> create_edge_open_list() override;
};

#endif
