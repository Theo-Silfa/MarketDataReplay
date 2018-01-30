//
//  bbo_subscription_data.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 30.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "bbo_subscription_data.hpp"

//System includes
#include <iostream>

//Local includes

using namespace std;
using namespace md::tokenizers;


BboSubscriptionData::BboSubscriptionData(const string &command) :
    symbol_(""),
    command_(command)
{
}


BboSubscriptionData::BboSubscriptionData(BboSubscriptionData &obj) :
    symbol_(obj.symbol_),
    command_(obj.command_)
{
}

BboSubscriptionData & BboSubscriptionData::operator=(const BboSubscriptionData &obj)
{
    symbol_ = obj.symbol_;
    command_ = obj.command_;
    return *this;
}

void BboSubscriptionData::processTokens(const vector<string> &tokens)
{
    try
    {
        if (tokens.size() != BboSubscriptionIndex::SIZE)
        {
            Parent::setProcessed(false);
            Parent::setErrorMessage("Bad number of tokens to process");
            return;
        }

        if (tokens.at(Parent::COMMAND_NAME) != command_)
        {
            Parent::setProcessed(false);
            Parent::setErrorMessage("Bad order type");
            return;
        }

        symbol_ = tokens.at(BboSubscriptionIndex::SYMBOL);

        Parent::setProcessed(true);
        Parent::setErrorMessage("Success");
        return;
    }
    catch (out_of_range &e)
    {
        cerr << "BboSubscriptionData::processTokens(): Out of range exception: ["
            << string(e.what()) << "]" << '\n';
    }

    Parent::setProcessed(false);
    Parent::setErrorMessage("Critical failure");
    return;
}

const string & BboSubscriptionData::getSymbol()
{
    return symbol_;
}
