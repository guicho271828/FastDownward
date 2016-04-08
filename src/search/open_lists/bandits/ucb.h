/* -*- mode : c++ -*- */

#pragma once
#include "../bandit_open_list.h"

namespace {
    using namespace bandit;
    using namespace std;
    template<class Entry>
    class UCBOpenList : public BanditOpenList<Entry> {
        // void update_pulls();
        double k;
        bool pull_once = true;
        int all_pulls = 0;
        bool just_pulled = false;
        vector<double> scores;
        vector<int> pulls;
        vector<int> best_values;
        vector<int> first_values;
    protected:
        virtual double lever_score(int j) override;
    public:
        explicit UCBOpenList(const Options &opts);
        virtual ~UCBOpenList() override = default;
        virtual size_t pull() override;
        virtual void reward(EvaluationContext &eval_context) override;
    };


    class UCBOpenListFactory : public OpenListFactory {
        Options options;
    public:
        explicit UCBOpenListFactory(const Options &options):options(options) {};
        virtual ~UCBOpenListFactory() override = default;
        virtual unique_ptr<StateOpenList> create_state_open_list() {
            return Utils::make_unique_ptr<UCBOpenList<StateOpenListEntry>>(options);
        };
        virtual unique_ptr<EdgeOpenList> create_edge_open_list() {
            return Utils::make_unique_ptr<UCBOpenList<EdgeOpenListEntry>>(options);
        };
    };
}
