//
//  order_iterator.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 16.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef _ORDERITERATOR_H
#define _ORDERITERATOR_H

//System includes
#include <set>

//Local includes

//Forward declarations
struct OrderRequest;

using namespace std;

/**
 * Iterator for the Order list. Is used to traverse the list. Can be
 * invalidated depending on the operations happened with the list object
 */
class OrderIterator
{
public:
    /** Multiset definition for the convinience */
    using OrderSetIterator = multiset<OrderRequest>::iterator;

    /**
     * Constructor
     * @param bid iterator to the "Buy" order
     * @param ask iterator to the "Sell" order
     */
    OrderIterator(OrderSetIterator bid, OrderSetIterator ask);

    /** Default destructor */
    ~OrderIterator() = default;

    /** Sets the iterator to the first element of the list */
    void first();

    /** Sets the iterator to the next element of the list */
    void next();

    /**
     * Checks if the iterator have reached the end of the list
     * @return true if end, false if not
     */
    bool done();

    /**
     * Get the current bid order for this iterator
     * @return current bid
     */
    const OrderRequest & getBid();

    /**
     * Get the current ask order for this iterator
     * @return current ask
     */
    const OrderRequest & getAsk();

private:
    /** Holds the iterator to the current "Buy" order */
    OrderSetIterator bid_itr_;

    /** Holds the iterator to the current "Sell" order */
    OrderSetIterator ask_itr_;

    /** Deleted default constructor */
    OrderIterator() = delete;
};

#endif //_ORDERITERATOR_H
