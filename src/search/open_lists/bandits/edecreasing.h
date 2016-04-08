/* -*- mode : c++ -*- */

#pragma once
#include "../bandit_open_list.h"

namespace {
    using namespace bandit;
    using namespace std;
    template<class Entry>
    class EDecreasingOpenList : public BanditOpenList<Entry> {
        vector<double> scores;
        vector<int> pulls;
        vector<int> best_values;
        vector<int> first_values;
        bool just_pulled = false;
        double epsilon;
        double decay;
        bool pull_once = true;
        int all_pulls = 0;
    protected:
        virtual double lever_score(int j) override;
    public:
        explicit EDecreasingOpenList(const Options &opts);
        virtual ~EDecreasingOpenList() override = default;
        virtual size_t pull() override;
        virtual void reward(EvaluationContext &eval_context) override;
    };


    class EDecreasingOpenListFactory : public OpenListFactory {
        Options options;
    public:
        explicit EDecreasingOpenListFactory(const Options &options):options(options) {};
        virtual ~EDecreasingOpenListFactory() override = default;
        virtual unique_ptr<StateOpenList> create_state_open_list() {
            return Utils::make_unique_ptr<EDecreasingOpenList<StateOpenListEntry>>(options);
        };
        virtual unique_ptr<EdgeOpenList> create_edge_open_list() {
            return Utils::make_unique_ptr<EDecreasingOpenList<EdgeOpenListEntry>>(options);
        };
    };
}
