//
//  order_bbo_unittest.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 18.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

//System includes
#include <gtest/gtest.h>
#include <iomanip>

//Local includes
#include "test_constants.hpp"
#include "order_bbo.hpp"

using namespace std;

/************************** OrderBboTestCase **************************/

TEST(OrderBboTestCase, RegularRunTest)
{
    OrderBbo bbo(bbo_buy_volume, bbo_buy_price, orders_in_buy_range,
        bbo_sell_volume, bbo_sell_price, orders_in_sell_range);

    stringstream actual, expected;

    actual << bbo;

    expected << '|' << setw(default_width) << orders_in_buy_range
            << '|' << setw(default_width) << bbo_buy_volume
            << '|' << setw(default_width) << fixed << setprecision(default_precision) << bbo_buy_price
            << '|' << setw(default_width) << fixed << setprecision(default_precision) << bbo_sell_price
            << '|' << setw(default_width) << bbo_sell_volume
            << '|' << setw(default_width) << orders_in_sell_range
            << '|';

    EXPECT_EQ(actual.str(), expected.str());
}

TEST(OrderBboTestCase, BuySellNilTest)
{
    OrderBbo bbo(0, 0.0, 0, 0, 0.0, 0);

    bbo.setBuyNil(true);
    bbo.setSellNil(true);

    stringstream actual, expected;

    actual << bbo;

    expected << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|';

    EXPECT_EQ(actual.str(), expected.str());
}

TEST(OrderBboTestCase, BuyNilRangeTest)
{
    OrderBbo bbo(0, 0.0, 0, bbo_sell_volume, bbo_sell_price, orders_in_sell_range);

    bbo.setBuyNil(true);

    stringstream actual, expected;

    actual << bbo;

    expected << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << fixed << setprecision(default_precision) << bbo_sell_price
            << '|' << setw(default_width) << bbo_sell_volume
            << '|' << setw(default_width) << orders_in_sell_range
            << '|';

    EXPECT_EQ(actual.str(), expected.str());
}

TEST(OrderBboTestCase, SellNilRangeTest)
{
    OrderBbo bbo(bbo_buy_volume, bbo_buy_price, orders_in_buy_range, 0, 0.0, 0);

    bbo.setSellNil(true);

    stringstream actual, expected;

    actual << bbo;

    expected << '|' << setw(default_width) << orders_in_buy_range
            << '|' << setw(default_width) << bbo_buy_volume
            << '|' << setw(default_width) << fixed << setprecision(default_precision) << bbo_buy_price
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|' << setw(default_width) << NIL
            << '|';

    EXPECT_EQ(actual.str(), expected.str());
}
