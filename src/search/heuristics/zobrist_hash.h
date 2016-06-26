#ifndef HEURISTICS_ZOBRIST_HASH_H
#define HEURISTICS_ZOBRIST_HASH_H

#include "../heuristic.h"
#include "../utils/rng.h"

namespace ZobristHash {
class ZobristHash : public Heuristic {
protected:
    std::vector<std::vector<unsigned int> > map;
    virtual void initialize();
    virtual int compute_heuristic(const GlobalState &state);
public:
    ZobristHash(const Options &options);
    ~ZobristHash();
};
}

#endif
