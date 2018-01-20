//
//  order_iterator.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 16.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//


#include "order_iterator.hpp"

//System includes

//Local includes
#include "order_request.hpp"

/*************************** Helper Functions *************************/

/*************************** OrderIterator ****************************/

OrderIterator::OrderIterator(OrderSetGreaterPtr bid, OrderSetLessPtr ask) :
    bid_(bid), ask_(ask), current_bid_(bid->begin()), current_ask_(ask->begin())
{
}

void OrderIterator::first()
{
    current_bid_ = bid_->begin();
    current_ask_ = ask_->begin();
}

void OrderIterator::next()
{
    DoneStatus status = done();

    if(status != DoneStatus::BID_DONE)
    {
        ++current_bid_;
    }

    if(status != DoneStatus::ASK_DONE)
    {
        ++current_ask_;
    }
}

OrderIterator::DoneStatus OrderIterator::done()
{
    DoneStatus status;

    if(current_bid_ == bid_->end() && current_ask_ == ask_->end())
    {
        status = DoneStatus::ALL_DONE;
    }
    else if(current_bid_ == bid_->end() && current_ask_ != ask_->end())
    {
        status = DoneStatus::BID_DONE;
    }
    else if(current_bid_ != bid_->end() && current_ask_ == ask_->end())
    {
        status = DoneStatus::ASK_DONE;
    }
    else
    {
        status = DoneStatus::NOT_DONE;
    }

    return status;
}

const OrderRequest & OrderIterator::getBid()
{
    return *(current_bid_);
}

const OrderRequest & OrderIterator::getAsk()
{
    return *(current_ask_);
}


