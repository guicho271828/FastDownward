/* -*- mode : c++ -*- */
#ifndef OPEN_LISTS_TYPED_TIEBREAKING_OPEN_LIST_H
#define OPEN_LISTS_TYPED_TIEBREAKING_OPEN_LIST_H

#include "open_list_factory.h"
#include "tiebreaking_open_list.h"

#include "../option_parser_util.h"
#include "../utils/collections.h"

#include <deque>
#include <map>
#include <unordered_map>
#include <vector>

template<typename K, typename V>
class index_access_unordered_map {
    using Storage = std::vector<std::pair<K,V>>;
    Storage storage;
    std::unordered_map<K, int> key_to_index;
    int count;
public:
    index_access_unordered_map(){
        count = 0;
    }
    V & operator[](const K &key) {
        int index;
        try{
            index = key_to_index.at(key);
        } catch (const std::out_of_range& c) {
            index = storage.size();
            key_to_index[key] = index;
        }
        auto &result = at_index(index);
        result.first = key;
        return result.second;
    }
    void erase(const K &key) {
        try{
            int index = key_to_index.at(key);
            auto &last_key = storage.back().first;
            key_to_index[last_key] = index;
            key_to_index.erase(key);
            Utils::swap_and_pop_from_vector(storage, index);
        } catch (const std::out_of_range& c) {}
    }
    void erase(typename Storage::iterator it) {
        erase(it->first);
    }
    typename Storage::value_type &at_index(const int &index) {
        try{
            return storage.at(index);
        } catch (const std::out_of_range& c) {
            storage.resize(index+1);
            return storage[index];
        }
    }
    typename Storage::value_type &at_random(){
        return at_index(g_rng(storage.size()));
    }
    typename Storage::iterator iter_random(){
        return begin()+g_rng(storage.size());
    }
    typename Storage::iterator iter_next(){
        if (count == 0) {
            count = storage.size();
        }
        --count;
        assert(count>=0);
        assert(!storage.empty());
        return begin()+count;
    }
    typename Storage::iterator begin() {return storage.begin();}
    typename Storage::iterator end() {return storage.end();}
    // int size (){
    //     int count = 0;
    //     for (auto &pair : storage){
    //         count += pair.second.size();
    //     }
    //     return count;
    // }
    int size (){
        return storage.size();
    }
    bool empty (){
        return storage.empty();
    }
};


template<class Entry>
class TypedTiebreakingOpenList : public TieBreakingOpenList<Entry> {

public:
    typedef std::vector<int> Key;
    typedef index_access_unordered_map<Key, Bucket<Entry>> TypeBuckets;
private:
    std::vector<ScalarEvaluator *> type_evaluators;
    bool stochastic;

protected:
    std::map<Key, TypeBuckets> buckets;
    virtual void do_insertion(EvaluationContext &eval_context,
                              const Entry &entry) override;
public:
    explicit TypedTiebreakingOpenList(const Options &opts);
    virtual ~TypedTiebreakingOpenList() override {
        for (auto e : type_evaluators){
            delete e;
        }
    };

    virtual Entry remove_min(std::vector<int> *key = nullptr) override;
    virtual bool is_dead_end(
        EvaluationContext &eval_context) const override;
    virtual void get_involved_heuristics(std::set<Heuristic *> &hset) override;

};

    

class TypedTiebreakingOpenListFactory : public OpenListFactory {
    Options options;
public:
    explicit TypedTiebreakingOpenListFactory(const Options &options);
    virtual ~TypedTiebreakingOpenListFactory() override = default;

    virtual std::unique_ptr<StateOpenList> create_state_open_list() override;
    virtual std::unique_ptr<EdgeOpenList> create_edge_open_list() override;
};

#endif
