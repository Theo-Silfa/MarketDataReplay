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

//Local includes

using namespace std;

//Forward declarations
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
    /**
     * Constructor
     * @param buy_total_volume cumulative volume for buy orders
     * @param buy_share_price buy share price level
     * @param buy_order_count number of same price level buy orders
     * @param sell_total_volume cumulative volume for sell orders
     * @param sell_share_price sell share price level
     * @param sell_order_count number of same price level sell orders
     */
    OrderBbo(uint64_t buy_total_volume,
             double buy_share_price,
             uint32_t buy_order_count,
             uint64_t sell_total_volume,
             double sell_share_price,
             uint32_t sell_order_count);

    /** Default destructor */
    ~OrderBbo() = default;

    /** Set nil state for buys */
    void setBuyNil(bool state);

    /** Set nil state for sells */
    void setSellNil(bool state);

        /** Friend definition for the out stream */
    friend ostream & operator<<(ostream & os, const OrderBbo & obj);

protected:
    /** Cumulative volume for buy orders */
    uint64_t buy_total_volume_;

    /** Buy share price level */
    double buy_share_price_;

    /** Number of same price level buy orders */
    uint32_t buy_order_count_;

    /** Cumulative volume for sell orders */
    uint64_t sell_total_volume_;

    /** Sell share price level */
    double sell_share_price_;

    /** Number of same price level sell orders */
    uint32_t sell_order_count_;

    /** Flag to indicate if buy bbo is present */
    bool nil_buy_;

    /** Flag to indicate if sell bbo is present */
    bool nil_sell_;

    /** Deleted default constructor */
    OrderBbo() = delete;
};

#endif //_ORDERBBO_H
