//
//  symbol_order_list_unittest.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 20.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

//System includes
#include <gtest/gtest.h>
#include <iomanip>
#include <limits>

//Local includes
#include "test_constants.hpp"
#include "symbol_order_list.hpp"
#include "order_iterator.hpp"
#include "order_bbo.hpp"
#include "order_request.hpp"

using namespace std;

/******************************* Helpers ******************************/

/*********************** SymbolOrderListTestCase **********************/

TEST(SymbolOrderListTestCase, BasicInitTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    auto actual_symbol = order_list.symbol();
    auto actual_quantity = order_list.totalQuantity();
    auto actual_bbo = order_list.bbo();
    auto actual_vwap = order_list.vwap(DEFAULT_VWAP_QUANTITY);

    EXPECT_EQ(actual_symbol, DEFAULT_SHARE_NAME);
    EXPECT_EQ(actual_quantity, 0u);

    stringstream actual, expected;

    actual << actual_bbo;

    expected << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|';

    EXPECT_EQ(actual.str(), expected.str());

    EXPECT_EQ(actual_vwap.buy_price, 0.0);
    EXPECT_EQ(actual_vwap.sell_price, 0.0);
}

TEST(SymbolOrderListTestCase, TotalQuantityTest)
{
    vector<OrderRequest> orders = {order_one, order_two, order_three, order_four};

    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    uint64_t expected_quantity = 0;

    for (const auto & order: orders)
    {
        order_list.add(order.order_id, "Buy", order.quantity, order.price);
        expected_quantity += order.quantity;
    }

    auto actual_quantity = order_list.totalQuantity();

    EXPECT_EQ(actual_quantity, expected_quantity);
}

TEST(SymbolOrderListTestCase, BasicOrderAddTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    order_list.add(order_one.order_id, "Buy", order_one.quantity, order_one.price);
    order_list.add(order_two.order_id, "Sell", order_two.quantity, order_two.price);

    OrderIterator itr = order_list.getIterator();

    ASSERT_EQ(itr.done(), OrderIterator::NOT_DONE) << "The iterator is not pointing to the data";

    const auto &actual_buy = itr.getBid();
    const auto &actual_sell = itr.getAsk();

    EXPECT_EQ(actual_buy, order_one);
    EXPECT_EQ(actual_sell, order_two);
}

TEST(SymbolOrderListTestCase, DuplicateOrderAddTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    order_list.add(order_one.order_id, "Buy", order_one.quantity, order_one.price);

    EXPECT_THROW(order_list.add(order_one.order_id, "Buy", order_one.quantity, order_one.price),
        OrderProcessException);

    EXPECT_THROW(order_list.add(order_one.order_id, "Sell", order_one.quantity, order_one.price),
        OrderProcessException);
}

TEST(SymbolOrderListTestCase, BadSideOrderAddTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    const string bad_side = "TATATA";

    EXPECT_THROW(order_list.add(order_one.order_id, bad_side, order_one.quantity, order_one.price),
        OrderProcessException);
}

TEST(SymbolOrderListTestCase, NegativePriceOrderAddTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    const auto negative_price = order_one.price * -1.0;

    EXPECT_THROW(order_list.add(order_one.order_id, "Buy", order_one.quantity, negative_price),
        OrderProcessException);
}

TEST(SymbolOrderListTestCase, ZeroQuantityOrderAddTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    EXPECT_THROW(order_list.add(order_one.order_id, "Buy", 0, order_one.price),
        OrderProcessException);
}

TEST(SymbolOrderListTestCase, BasicOrderModifyTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    order_list.add(order_one.order_id, "Buy", order_one.quantity, order_one.price);
    order_list.add(order_two.order_id, "Sell", order_two.quantity, order_two.price);

    OrderRequest expected_order_buy = order_one;
    expected_order_buy.quantity += 10;
    expected_order_buy.price += 0.02;

    OrderRequest expected_order_sell = order_two;
    expected_order_sell.quantity -= 10;
    expected_order_sell.price -= 0.02;

    order_list.modify(expected_order_buy.order_id, expected_order_buy.quantity, expected_order_buy.price);
    order_list.modify(expected_order_sell.order_id, expected_order_sell.quantity, expected_order_sell.price);

    OrderIterator itr = order_list.getIterator();

    ASSERT_EQ(itr.done(), OrderIterator::NOT_DONE) << "The iterator is not pointing to the data";

    const auto &actual_buy = itr.getBid();
    const auto &actual_sell = itr.getAsk();

    EXPECT_EQ(actual_buy, expected_order_buy);
    EXPECT_EQ(actual_sell, expected_order_sell);
}

TEST(SymbolOrderListTestCase, UnknownOrderModifyTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    EXPECT_THROW(order_list.modify(order_one.order_id, order_one.quantity, order_one.price),
        OrderProcessException);
}

TEST(SymbolOrderListTestCase, NegativePriceOrderModifyTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    order_list.add(order_one.order_id, "Buy", order_one.quantity, order_one.price);

    const auto negative_price = order_one.price * -1.0;

    EXPECT_THROW(order_list.modify(order_one.order_id, order_one.quantity, negative_price),
        OrderProcessException);
}

TEST(SymbolOrderListTestCase, ZeroQuantityOrderModifyTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    order_list.add(order_one.order_id, "Buy", order_one.quantity, order_one.price);

    EXPECT_THROW(order_list.modify(order_one.order_id, 0, order_one.price),
        OrderProcessException);
}

TEST(SymbolOrderListTestCase, BasicOrderCancelTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    order_list.add(order_one.order_id, "Buy", order_one.quantity, order_one.price);
    order_list.add(order_two.order_id, "Sell", order_two.quantity, order_two.price);

    order_list.cancel(order_one.order_id);
    order_list.cancel(order_two.order_id);

    OrderIterator itr = order_list.getIterator();

    EXPECT_EQ(itr.done(), OrderIterator::ALL_DONE);
}

TEST(SymbolOrderListTestCase, UnknownOrderCancelTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    EXPECT_THROW(order_list.cancel(order_one.order_id), OrderProcessException);
}

TEST(SymbolOrderListTestCase, BasicOrderBboTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    auto AddOrders = [&order_list](const string& side, uint64_t order_id_shift)
    {
        order_list.add(order_one.order_id + order_id_shift, side, order_one.quantity, order_one.price);
        order_list.add(order_two.order_id + order_id_shift, side, order_two.quantity, order_two.price);
        order_list.add(order_three.order_id + order_id_shift, side, order_three.quantity, order_three.price);
        order_list.add(order_four.order_id + order_id_shift, side, order_four.quantity, order_four.price);
        order_list.add(order_one_dub.order_id + order_id_shift, side, order_one_dub.quantity, order_one_dub.price);
        order_list.add(order_two_dub.order_id + order_id_shift, side, order_two_dub.quantity, order_two_dub.price);
        order_list.add(order_three_dub.order_id + order_id_shift, side, order_three_dub.quantity, order_three_dub.price);
        order_list.add(order_four_dub.order_id + order_id_shift, side, order_four_dub.quantity, order_four_dub.price);
    };

    AddOrders("Buy", 0);
    AddOrders("Sell", 1000);

    OrderBbo actual_bbo = order_list.bbo();

    OrderBbo expected_bbo(bbo_buy_volume, bbo_buy_price, orders_in_buy_range,
    bbo_sell_volume, bbo_sell_price, orders_in_sell_range);

    EXPECT_EQ(expected_bbo, actual_bbo);
}

TEST(SymbolOrderListTestCase, EmptyListOrderBboTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    OrderBbo actual_bbo = order_list.bbo();

    OrderBbo expected_bbo(0, 0.0, 0, 0, 0.0, 0);

    expected_bbo.setBuyNil(true);
    expected_bbo.setSellNil(true);

    EXPECT_EQ(expected_bbo, actual_bbo);
}

TEST(SymbolOrderListTestCase, BasicOrderVwapTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    auto AddOrders = [&order_list](const string& side, uint64_t order_id_shift)
    {
        order_list.add(order_one.order_id + order_id_shift, side, order_one.quantity, order_one.price);
        order_list.add(order_two.order_id + order_id_shift, side, order_two.quantity, order_two.price);
        order_list.add(order_three.order_id + order_id_shift, side, order_three.quantity, order_three.price);
        order_list.add(order_four.order_id + order_id_shift, side, order_four.quantity, order_four.price);
        order_list.add(order_one_dub.order_id + order_id_shift, side, order_one_dub.quantity, order_one_dub.price);
        order_list.add(order_two_dub.order_id + order_id_shift, side, order_two_dub.quantity, order_two_dub.price);
        order_list.add(order_three_dub.order_id + order_id_shift, side, order_three_dub.quantity, order_three_dub.price);
        order_list.add(order_four_dub.order_id + order_id_shift, side, order_four_dub.quantity, order_four_dub.price);
    };

    AddOrders("Buy", 0);
    AddOrders("Sell", 1000);

    uint64_t demanded_quantity = 125;

    double total_bid_price_over_quantity =
        order_one.quantity * order_one.price +
        order_one_dub.quantity * order_one_dub.price +
        order_two.quantity * order_two.price;

    double expected_bid_vwap = total_bid_price_over_quantity / demanded_quantity;

    // quantity from order_four and order_four_dub will cover the demanded_quantity
    double expected_ask_vwap = order_four.price;

    OrderVwap actual_vwap = order_list.vwap(demanded_quantity);

    EXPECT_EQ(expected_bid_vwap, actual_vwap.buy_price);
    EXPECT_EQ(expected_ask_vwap, actual_vwap.sell_price);
}

TEST(SymbolOrderListTestCase, UnEvenBasicOrderVwapTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    auto AddOrders = [&order_list](const string& side, uint64_t order_id_shift)
    {
        order_list.add(order_one.order_id + order_id_shift, side, order_one.quantity, order_one.price);
        order_list.add(order_two.order_id + order_id_shift, side, order_two.quantity, order_two.price);
        order_list.add(order_three.order_id + order_id_shift, side, order_three.quantity, order_three.price);
        order_list.add(order_four.order_id + order_id_shift, side, order_four.quantity, order_four.price);
        order_list.add(order_one_dub.order_id + order_id_shift, side, order_one_dub.quantity, order_one_dub.price);
        order_list.add(order_two_dub.order_id + order_id_shift, side, order_two_dub.quantity, order_two_dub.price);
        order_list.add(order_three_dub.order_id + order_id_shift, side, order_three_dub.quantity, order_three_dub.price);
        order_list.add(order_four_dub.order_id + order_id_shift, side, order_four_dub.quantity, order_four_dub.price);
    };

    AddOrders("Buy", 0);
    AddOrders("Sell", 1000);

    uint64_t demanded_quantity = 130;

    double total_bid_price_over_quantity =
        order_one.quantity * order_one.price +
        order_one_dub.quantity * order_one_dub.price +
        order_two.quantity * order_two.price + order_two_dub.price * 5;

    double expected_bid_vwap = total_bid_price_over_quantity / demanded_quantity;

    double total_ask_price_over_quantity =
        order_four.quantity * order_four.price +
        order_four.price * 30;

    double expected_ask_vwap = total_ask_price_over_quantity / demanded_quantity;

    OrderVwap actual_vwap = order_list.vwap(demanded_quantity);

    EXPECT_EQ(expected_bid_vwap, actual_vwap.buy_price);
    EXPECT_EQ(expected_ask_vwap, actual_vwap.sell_price);
}

TEST(SymbolOrderListTestCase, TooMuchQuantityOrderVwapTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    auto AddOrders = [&order_list](const string& side, uint64_t order_id_shift)
    {
        order_list.add(order_one.order_id + order_id_shift, side, order_one.quantity, order_one.price);
        order_list.add(order_two.order_id + order_id_shift, side, order_two.quantity, order_two.price);
        order_list.add(order_three.order_id + order_id_shift, side, order_three.quantity, order_three.price);
        order_list.add(order_four.order_id + order_id_shift, side, order_four.quantity, order_four.price);
        order_list.add(order_one_dub.order_id + order_id_shift, side, order_one_dub.quantity, order_one_dub.price);
        order_list.add(order_two_dub.order_id + order_id_shift, side, order_two_dub.quantity, order_two_dub.price);
        order_list.add(order_three_dub.order_id + order_id_shift, side, order_three_dub.quantity, order_three_dub.price);
        order_list.add(order_four_dub.order_id + order_id_shift, side, order_four_dub.quantity, order_four_dub.price);
    };

    AddOrders("Buy", 0);
    AddOrders("Sell", 1000);

    uint64_t demanded_quantity = numeric_limits<uint64_t>::max();
    double expected_bid_vwap = 0.0;
    double expected_ask_vwap = 0.0;

    OrderVwap actual_vwap = order_list.vwap(demanded_quantity);
    EXPECT_EQ(expected_bid_vwap, actual_vwap.buy_price);
    EXPECT_EQ(expected_ask_vwap, actual_vwap.sell_price);
}

TEST(SymbolOrderListTestCase, EmptyListOrderVwapTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    uint64_t demanded_quantity = 125;
    double expected_bid_vwap = 0.0;
    double expected_ask_vwap = 0.0;

    OrderVwap actual_vwap = order_list.vwap(demanded_quantity);

    EXPECT_EQ(expected_bid_vwap, actual_vwap.buy_price);
    EXPECT_EQ(expected_ask_vwap, actual_vwap.sell_price);
}

TEST(SymbolOrderListTestCase, ZeroQuantityOrderVwapTest)
{
    SymbolOrderList order_list(DEFAULT_SHARE_NAME);

    EXPECT_THROW(order_list.vwap(0), OrderProcessException);
}