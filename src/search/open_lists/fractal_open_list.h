/* -*- mode : c++ -*- */
#ifndef OPEN_LISTS_FRACTAL_OPEN_LIST_H
#define OPEN_LISTS_FRACTAL_OPEN_LIST_H

#include "open_list_factory.h"
#include "typed_tiebreaking_open_list.h"

#include "../option_parser_util.h"
#include "../utils/collections.h"

// #include <deque>
// #include <map>
// #include <unordered_map>
// #include <vector>

template<class Entry>
class FractalOpenList : public TypedTiebreakingOpenList<Entry> {

public:
private:
    uint max_depth;
    uint current_dimension;
    int random_index_with_size_diff(typename TypedTiebreakingOpenList<Entry>::TypeBuckets &tbuckets);
    int first_index_with_size_diff(typename TypedTiebreakingOpenList<Entry>::TypeBuckets &tbuckets);
protected:
public:
    explicit FractalOpenList(const Options &opts);
    // virtual ~FractalOpenList() override {};

    virtual Entry remove_min(std::vector<int> *key = nullptr) override;
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
