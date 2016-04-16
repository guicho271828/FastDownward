/* -*- mode : c++ -*- */

#pragma once
#include "open_list_factory.h"
#include "../option_parser_util.h"
#include "standard_scalar_open_list.h"


#include "open_list.h"

#include "../option_parser.h"
#include "../plugin.h"

#include "../utils/memory.h"
#include "../utils/system.h"

#include <cassert>
#include <memory>
#include <vector>

namespace bandit {
    using namespace std;

    enum RewardType { NORMAL, RELATIVE, ONE};

    template<class Entry>
    class BanditOpenList : public OpenList<Entry> {
        // friend StandardScalarOpenList<Entry>;
    protected:
        size_t last_pulled = 0;
        RewardType reward_type;
        vector<unique_ptr<StandardScalarOpenList<Entry> > > open_lists;
        virtual void do_insertion(EvaluationContext &eval_context, const Entry &entry) override;
        virtual size_t pull() = 0;
        virtual void reward(EvaluationContext &eval_context) = 0;
        virtual double lever_score(int j) = 0;
    public:
        explicit BanditOpenList(const Options &opts);
        virtual ~BanditOpenList() override = default;
        virtual Entry remove_min(vector<int> *key = nullptr) override;
        virtual bool empty() const override;
        virtual void clear() override;
        // virtual void boost_preferred() override;
        virtual void get_involved_heuristics(set<Heuristic *> &hset) override;
        virtual bool is_dead_end(
            EvaluationContext &eval_context) const override;
        virtual bool is_reliable_dead_end(
            EvaluationContext &eval_context) const override;
    };


// class BanditOpenListFactory : public OpenListFactory {
//     Options options;
// public:
//     explicit BanditOpenListFactory(const Options &options);
//     virtual ~BanditOpenListFactory() override = default;
// 
//     virtual unique_ptr<StateOpenList> create_state_open_list() override;
//     virtual unique_ptr<EdgeOpenList> create_edge_open_list() override;
// };
    Options add_bandit_option_to_parser(OptionParser &parser, string def_type = "RELATIVE");


    // when these definitions are moved to .cc, it will cause undefined reference error.
    // see http://stackoverflow.com/questions/8752837/undefined-reference-to-template-class-constructor
    using Utils::ExitCode;

    template<class Entry>
    BanditOpenList<Entry>::BanditOpenList(const Options &opts)
        : last_pulled(0),
          reward_type(RewardType(opts.get_enum("reward_type"))) {
        auto evals = opts.get_list<ScalarEvaluator *>("evals");
        open_lists.reserve(evals.size());
        for (auto eval : evals){
            open_lists.push_back(
                Utils::make_unique_ptr<StandardScalarOpenList<Entry>>(eval,false));
        }
    }

    template<class Entry>
    void BanditOpenList<Entry>::do_insertion(
        EvaluationContext &eval_context, const Entry &entry) {
        for (const auto &sublist : open_lists){
            sublist->insert(eval_context, entry);
        }
        reward(eval_context);
    }

    template<class Entry>
    Entry BanditOpenList<Entry>::remove_min(vector<int> *key) {
        if (key) {
            cerr << "not implemented -- see msg639 in the tracker" << endl;
            Utils::exit_with(ExitCode::UNSUPPORTED);
        }
        last_pulled = pull();
        return open_lists[last_pulled]->remove_min(nullptr);
    }



    template<class Entry>
    bool BanditOpenList<Entry>::empty() const {
        for (const auto &sublist : open_lists)
            if (!sublist->empty())
                return false;
        return true;
    }

    template<class Entry>
    void BanditOpenList<Entry>::clear() {
        for (const auto &sublist : open_lists)
            sublist->clear();
    }

    template<class Entry>
    void BanditOpenList<Entry>::get_involved_heuristics(
        set<Heuristic *> &hset) {
        for (const auto &sublist : open_lists)
            sublist->get_involved_heuristics(hset);
    }

    template<class Entry>
    bool BanditOpenList<Entry>::is_dead_end(
        EvaluationContext &eval_context) const {
        // If one sublist is sure we have a dead end, return true.
        if (is_reliable_dead_end(eval_context))
            return true;
        // Otherwise, return true if all sublists agree this is a dead-end.
        for (const auto &sublist : open_lists)
            if (!sublist->is_dead_end(eval_context))
                return false;
        return true;
    }

    template<class Entry>
    bool BanditOpenList<Entry>::is_reliable_dead_end(
        EvaluationContext &eval_context) const {
        for (const auto &sublist : open_lists)
            if (sublist->is_reliable_dead_end(eval_context))
                return true;
        return false;
    }








}


