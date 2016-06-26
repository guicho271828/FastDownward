#pragma once

#include "open_list_factory.h"

#include "../option_parser_util.h"


class WeightedAlternationOpenListFactory : public OpenListFactory {
    Options options;
public:
    explicit WeightedAlternationOpenListFactory(const Options &options);
    virtual ~WeightedAlternationOpenListFactory() override = default;

    virtual std::unique_ptr<StateOpenList> create_state_open_list() override;
    virtual std::unique_ptr<EdgeOpenList> create_edge_open_list() override;
};


