//
//  order_bbo.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "order_bbo.hpp"

//System includes
#include <iomanip>

//Local includes


/*************************** Helper Functions *************************/

const int default_width = 8;
const int default_precision = 2;
const string NIL = "NIL";

ostream & operator<<(ostream & out, const OrderBbo & obj)
{
    if(!obj.nil_buy_)
    {
        out << '|' << setw(default_width) << obj.buy_order_count_
            << '|' << setw(default_width) << obj.buy_total_volume_
            << '|' << setw(default_width) << fixed << setprecision(default_precision) << obj.buy_share_price_;
    }
    else
    {
        out << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL;
    }

    if(!obj.nil_sell_)
    {
        out << '|' << setw(default_width) << fixed << setprecision(default_precision) << obj.sell_share_price_
            << '|' << setw(default_width) << obj.sell_total_volume_
            << '|' << setw(default_width) << obj.sell_order_count_
            << '|';
    }
    else
    {
        out << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|';
    }

    return out;
}

/*************************** OrderBbo *********************************/

OrderBbo::OrderBbo(uint64_t buy_total_volume,
                   double buy_share_price,
                   uint32_t buy_order_count,
                   uint64_t sell_total_volume,
                   double sell_share_price,
                   uint32_t sell_order_count) :
    buy_total_volume_(buy_total_volume),
    buy_share_price_(buy_share_price),
    buy_order_count_(buy_order_count),
    sell_total_volume_(sell_total_volume),
    sell_share_price_(sell_share_price),
    sell_order_count_(sell_order_count),
    nil_buy_(false),
    nil_sell_(false)
{
}

void OrderBbo::setBuyNil(bool state)
{
    nil_buy_ = state;
}

void OrderBbo::setSellNil(bool state)
{
    nil_sell_ = state;
}