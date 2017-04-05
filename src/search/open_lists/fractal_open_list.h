/* -*- mode : c++ -*- */
#ifndef OPEN_LISTS_FRACTAL_OPEN_LIST_H
#define OPEN_LISTS_FRACTAL_OPEN_LIST_H

#include "open_list_factory.h"
#include "typed_tiebreaking_open_list.h"

#include "../option_parser_util.h"
#include "../utils/collections.h"

#include <unordered_map>

template<class Entry>
class FractalOpenList : public TypedTiebreakingOpenList<Entry> {
    typedef typename TypedTiebreakingOpenList<Entry>::Key Key;
    typedef typename TypedTiebreakingOpenList<Entry>::TypeBuckets TypeBuckets;
public:
    
private:
    uint max_depth;
    std::unordered_map<Key,uint> current_dimension;
    std::unordered_map<Key,std::vector<uint> > expansion_records; // how many times the depth was expanded
    
    int random_index_with_size_diff(const std::vector<uint> &records, int dim);
    int first_index_with_size_diff(const std::vector<uint> &records, int dim);
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
