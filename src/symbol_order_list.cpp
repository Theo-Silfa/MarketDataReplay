//
//  symbol_order_list.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "symbol_order_list.hpp"

//System includes
#include <iostream>
#include <cstdlib>
#include <algorithm>

//Local includes
#include "order_iterator.hpp"
#include "order_bbo.hpp"
#include "order_request.hpp"

/*************************** Helper Functions *************************/

namespace
{

/**
 * Is used to check the order for validity. Throws OrderProcessException
 * if something is wrong
 * @param order_id order unique identificator
 * @param side "Buy" or "Sell"
 * @param quantity number of shares
 * @param price for one share
 */
void OrderCheckAssertion(const uint64_t order_id,
                         const string &side,
                         const uint64_t quantity,
                         const double price)
{
    if (quantity == 0)
    {
        throw OrderProcessException("For order_id [" + to_string(order_id)
            + "] quantity is zero");
    }

    if (price < 0)
    {
        throw OrderProcessException("For order_id [" + to_string(order_id)
            + "] price is negative");
    }

    if (side != "Buy" && side != "Sell")
    {
        throw OrderProcessException("For order_id [" + to_string(order_id)
            + "] side is unknown: [" + side + "]");
    }
}

/**
 * Is used to collect the bbo information through out the range
 * @param range pair of iterators of the range of interest
 * @param bbo_volume reference to return back the total volume
 * @param bbo_price reference to return back the price level
 * @param orders_in_range reference to return back the number of orders on this price level
 */
void PutTogetherBbo(const pair<multiset<OrderRequest>::iterator, multiset<OrderRequest>::iterator> &range,
                    uint64_t &bbo_volume,
                    double &bbo_price,
                    uint32_t &orders_in_range)
{
    for_each(range.first, range.second,
        [&bbo_volume](const OrderRequest& obj){ bbo_volume += obj.quantity; });

    bbo_price = range.first->price;

    orders_in_range = distance(range.first, range.second);
}

/**
 * Is used to calculate the vwap information on the requested quantity
 * @param begin multiset iterator to the begining of the range of interest
 * @param end multiset iterator to the end of the range of interest
 * @param requested_quantity numbeer of shares to calculate vwap for
 * @return vwap information
 */
double CalculateVwap(multiset<OrderRequest>::iterator begin,
                     multiset<OrderRequest>::iterator end,
                     const uint64_t &requested_quantity)
{
    double total_price_over_quantity = 0.0;
    int64_t left_quantity = requested_quantity;

    for(auto itr = begin; itr != end; ++itr)
    {
        left_quantity -= itr->quantity;

        if(left_quantity > 0)
        {
            total_price_over_quantity += itr->price * itr->quantity;
        }
        else
        {
            total_price_over_quantity += itr->price * (itr->quantity - llabs(left_quantity));

            // We covered all the requested quantity
            break;
        }
    }

    return total_price_over_quantity / requested_quantity;
}

} // namespace

/*************************** SymbolOrderList **************************/

SymbolOrderList::SymbolOrderList(string symbol) :
    orders_buy_(make_shared<multiset<OrderRequest, greater<OrderRequest>>>()),
    orders_sell_(make_shared<multiset<OrderRequest, less<OrderRequest>>>()),
    symbol_(symbol)
{
}

uint64_t SymbolOrderList::totalQuantity()
{
    uint64_t total_quantity = 0;

    for(const auto &order: *(orders_buy_))
    {
        total_quantity += order.quantity;
    }

    for(const auto &order: *(orders_sell_))
    {
        total_quantity += order.quantity;
    }

    return total_quantity;
}

const string & SymbolOrderList::symbol()
{
    return symbol_;
}

void SymbolOrderList::add(uint64_t order_id, const string &side, uint64_t quantity, double price)
{
    OrderCheckAssertion(order_id, side, quantity, price);

    auto search = existing_orders_.find(order_id);

    if (search != existing_orders_.end())
    {
        throw OrderProcessException("Dublicated order_id [" + to_string(order_id)
            + "]");
    }

    OrderSide side_choice;
    multiset<OrderRequest>::iterator itr;

    if (side == "Buy")
    {
        side_choice = OrderSide::BUY;
        itr = orders_buy_->insert({order_id, quantity, price});
    }
    else if (side == "Sell")
    {
        side_choice = OrderSide::SELL;
        itr = orders_sell_->insert({order_id, quantity, price});
    }

    existing_orders_.insert({ order_id, {side_choice, itr} });
}

void SymbolOrderList::modify(uint64_t order_id, uint64_t quantity, double price)
{
    //To prevent from throwing - feed dummy side to the assertion
    OrderCheckAssertion(order_id, "Buy", quantity, price);

    auto search = existing_orders_.find(order_id);

    if (search == existing_orders_.end())
    {
        throw OrderProcessException("No such order_id [" + to_string(order_id)
            + "]");
    }

    if (search->second.first == OrderSide::BUY)
    {
        orders_buy_->erase(search->second.second);
        search->second.second = orders_buy_->insert({order_id, quantity, price});
    }
    else if (search->second.first == OrderSide::SELL)
    {
        orders_sell_->erase(search->second.second);
        search->second.second = orders_sell_->insert({order_id, quantity, price});
    }
}

void SymbolOrderList::cancel(uint64_t order_id)
{
    auto search = existing_orders_.find(order_id);

    if (search == existing_orders_.end())
    {
        throw OrderProcessException("No such order_id [" + to_string(order_id)
            + "]");
    }

    if (search->second.first == OrderSide::BUY)
    {
        orders_buy_->erase(search->second.second);
    }
    else if (search->second.first == OrderSide::SELL)
    {
        orders_sell_->erase(search->second.second);
    }

    existing_orders_.erase(search);
}

OrderBbo SymbolOrderList::bbo()
{
    OrderBbo result;

    uint64_t bbo_volume = 0;
    double bbo_price = 0;
    uint32_t orders_in_range = 0;

    if (!orders_buy_->empty())
    {
        auto buy_range = orders_buy_->equal_range(*(orders_buy_->begin()));
        PutTogetherBbo(buy_range, bbo_volume, bbo_price, orders_in_range);

        result.setBuyTotalVolume(bbo_volume);
        result.setBuySharePrice(bbo_price);
        result.setBuyOrderCount(orders_in_range);
        result.setBuyNil(false);

        bbo_volume = 0;
        bbo_price = 0;
        orders_in_range = 0;
    }

    if (!orders_sell_->empty())
    {
        auto sell_range = orders_sell_->equal_range(*(orders_sell_->begin()));
        PutTogetherBbo(sell_range, bbo_volume, bbo_price, orders_in_range);

        result.setSellTotalVolume(bbo_volume);
        result.setSellSharePrice(bbo_price);
        result.setSellOrderCount(orders_in_range);
        result.setSellNil(false);
    }

    return result;
}

OrderVwap SymbolOrderList::vwap(uint64_t quantity)
{
    if (quantity == 0)
    {
        throw OrderProcessException("Can't calculate vwap for zero quantity");
    }

    if (quantity > totalQuantity())
    {
        return {0.0, 0.0};
    }

    double buy_vwap = 0, sell_vwap = 0;

    if (!orders_buy_->empty())
    {
        buy_vwap = CalculateVwap(orders_buy_->begin(), orders_buy_->end(), quantity);
    }

    if (!orders_sell_->empty())
    {
        sell_vwap = CalculateVwap(orders_sell_->begin(), orders_sell_->end(), quantity);
    }

    return {buy_vwap, sell_vwap};
}

OrderIterator SymbolOrderList::getIterator()
{
    return OrderIterator(orders_buy_, orders_sell_);
}
