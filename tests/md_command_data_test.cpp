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
#include "order_modify_data.hpp"
#include "order_cancel_data.hpp"
#include "bbo_subscription_data.hpp"
#include "vwap_subscription_data.hpp"
#include "print_data.hpp"

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
        { "ORDER ADD,1000,AAPL,Buy," + overflown_uint64_t + ",72.82", "Critical failure" },
        { "ORDER ADD,1000,AAPL,Buy,10,1.79769e+309",      "Critical failure" }
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

/************************** OrderModifyData ***************************/

TEST(OrderModifyDataTestCase, RegularRunTest)
{
    OrderModifyData obj;

    //Check initial values
    EXPECT_FALSE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Object is empty");

    EXPECT_EQ(obj.getOrderId(), 0ull);
    EXPECT_EQ(obj.getQuantity(), 0ull);
    EXPECT_EQ(obj.getPrice(), 0.0);

    //Check processed values
    auto tokens = GetTokens("ORDER MODIFY,1000,11,72.81");
    obj.processTokens(tokens);

    EXPECT_TRUE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Success");

    EXPECT_EQ(obj.getOrderId(), 1000ull);
    EXPECT_EQ(obj.getQuantity(), 11ull);
    EXPECT_EQ(obj.getPrice(), 72.81);
}

TEST(OrderModifyDataTestCase, EmptyTokenTest)
{
    vector<string> empty_commands =
        {
            ",,,,,",
            "ORDER MODIFY,,,",
            ",1000,,",
            ",,11,",
            ",,,72.81",
            ",1000,11,72.81",
            "ORDER MODIFY,,11,72.81",
            "ORDER MODIFY,1000,,72.81",
            "ORDER MODIFY,1000,11,"
        };

    OrderModifyData obj;
    vector<string> tokens;

    for (const auto &command : empty_commands)
    {
        tokens = GetTokens(command);
        obj.processTokens(tokens);

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), "Bad number of tokens to process");
    }
}

TEST(OrderModifyDataTestCase, UnexpectedValuesTest)
{
    vector<pair<string, string>> commands_to_expected_err_msg =
    {
        { "BADTYPE,1000,11,72.81",               "Bad order type" },
        { "ORDER MODIFY,BADORDERID,11,72.81",    "Critical failure" },
        { "ORDER MODIFY,1000,BADQUANTITY,72.81", "Critical failure" },
        { "ORDER MODIFY,1000,11,BADPRICE",       "Critical failure" }
    };

    OrderModifyData obj;
    vector<string> tokens;

    for (const auto &value : commands_to_expected_err_msg)
    {
        tokens = GetTokens(value.first);
        EXPECT_NO_THROW(obj.processTokens(tokens));

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), value.second);
    }
}

TEST(OrderModifyDataTestCase, NumericalOverflowTest)
{
    const string overflown_uint64_t = to_string(numeric_limits<uint64_t>::max()) + '1';

    vector<pair<string, string>> commands_to_expected_err_msg =
    {
        { "ORDER MODIFY," + overflown_uint64_t + ",11,72.81",   "Critical failure" },
        { "ORDER MODIFY,1000," + overflown_uint64_t + ",72.81", "Critical failure" },
        { "ORDER MODIFY,1000,11,1.79769e+309",                  "Critical failure" }
    };

    OrderModifyData obj;
    vector<string> tokens;

    for (const auto &value : commands_to_expected_err_msg)
    {
        tokens = GetTokens(value.first);
        EXPECT_NO_THROW(obj.processTokens(tokens));

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), value.second);
    }
}

/************************** OrderCancelData ***************************/

TEST(OrderCancelDataTestCase, RegularRunTest)
{
    OrderCancelData obj;

    //Check initial values
    EXPECT_FALSE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Object is empty");

    EXPECT_EQ(obj.getOrderId(), 0ull);

    //Check processed values
    auto tokens = GetTokens("ORDER CANCEL,1000");
    obj.processTokens(tokens);

    EXPECT_TRUE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Success");

    EXPECT_EQ(obj.getOrderId(), 1000ull);
}

TEST(OrderCancelDataTestCase, EmptyTokenTest)
{
    vector<string> empty_commands =
        {
            ",",
            "ORDER CANCEL,",
            ",1000"
        };

    OrderCancelData obj;
    vector<string> tokens;

    for (const auto &command : empty_commands)
    {
        tokens = GetTokens(command);
        obj.processTokens(tokens);

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), "Bad number of tokens to process");
    }
}

TEST(OrderCancelDataTestCase, UnexpectedValuesTest)
{
    vector<pair<string, string>> commands_to_expected_err_msg =
    {
        { "BADTYPE,1000",               "Bad order type" },
        { "ORDER CANCEL,BADORDERID",    "Critical failure" }
    };

    OrderCancelData obj;
    vector<string> tokens;

    for (const auto &value : commands_to_expected_err_msg)
    {
        tokens = GetTokens(value.first);
        EXPECT_NO_THROW(obj.processTokens(tokens));

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), value.second);
    }
}

TEST(OrderCancelDataTestCase, NumericalOverflowTest)
{
    const string overflown_uint64_t = to_string(numeric_limits<uint64_t>::max()) + '1';

    vector<pair<string, string>> commands_to_expected_err_msg =
    {
        { "ORDER CANCEL," + overflown_uint64_t, "Critical failure" },
    };

    OrderCancelData obj;
    vector<string> tokens;

    for (const auto &value : commands_to_expected_err_msg)
    {
        tokens = GetTokens(value.first);
        EXPECT_NO_THROW(obj.processTokens(tokens));

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), value.second);
    }
}

/**************************** BboSubscriptionData ****************************/

TEST(BboSubscriptionDataTestCase, RegularRunTest)
{
    BboSubscriptionData obj("SUBSCRIBE BBO");

    //Check initial values
    EXPECT_FALSE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Object is empty");

    EXPECT_EQ(obj.getSymbol(), "");

    //Check processed values
    auto tokens = GetTokens("SUBSCRIBE BBO,AAPL");
    obj.processTokens(tokens);

    EXPECT_TRUE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Success");

    EXPECT_EQ(obj.getSymbol(), "AAPL");
}

TEST(BboSubscriptionDataTestCase, EmptyTokenTest)
{
    vector<string> empty_commands =
        {
            ",",
            "SUBSCRIBE BBO,",
            ",AAPL",
        };

    BboSubscriptionData obj("SUBSCRIBE BBO");
    vector<string> tokens;

    for (const auto &command : empty_commands)
    {
        tokens = GetTokens(command);
        obj.processTokens(tokens);

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), "Bad number of tokens to process");
    }
}

TEST(BboSubscriptionDataTestCase, UnexpectedValuesTest)
{
    vector<pair<string, string>> commands_to_expected_err_msg =
    {
        { "BADTYPE,AAPL", "Bad order type" }
    };

    BboSubscriptionData obj("SUBSCRIBE BBO");
    vector<string> tokens;

    for (const auto &value : commands_to_expected_err_msg)
    {
        tokens = GetTokens(value.first);
        EXPECT_NO_THROW(obj.processTokens(tokens));

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), value.second);
    }
}

/**************************** VwapSubscriptionData ****************************/

TEST(VwapSubscriptionDataTestCase, RegularRunTest)
{
    VwapSubscriptionData obj("SUBSCRIBE VWAP");

    //Check initial values
    EXPECT_FALSE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Object is empty");

    EXPECT_EQ(obj.getSymbol(), "");
    EXPECT_EQ(obj.getQuantity(), 0ull);

    //Check processed values
    auto tokens = GetTokens("SUBSCRIBE VWAP,AAPL,5");
    obj.processTokens(tokens);

    EXPECT_TRUE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Success");

    EXPECT_EQ(obj.getSymbol(), "AAPL");
    EXPECT_EQ(obj.getQuantity(), 5ull);
}

TEST(VwapSubscriptionDataTestCase, EmptyTokenTest)
{
    vector<string> empty_commands =
        {
            ",,",
            "SUBSCRIBE VWAP,,",
            ",AAPL,",
            ",,5",
            ",AAPL,5",
            "SUBSCRIBE VWAP,,5",
            "SUBSCRIBE VWAP,AAPL,"
        };

    VwapSubscriptionData obj("SUBSCRIBE VWAP");
    vector<string> tokens;

    for (const auto &command : empty_commands)
    {
        tokens = GetTokens(command);
        obj.processTokens(tokens);

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), "Bad number of tokens to process");
    }
}

TEST(VwapSubscriptionDataTestCase, UnexpectedValuesTest)
{
    vector<pair<string, string>> commands_to_expected_err_msg =
    {
        { "BADTYPE,AAPL,5",                  "Bad order type" },
        { "SUBSCRIBE VWAP,AAPL,BADQUANTITY", "Critical failure" }
    };

    VwapSubscriptionData obj("SUBSCRIBE VWAP");
    vector<string> tokens;

    for (const auto &value : commands_to_expected_err_msg)
    {
        tokens = GetTokens(value.first);
        EXPECT_NO_THROW(obj.processTokens(tokens));

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), value.second);
    }
}

TEST(VwapSubscriptionDataTestCase, NumericalOverflowTest)
{
    const string overflown_uint64_t = to_string(numeric_limits<uint64_t>::max()) + '1';

    vector<pair<string, string>> commands_to_expected_err_msg =
    {
        { "SUBSCRIBE VWAP,AAPL," + overflown_uint64_t, "Critical failure" },
    };

    VwapSubscriptionData obj("SUBSCRIBE VWAP");
    vector<string> tokens;

    for (const auto &value : commands_to_expected_err_msg)
    {
        tokens = GetTokens(value.first);
        EXPECT_NO_THROW(obj.processTokens(tokens));

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), value.second);
    }
}

/**************************** PrintData ****************************/

TEST(PrintDataTestCase, RegularRunTest)
{
    PrintData obj("PRINT");

    //Check initial values
    EXPECT_FALSE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Object is empty");

    EXPECT_EQ(obj.getSymbol(), "");

    //Check processed values
    auto tokens = GetTokens("PRINT,AAPL");
    obj.processTokens(tokens);

    EXPECT_TRUE(obj.isProcessed());
    EXPECT_EQ(obj.errorMessage(), "Success");

    EXPECT_EQ(obj.getSymbol(), "AAPL");
}

TEST(PrintDataTestCase, EmptyTokenTest)
{
    vector<string> empty_commands =
        {
            ",",
            "PRINT,",
            ",AAPL",
        };

    PrintData obj("PRINT");
    vector<string> tokens;

    for (const auto &command : empty_commands)
    {
        tokens = GetTokens(command);
        obj.processTokens(tokens);

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), "Bad number of tokens to process");
    }
}

TEST(PrintDataTestCase, UnexpectedValuesTest)
{
    vector<pair<string, string>> commands_to_expected_err_msg =
    {
        { "BADTYPE,AAPL", "Bad order type" }
    };

    PrintData obj("PRINT");
    vector<string> tokens;

    for (const auto &value : commands_to_expected_err_msg)
    {
        tokens = GetTokens(value.first);
        EXPECT_NO_THROW(obj.processTokens(tokens));

        EXPECT_FALSE(obj.isProcessed());
        EXPECT_EQ(obj.errorMessage(), value.second);
    }
}
