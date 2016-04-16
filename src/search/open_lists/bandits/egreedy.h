/* -*- mode : c++ -*- */

#pragma once
#include "../bandit_open_list.h"

namespace {
    using namespace bandit;
    using namespace std;
    template<class Entry>
    class EGreedyOpenList : public BanditOpenList<Entry> {
        vector<double> scores;
        vector<int> pulls;
        vector<int> best_values;
        vector<int> first_values;
        bool just_pulled = false;
        double epsilon;
        bool pull_once = true;
    protected:
        virtual double lever_score(int j) override;
    public:
        explicit EGreedyOpenList(const Options &opts);
        virtual ~EGreedyOpenList() override = default;
        virtual size_t pull() override;
        virtual void reward(EvaluationContext &eval_context) override;
    };


    class EGreedyOpenListFactory : public OpenListFactory {
        Options options;
    public:
        explicit EGreedyOpenListFactory(const Options &options):options(options) {};
        virtual ~EGreedyOpenListFactory() override = default;
        virtual unique_ptr<StateOpenList> create_state_open_list() {
            return Utils::make_unique_ptr<EGreedyOpenList<StateOpenListEntry>>(options);
        };
        virtual unique_ptr<EdgeOpenList> create_edge_open_list() {
            return Utils::make_unique_ptr<EGreedyOpenList<EdgeOpenListEntry>>(options);
        };
    };
}
