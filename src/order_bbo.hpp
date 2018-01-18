//
//  order_bbo.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef _ORDERBBO_H
#define _ORDERBBO_H

//System includes
#include <ostream>
#include <set>

//Local includes

using namespace std;

//Forward declarations
struct OrderRequest;
class OrderBbo;

/**
 * Is used to define the out stream print for OrderBbo class
 * @param out where to print
 * @param obj what to print
 * @return modified out stream
 */
ostream & operator<<(ostream & out, const OrderBbo & obj);

/**
 * Order Best Bid Offer class. Holds the information about the current
 * BBO. Can be invalidated.
 */
class OrderBbo final
{
public:
    /** Order range definition for convinience */
    using OrderRangeIterators =
        pair<multiset<OrderRequest>::iterator,
             multiset<OrderRequest>::iterator>;

    /**
     * Constructor
     * @param bid range for the BBO order for buy
     * @param ask range for the BBO order for sell
     */
    OrderBbo(OrderRangeIterators bid, OrderRangeIterators ask);

    /** Default destructor */
    ~OrderBbo() = default;

    /** Friend definition for the out stream */
    friend ostream & operator<<(ostream & os, const OrderBbo & obj);

    /** Calculate the current range */
    void calculate();

protected:
    /** Range from order list for bids */
    OrderRangeIterators  bid;

    /** Range from order list for asks */
    OrderRangeIterators  ask;

    /** Deleted default constructor */
    OrderBbo() = delete;
};

#endif //_ORDERBBO_H
