//
//  test_constants.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 20.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef _TESTCONSTANTS_H
#define _TESTCONSTANTS_H

//System includes
#include <functional>

//Local includes
#include "order_request.hpp"

using namespace std;

/****************************** Constants *****************************/

const uint64_t DEFAULT_ORDER_ID = 100;
const uint64_t DEFAULT_QUANTITY = 10;
const double DEFAULT_PRICE = 72.82;
const string DEFAULT_SHARE_NAME = "AAPL";
const uint64_t DEFAULT_VWAP_QUANTITY = 5;

const OrderRequest order_one       = {100, 10, 72.82};
const OrderRequest order_one_dub   = {200, 15, 72.82};
const OrderRequest order_two       = {101, 100, 72.81};
const OrderRequest order_two_dub   = {201, 125, 72.81};
const OrderRequest order_three     = {102, 100, 72.79};
const OrderRequest order_three_dub = {202, 125, 72.79};
const OrderRequest order_four      = {103, 100, 72.78};
const OrderRequest order_four_dub  = {203, 125, 72.78};

const int default_width = 10;
const int default_precision = 2;
const string NIL = "NIL";

const multiset<OrderRequest, greater<OrderRequest>> orders_buy =
        {order_one, order_one_dub, order_two, order_two_dub,
         order_three, order_three_dub, order_four, order_four_dub};

const multiset<OrderRequest, less<OrderRequest>> orders_sell =
        {order_one, order_one_dub, order_two, order_two_dub,
         order_three, order_three_dub, order_four, order_four_dub};

const auto buy_range = orders_buy.equal_range(*(orders_buy.begin()));
const auto sell_range = orders_sell.equal_range(*(orders_sell.begin()));

const auto bbo_buy_volume = order_one.quantity + order_one_dub.quantity;
const auto bbo_buy_price = order_one.price;
const auto orders_in_buy_range = distance(buy_range.first, buy_range.second);
const auto bbo_sell_volume = order_four.quantity + order_four_dub.quantity;
const auto bbo_sell_price = order_four.price;
const auto orders_in_sell_range = distance(sell_range.first, sell_range.second);

#endif //_TESTCONSTANTS_H
