/* -*- mode : c++ -*- */

#pragma once
#include "../bandit_open_list.h"
#include <random>

namespace {
    using namespace bandit;
    using namespace std;
    template<class Entry>
    class SoftmaxOpenList : public BanditOpenList<Entry> {
        vector<double> scores;
        vector<int> pulls;
        vector<int> best_values;
        vector<int> first_values;
        bool just_pulled = false;
        double t;
        bool pull_once = true;
        mt19937 rng;
    protected:
        virtual double lever_score(int j) override;
    public:
        explicit SoftmaxOpenList(const Options &opts);
        virtual ~SoftmaxOpenList() override = default;
        virtual size_t pull() override;
        virtual void reward(EvaluationContext &eval_context) override;
    };


    class SoftmaxOpenListFactory : public OpenListFactory {
        Options options;
    public:
        explicit SoftmaxOpenListFactory(const Options &options):options(options) {};
        virtual ~SoftmaxOpenListFactory() override = default;
        virtual unique_ptr<StateOpenList> create_state_open_list() {
            return Utils::make_unique_ptr<SoftmaxOpenList<StateOpenListEntry>>(options);
        };
        virtual unique_ptr<EdgeOpenList> create_edge_open_list() {
            return Utils::make_unique_ptr<SoftmaxOpenList<EdgeOpenListEntry>>(options);
        };
    };
}
