#ifndef OPEN_LISTS_EPSILON_GREEDY_OPEN_LIST_H
#define OPEN_LISTS_EPSILON_GREEDY_OPEN_LIST_H

#include "open_list_factory.h"

#include "../option_parser_util.h"


/*
    Epsilon-greedy open list based on Valenzano et al. (ICAPS 2014).

    With probability epsilon the next entry is selected uniformly
    randomly, otherwise the minimum entry is chosen. While the original
    implementation by Valenzano et al. is based on buckets (personal
    communication with the authors), this implementation stores entries
    in a heap. It is usually desirable to let open lists break ties in
    FIFO order. When using a heap, this can be achieved without using
    significantly more time by assigning increasing IDs to new entries
    and using the IDs as tiebreakers for entries with identical values.
    On the other hand, FIFO tiebreaking induces a substantial worst-case
    runtime penalty for bucket-based implementations. In the table below
    we list the worst-case time complexities for the discussed
    implementation strategies.

    n: number of entries
    m: number of buckets

                            Buckets       Buckets (no FIFO)    Heap
    Insert entry            O(log(m))     O(log(m))            O(log(n))
    Remove random entry     O(m + n)      O(m)                 O(log(n))
    Remove minimum entry    O(log(m))     O(log(m))            O(log(n))
        
    These results assume that the buckets are implemented as deques and
    are stored in a sorted dictionary, mapping from evaluator values to
    buckets. For inserting a new entry and removing the minimum entry the
    bucket-based implementations need to find the correct bucket
    (O(log(m))) and can then push or pop from one end of the deque
    (O(1)). For returning a random entry, bucket-based implementations
    need to loop over all buckets (O(m)) to find the one that contains
    the randomly selected entry. If FIFO ordering is ignored, one can use
    swap-and-pop to remove the entry in constant time. Otherwise, the
    removal is linear in the number of entries in the bucket (O(n), since
    there could be only one bucket).
    
    Comments from Masataro Asai:
    
    I reverted above changes because of the following reasons.
    
    RE: "It is usually desirable to" ...  FIFO is not desirable at all. It
    is the *worst*. (Asai,Fukunaga 2016).
    
    RE: "If FIFO ordering is ignored ... Otherwise, the removal is linear"
    Removal of the first/last entry from deque is O(1), not
    linear. Therefore O(m+n) is actually O(m).
    
    RE:"loop over all buckets (O(m)) ..." This can be improved to
    O(log(m)) using a binary search tree (not implemented yet).
    
    In this BST, each node have a pointer to the parent, a value, and two
    children.  The value of each node is the sum of the values of its two
    children.  Leaf nodes have a pointer to a bucket, and its value is the
    number of elements in the bucket.  The root node has the sum of the
    sizes of all buckets.
        
    When inserting a search node, it should propagate the increase up to the root, which is O(log(m)). When
    removing a node, select LEFT node with probability
    value(LEFT)/(value(LEFT)+value(RIGHT)), (resp. RIGHT), which requires O(log(m)) coin flips.
    There is a hashtable from each bucket to the leaf node, enabling propagation.
    The tree should be balanced. Thus the actual implementation would use Red-Black trees.

    Overall, switching FIFO/LIFO/RANDOM last-resort tiebreaking 
    on heap-based implementation does not seem trivial. this is the main reason I fall back to the
    bucket-basaed implementation.

*/

class EpsilonGreedyOpenListFactory : public OpenListFactory {
    Options options;
public:
    explicit EpsilonGreedyOpenListFactory(const Options &options);
    virtual ~EpsilonGreedyOpenListFactory() override = default;

    virtual std::unique_ptr<StateOpenList> create_state_open_list() override;
    virtual std::unique_ptr<EdgeOpenList> create_edge_open_list() override;
};

#endif
