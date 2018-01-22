//
//  order_request_unittest.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 18.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

//System includes
#include <gtest/gtest.h>
#include <sstream>
#include <iomanip>

//Local includes
#include "test_constants.hpp"
#include "order_request.hpp"

using namespace std;

/************************* OrderRequestTestCase ***********************/

TEST(OrderRequestTestCase, OperatorLessTest)
{
    OrderRequest lhs, rhs;

    lhs = {DEFAULT_ORDER_ID, DEFAULT_QUANTITY, DEFAULT_PRICE};
    rhs = lhs;
    rhs.price += 2.0;

    EXPECT_LT(lhs, rhs);
}

TEST(OrderRequestTestCase, OperatorGreaterThanTest)
{
    OrderRequest lhs, rhs;

    rhs = {DEFAULT_ORDER_ID, DEFAULT_QUANTITY, DEFAULT_PRICE};
    lhs = rhs;
    lhs.price += 2.0;

    EXPECT_GT(lhs, rhs);
}

TEST(OrderRequestTestCase, OperatorEqualTest)
{
    OrderRequest lhs, rhs;

    lhs = {DEFAULT_ORDER_ID, DEFAULT_QUANTITY, DEFAULT_PRICE};
    rhs = lhs;

    EXPECT_EQ(lhs, rhs);
}

TEST(OrderRequestTestCase, VwapOutStreamTest)
{
    stringstream actual, expected;
    OrderVwap obj = {DEFAULT_PRICE, DEFAULT_PRICE};

    actual << obj;
    expected << '<' << fixed << setprecision(2) << DEFAULT_PRICE << ','
        << fixed << setprecision(2) << DEFAULT_PRICE << '>';

    EXPECT_EQ(actual.str(), expected.str());
}

TEST(OrderRequestTestCase, VwapOutStreamBuyNilTest)
{
    stringstream actual, expected;
    OrderVwap obj = {0.0, DEFAULT_PRICE};

    actual << obj;
    expected << '<' << fixed << setprecision(2) << NIL << ','
        << fixed << setprecision(2) << DEFAULT_PRICE << '>';

    EXPECT_EQ(actual.str(), expected.str());
}

TEST(OrderRequestTestCase, VwapOutStreamSellNilTest)
{
    stringstream actual, expected;
    OrderVwap obj = {DEFAULT_PRICE, 0.0};

    actual << obj;
    expected << '<' << fixed << setprecision(2) << DEFAULT_PRICE << ','
        << fixed << setprecision(2) << NIL << '>';

    EXPECT_EQ(actual.str(), expected.str());
}

TEST(OrderRequestTestCase, VwapOutStreamNilTest)
{
    stringstream actual, expected;
    OrderVwap obj = {0.0, 0.0};

    actual << obj;
    expected << '<' << fixed << setprecision(2) << NIL << ','
        << fixed << setprecision(2) << NIL << '>';

    EXPECT_EQ(actual.str(), expected.str());
}