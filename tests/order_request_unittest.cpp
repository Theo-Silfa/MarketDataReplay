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
#include <string>

//Local includes
#include "order_request.hpp"

using namespace std;

/******************************* Helpers ******************************/

const uint64_t DEFAULT_ORDER_ID = 100;
const uint64_t DEFAULT_QUANTITY = 10;
const double DEFAULT_PRICE = 72.82;

/************************* OrderRequestTestCase ***********************/

TEST(OperatorLessTest, OrderRequestTestCase)
{
    OrderRequest lhs, rhs;

    lhs = {DEFAULT_ORDER_ID, DEFAULT_QUANTITY, DEFAULT_PRICE};
    rhs = lhs;
    rhs.price += 2.0;

    EXPECT_LT(lhs, rhs);
}

TEST(OperatorGreaterThanTest, OrderRequestTestCase)
{
    OrderRequest lhs, rhs;

    rhs = {DEFAULT_ORDER_ID, DEFAULT_QUANTITY, DEFAULT_PRICE};
    lhs = rhs;
    lhs.price += 2.0;

    EXPECT_GT(lhs, rhs);
}

TEST(OperatorEqualTest, OrderRequestTestCase)
{
    OrderRequest lhs, rhs;

    lhs = {DEFAULT_ORDER_ID, DEFAULT_QUANTITY, DEFAULT_PRICE};
    rhs = lhs;

    EXPECT_EQ(lhs, rhs);
}

TEST(VwapOutStreamTest, OrderRequestTestCase)
{
    stringstream actual, expected;
    OrderVwap obj = {DEFAULT_PRICE, DEFAULT_PRICE};

    actual << obj;
    expected << '<' << fixed << setprecision(2) << DEFAULT_PRICE << ','
        << fixed << setprecision(2) << DEFAULT_PRICE << '>';

    EXPECT_EQ(actual.str(), expected.str());
}
