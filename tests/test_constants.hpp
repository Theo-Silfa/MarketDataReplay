//
//  test_constants.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 20.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

//System includes

//Local includes
#include "order_request.hpp"

using namespace std;

/****************************** Constants *****************************/

const uint64_t DEFAULT_ORDER_ID = 100;
const uint64_t DEFAULT_QUANTITY = 10;
const double DEFAULT_PRICE = 72.82;

const OrderRequest order_one = {100, 10, 72.82};
const OrderRequest order_two = {101, 100, 72.82};
const OrderRequest order_three = {102, 100, 72.79};
const OrderRequest order_four = {103, 100, 72.79};

const int default_width = 8;
const int default_precision = 2;
const string NIL = "NIL";

const multiset<OrderRequest, greater<OrderRequest>> orders_buy =
        {order_one, order_two, order_three, order_four};

const multiset<OrderRequest, less<OrderRequest>> orders_sell_ =
        {order_one, order_two, order_three, order_four};

const auto buy_range = orders_buy.equal_range(*(orders_buy.begin()));
const auto sell_range = orders_sell_.equal_range(*(orders_sell_.begin()));

const auto bbo_buy_volume = order_one.quantity + order_two.quantity;
const auto bbo_buy_price = order_one.price;
const auto orders_in_buy_range = distance(buy_range.first, buy_range.second);
const auto bbo_sell_volume = order_three.quantity + order_four.quantity;
const auto bbo_sell_price = order_four.price;
const auto orders_in_sell_range = distance(sell_range.first, sell_range.second);
