//
//  order_iterator_unittest.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 20.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

//System includes
#include <gtest/gtest.h>
#include <iomanip>
#include <iostream>

//Local includes
#include "test_constants.hpp"
#include "order_iterator.hpp"

using namespace std;

/****************************** Constants *****************************/

auto orders_buy_ptr = make_shared<multiset<OrderRequest, greater<OrderRequest>>>(
        initializer_list<OrderRequest>{ order_one, order_two, order_three, order_four } );

auto orders_sell_ptr = make_shared<multiset<OrderRequest, less<OrderRequest>>>(
        initializer_list<OrderRequest>{ order_one, order_two, order_three, order_four } );

/************************ OrderIteratorTestCase ***********************/

TEST(OrderIteratorTestCase, FirstElementOnInitTest)
{
    const auto &bid_expected = order_one;
    const auto &ask_expected = order_four;

    OrderIterator itr(orders_buy_ptr, orders_sell_ptr);

    const auto &bid_actual = itr.getBid();
    const auto &ask_actual = itr.getAsk();

    EXPECT_EQ(bid_actual, bid_expected);
    EXPECT_EQ(ask_actual, ask_expected);
}

TEST(OrderIteratorTestCase, BackToFirstTest)
{
    const auto &bid_expected = order_one;
    const auto &ask_expected = order_four;

    OrderIterator itr(orders_buy_ptr, orders_sell_ptr);

    itr.next();
    itr.next();

    itr.first();

    const auto &bid_actual = itr.getBid();
    const auto &ask_actual = itr.getAsk();

    EXPECT_EQ(bid_actual, bid_expected);
    EXPECT_EQ(ask_actual, ask_expected);
}

TEST(OrderIteratorTestCase, ForwardTraverseTest)
{
    const auto &bid_expected = order_three;
    const auto &ask_expected = order_two;

    OrderIterator itr(orders_buy_ptr, orders_sell_ptr);

    itr.next();
    itr.next();

    const auto &bid_actual = itr.getBid();
    const auto &ask_actual = itr.getAsk();

    EXPECT_EQ(bid_actual, bid_expected);
    EXPECT_EQ(ask_actual, ask_expected);
}

TEST(OrderIteratorTestCase, DoneStatusOnInitTest)
{
    OrderIterator itr(orders_buy_ptr, orders_sell_ptr);

    OrderIterator::DoneStatus actual = itr.done();
    OrderIterator::DoneStatus expected = OrderIterator::NOT_DONE;

    EXPECT_EQ(actual, expected);
}

TEST(OrderIteratorTestCase, DoneStatusOnInitWithEmptyContainersTest)
{
    auto empty_orders_buy_ptr = make_shared<multiset<OrderRequest, greater<OrderRequest>>>();
    auto empty_orders_sell_ptr = make_shared<multiset<OrderRequest, less<OrderRequest>>>();

    OrderIterator itr(empty_orders_buy_ptr, empty_orders_sell_ptr);

    OrderIterator::DoneStatus actual = itr.done();
    OrderIterator::DoneStatus expected = OrderIterator::ALL_DONE;

    EXPECT_EQ(actual, expected);
}

TEST(OrderIteratorTestCase, DoneStatusOnTraverseTest)
{
    OrderIterator itr(orders_buy_ptr, orders_sell_ptr);

    itr.next();
    itr.next();

    OrderIterator::DoneStatus actual = itr.done();
    OrderIterator::DoneStatus expected = OrderIterator::NOT_DONE;

    EXPECT_EQ(actual, expected);
}

TEST(OrderIteratorTestCase, DoneStatusOnListEndTest)
{
    OrderIterator itr(orders_buy_ptr, orders_sell_ptr);

    itr.next();
    itr.next();
    itr.next();
    itr.next();

    OrderIterator::DoneStatus actual = itr.done();
    OrderIterator::DoneStatus expected = OrderIterator::ALL_DONE;

    EXPECT_EQ(actual, expected);
}

TEST(OrderIteratorTestCase, DoneStatusOnUnEqualBuyListTest)
{
    auto unequal_orders_buy_ptr = make_shared<multiset<OrderRequest, greater<OrderRequest>>>(
        initializer_list<OrderRequest>{ order_one, order_two } );

    OrderIterator itr(unequal_orders_buy_ptr, orders_sell_ptr);

    itr.next();
    itr.next();

    OrderIterator::DoneStatus actual = itr.done();
    OrderIterator::DoneStatus expected = OrderIterator::BID_DONE;

    EXPECT_EQ(actual, expected);

    itr.next();
    itr.next();

    actual = itr.done();
    expected = OrderIterator::ALL_DONE;

    EXPECT_EQ(actual, expected);
}

TEST(OrderIteratorTestCase, DoneStatusOnUnEqualSellListTest)
{
    auto unequal_orders_sell_ptr = make_shared<multiset<OrderRequest, less<OrderRequest>>>(
        initializer_list<OrderRequest>{ order_one, order_two } );

    OrderIterator itr(orders_buy_ptr, unequal_orders_sell_ptr);

    itr.next();
    itr.next();

    OrderIterator::DoneStatus actual = itr.done();
    OrderIterator::DoneStatus expected = OrderIterator::ASK_DONE;

    EXPECT_EQ(actual, expected);

    itr.next();
    itr.next();

    actual = itr.done();
    expected = OrderIterator::ALL_DONE;

    EXPECT_EQ(actual, expected);
}

