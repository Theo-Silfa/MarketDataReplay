//
//  md_command_data_test.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 30.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

//System includes
#include <gtest/gtest.h>
#include <limits>

//Local includes
#include "split.hpp"
#include "order_add_data.hpp"

using namespace std;
using namespace md::tokenizers;

/******************************* Helpers ******************************/

vector<string> GetTokens(const string & target_command)
{
    return split(target_command, ',');
}

/**************************** OrderAddData ****************************/

TEST(OrderAddDataTestCase, RegularRunTest)
{
    OrderAddData obj;

    //Check initial values
    EXPECT_FALSE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Object is empty");

    EXPECT_EQ(obj.getOrderId(), 0ull);
    EXPECT_EQ(obj.getSymbol(), "");
    EXPECT_EQ(obj.getSide(), OrderSide::UNKNOWN);
    EXPECT_EQ(obj.getQuantity(), 0ull);
    EXPECT_EQ(obj.getPrice(), 0.0);

    //Check for buy side
    auto tokens = GetTokens("ORDER ADD,1000,AAPL,Buy,10,72.82");
    obj.processTokens(tokens);

    EXPECT_TRUE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Success");

    EXPECT_EQ(obj.getOrderId(), 1000ull);
    EXPECT_EQ(obj.getSymbol(), "AAPL");
    EXPECT_EQ(obj.getSide(), OrderSide::BUY);
    EXPECT_EQ(obj.getQuantity(), 10ull);
    EXPECT_EQ(obj.getPrice(), 72.82);

    //Check for sell side
    tokens = GetTokens("ORDER ADD,1007,AAPL,Sell,100,72.85");
    obj.processTokens(tokens);

    EXPECT_TRUE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Success");

    EXPECT_EQ(obj.getOrderId(), 1007ull);
    EXPECT_EQ(obj.getSymbol(), "AAPL");
    EXPECT_EQ(obj.getSide(), OrderSide::SELL);
    EXPECT_EQ(obj.getQuantity(), 100ull);
    EXPECT_EQ(obj.getPrice(), 72.85);
}

TEST(OrderAddDataTestCase, EmptyTokenTest)
{
    vector<string> empty_commands =
        {
            ",,,,,",
            "ORDER ADD,,,,,",
            ",1000,,,,",
            ",,AAPL,,,",
            ",,,Buy,,",
            ",,,,10,",
            ",,,,,72.82",
            ",1000,AAPL,Buy,10,72.82",
            "ORDER ADD,,AAPL,Buy,10,72.82",
            "ORDER ADD,1000,,Buy,10,72.82",
            "ORDER ADD,1000,AAPL,,10,72.82",
            "ORDER ADD,1000,AAPL,Buy,,72.82",
            "ORDER ADD,1000,AAPL,Buy,10,"
        };

    OrderAddData obj;
    vector<string> tokens;

    for (const auto &command : empty_commands)
    {
        tokens = GetTokens(command);
        obj.processTokens(tokens);

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), "Bad number of tokens to process");
    }
}

TEST(OrderAddDataTestCase, UnexpectedValuesTest)
{
    vector<pair<string, string>> commands_to_expected_err_msg =
    {
        { "BADTYPE,1000,AAPL,Buy,10,72.82",            "Bad order type" },
        { "ORDER ADD,BADORDERID,AAPL,Buy,10,72.82",    "Critical failure" },
        { "ORDER ADD,1000,AAPL,BADSIDE,10,72.82",      "Bad side" },
        { "ORDER ADD,1000,AAPL,Buy,BADQUANTITY,72.82", "Critical failure" },
        { "ORDER ADD,1000,AAPL,Buy,10,BADPRICE",       "Critical failure" }
    };

    OrderAddData obj;
    vector<string> tokens;

    for (const auto &value : commands_to_expected_err_msg)
    {
        tokens = GetTokens(value.first);
        EXPECT_NO_THROW(obj.processTokens(tokens));

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), value.second);
    }
}

TEST(OrderAddDataTestCase, NumericalOverflowTest)
{
    const string overflown_uint64_t = to_string(numeric_limits<uint64_t>::max()) + '1';

    vector<pair<string, string>> commands_to_expected_err_msg =
    {
        { "ORDER ADD," + overflown_uint64_t + ",AAPL,Buy,10,72.82",   "Critical failure" },
        { "ORDER ADD,1000,AAPL,Buy," + overflown_uint64_t + ",72.82", "Critical failure" }
        //TODO: How to overflow double?
        //{ "ORDER ADD,1000,AAPL,Buy,10," + to_string(numeric_limits<double>::max()),      "Critical failure" }
    };

    OrderAddData obj;
    vector<string> tokens;

    for (const auto &value : commands_to_expected_err_msg)
    {
        tokens = GetTokens(value.first);
        EXPECT_NO_THROW(obj.processTokens(tokens));

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), value.second);
    }
}