#ifndef OPEN_LISTS_ALTERNATION_OPEN_LIST_H
#define OPEN_LISTS_ALTERNATION_OPEN_LIST_H

#include "open_list_factory.h"
#include "../option_parser_util.h"


#include "open_list.h"

#include "../option_parser.h"
#include "../plugin.h"

#include "../utils/memory.h"
#include "../utils/system.h"

#include <cassert>
#include <memory>
#include <vector>


template<class Entry>
class AlternationOpenList : public OpenList<Entry> {
    std::vector<int> priorities;
protected:
    std::vector<std::unique_ptr<OpenList<Entry>>> open_lists;
    const int boost_amount;
    virtual void do_insertion(EvaluationContext &eval_context,
                              const Entry &entry) override;

public:
    explicit AlternationOpenList(const Options &opts);
    virtual ~AlternationOpenList() override = default;

    virtual Entry remove_min(std::vector<int> *key = nullptr) override;
    virtual bool empty() const override;
    virtual void clear() override;
    virtual void boost_preferred() override;
    virtual void get_involved_heuristics(std::set<Heuristic *> &hset) override;
    virtual bool is_dead_end(
        EvaluationContext &eval_context) const override;
    virtual bool is_reliable_dead_end(
        EvaluationContext &eval_context) const override;
};


class AlternationOpenListFactory : public OpenListFactory {
    Options options;
public:
    explicit AlternationOpenListFactory(const Options &options);
    virtual ~AlternationOpenListFactory() override = default;

    virtual std::unique_ptr<StateOpenList> create_state_open_list() override;
    virtual std::unique_ptr<EdgeOpenList> create_edge_open_list() override;
};

#endif
