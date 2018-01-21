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
#include <tuple>

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
    /** Default constructor */
    OrderBbo();

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

    /**
     * Sets the total volume for bids for this obj
     * @param total volume for bids
     */
    void setBuyTotalVolume(uint64_t volume);

    /**
     * Sets the price for one buy share
     * @param price for one share
     */
    void setBuySharePrice(double price);

    /**
     * Sets the buy order count with the same price level
     * @param order count
     */
    void setBuyOrderCount(uint64_t count);

    /**
     * Sets the total volume for asks for this obj
     * @param total volume for asks
     */
    void setSellTotalVolume(uint64_t volume);

    /**
     * Sets the price for one sell share
     * @param price for one share
     */
    void setSellSharePrice(double price);

    /**
     * Sets the sell order count with the same price level
     * @param order count
     */
    void setSellOrderCount(uint64_t count);

    /**
     * Returns the total volume for bids for this obj
     * @return total volume for bids
     */
    uint64_t getBuyTotalVolume();

    /**
     * Returns the price for one buy share
     * @return price for one share
     */
    double getBuySharePrice();

    /**
     * Returns the buy order count with the same price level
     * @return order count
     */
    uint64_t getBuyOrderCount();

    /**
     * Returns the total volume for asks for this obj
     * @return total volume for asks
     */
    uint64_t getSellTotalVolume();

    /**
     * Returns the price for one sell share
     * @return price for one share
     */
    double getSellSharePrice();

    /**
     * Returns the sell order count with the same price level
     * @return order count
     */
    uint64_t getSellOrderCount();

    /**
     * Packs all of the fields in to tie
     * @return tie object
     */
    auto getTie() const;

    /** Friend definition for the out stream */
    friend ostream & operator<<(ostream & os, const OrderBbo & obj);

    /**
     * Equality operator implementation for OrderBbo class
     * @param obj object to compare with
     * @return true if equal
     */
    bool operator==(const OrderBbo &obj) const;

    /**
     * Nonequality operator implementation for OrderBbo class
     * @param obj object to compare with
     * @return true if not equal
     */
    bool operator!=(const OrderBbo &obj) const;

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
};

#endif //_ORDERBBO_H
