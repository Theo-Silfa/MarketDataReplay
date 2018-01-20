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
#include "container_definitions.hpp"

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
    /** Multiset iterator definition for the convinience */
    using OrderSetIterator = multiset<OrderRequest>::iterator;

    enum DoneStatus
    {
        NOT_DONE,
        BID_DONE,
        ASK_DONE,
        ALL_DONE
    };

    /**
     * Constructor
     * @param bid pointer to the "Buy" order table
     * @param ask pointer to the "Sell" order table
     */
    OrderIterator(OrderSetGreaterPtr bid, OrderSetLessPtr ask);

    /** Default destructor */
    ~OrderIterator() = default;

    /** Sets the iterator to the first element of the list */
    void first();

    /** Sets the iterator to the next element of the list */
    void next();

    /**
     * Checks if the iterator have reached the end of the list
     * @return status of the traversal
     */
    DoneStatus done();

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
    /** Holds the pointer to the current "Buy" order table */
    OrderSetGreaterPtr bid_;

    /** Holds the pointer to the current "Sell" order table */
    OrderSetLessPtr ask_;

    /** Holds the multiset iterator to the current "Buy" order table */
    OrderSetIterator current_bid_;

    /** Holds the multiset iterator to the current "Sell" order table */
    OrderSetIterator current_ask_;

    /** Deleted default constructor */
    OrderIterator() = delete;
};

#endif //_ORDERITERATOR_H
