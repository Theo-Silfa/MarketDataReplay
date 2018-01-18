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

OrderIterator::OrderIterator(OrderSetIterator bid, OrderSetIterator ask) :
    bid_itr_(bid), ask_itr_(ask)
{
}

void OrderIterator::first()
{
    return;
}

void OrderIterator::next()
{
    return;
}

bool OrderIterator::done()
{
    return false;
}

const OrderRequest & OrderIterator::getBid()
{
    return *(bid_itr_);
}

const OrderRequest & OrderIterator::getAsk()
{
    return *(ask_itr_);
}


