//
//  md_processor.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 16.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "md_processor.hpp"

//System includes
#include <set>
#include <iostream>

//Local includes
#include "order_registry.hpp"

using namespace md::processors;

/*************************** Helper Functions *************************/

namespace
{
/**
 * This function implements Order Add command
 * @param tokens for OA command
 */
void ProcessOrderAdd(const CommandTokenizer &tokens)
{
    try
    {
        uint64_t order_id = stoull(tokens[CommandTokenizer::OA_ORDER_ID]);
        string symbol = tokens[CommandTokenizer::OA_SYMBOL];
        string side = tokens[CommandTokenizer::OA_SIDE];
        uint64_t quantity = stoull(tokens[CommandTokenizer::OA_QUANTITY]);
        double price = stod(tokens[CommandTokenizer::OA_PRICE]);

        auto & orders_active = OrderRegistry::get().getOrdersActive();
        if(orders_active.find(order_id) != orders_active.end())
        {
            cout << "ProcessOrderAdd(): Order with id [" << order_id <<
                "] already exists" << '\n';
            return;
        }

        orders_active.insert({order_id, symbol});

        cout << "Recieved order: " << order_id << "|" << symbol << "|" << side << "|"
        << quantity << "|" << price << '\n';
    }
    catch (logic_error &e)
    {
        cerr << "ProcessOrderAdd(): Bad attempt to convert string. Exception: ["
        << string(e.what()) << "]" << '\n';
        return;
    }
}

/**
 * This function implements Order Modify command
 * @param tokens for OM command
 */
void ProcessOrderModify(const CommandTokenizer &tokens)
{
    try
    {
        uint64_t order_id = stoull(tokens[CommandTokenizer::OM_ORDER_ID]);
        uint64_t quantity = stoull(tokens[CommandTokenizer::OM_QUANTITY]);
        double price = stod(tokens[CommandTokenizer::OM_PRICE]);

        auto & orders_active = OrderRegistry::get().getOrdersActive();
        auto search = orders_active.find(order_id);

        if( search == orders_active.end())
        {
            cout << "ProcessOrderModify(): Can't find order with id [" << order_id <<
            "]" << '\n';
            return;
        }

        cout << "Modified order: " << order_id << "|"<< search->second << "|"
        << quantity << "|" << price << '\n';
    }
    catch (logic_error &e)
    {
        cerr << "ProcessOrderAdd(): Bad attempt to convert string. Exception: ["
        << string(e.what()) << "]" << '\n';
        return;
    }
}

/**
 * This function implements Order Cancel command
 * @param tokens for OC command
 */
void ProcessOrderCancel(const CommandTokenizer &tokens)
{
    try
    {
        uint64_t order_id = stoull(tokens[CommandTokenizer::OC_ORDER_ID]);

        auto & orders_active = OrderRegistry::get().getOrdersActive();
        auto search = orders_active.find(order_id);

        if( search == orders_active.end())
        {
            cout << "ProcessOrderCancel(): Can't find order with id [" << order_id <<
            "]" << '\n';
            return;
        }

        cout << "Canceled order: " << order_id << "|" << search->second << '\n';
        orders_active.erase(search);
    }
    catch (logic_error &e)
    {
        cerr << "ProcessOrderAdd(): Bad attempt to convert string. Exception: ["
        << string(e.what()) << "]" << '\n';
        return;
    }
}

/**
 * This function implements Subscribe Bbo command
 * @param tokens for BBO command
 * @param symbol to be shown in output
 */
void ProcessSubscribeBbo(const CommandTokenizer &tokens, const string &symbol) {}

/**
 * This function implements Unsubscribe Bbo command
 * @param tokens for BBO command
 * @param symbol to be shown in output
 */
void ProcessUnsubscribeBbo(const CommandTokenizer &tokens, const string &symbol) {}

/**
 * This function implements Subscribe Vwap command
 * @param tokens for VWAP command
 * @param symbol to be shown in output
 */
void ProcessSubscribeVwap(const CommandTokenizer &tokens, const string &symbol) {}

/**
 * This function implements Unsubscribe Vwap command
 * @param tokens for VWAP command
 * @param symbol to be shown in output
 */
void ProcessUnsubscribeVwap(const CommandTokenizer &tokens, const string &symbol) {}

/**
 * This function implements Print command
 * @param tokens for PRINT command
 * @param symbol to be shown in output
 */
void ProcessPrint(const CommandTokenizer &tokens, const string &symbol) {}

/**
 * This function implements Print all command
 * @param tokens for PRINT_ALL command
 * @param symbol to be shown in output
 */
void ProcessPrintFull(const CommandTokenizer &tokens, const string &symbol) {}

} // namespace

/*************************** MdProcessor *******************************/

MdProcessor::MdProcessor() :
    handlers_
    {
        { "ORDER ADD", ProcessOrderAdd },
        { "ORDER MODIFY", ProcessOrderModify },
        { "ORDER CANCEL", ProcessOrderCancel },
        { "SUBSCRIBE BBO", bind(ProcessSubscribeBbo, placeholders::_1, cref(getFilter()))},
        { "UNSUBSCRIBE BBO", bind(ProcessUnsubscribeBbo, placeholders::_1, cref(getFilter()))},
        { "SUBSCRIBE VWAP", bind(ProcessSubscribeVwap, placeholders::_1, cref(getFilter()))},
        { "UNSUBSCRIBE VWAP", bind(ProcessUnsubscribeVwap, placeholders::_1, cref(getFilter()))},
        { "PRINT", bind(ProcessPrint, placeholders::_1, cref(getFilter()))},
        { "PRINT_FULL", bind(ProcessPrintFull, placeholders::_1, cref(getFilter()))}
    }
{
}

void MdProcessor::setFilter(const string &val)
{
    symbol_ = val;
}

const string & MdProcessor::getFilter() const
{
    return symbol_;
}

void MdProcessor::process(const CommandTokenizer &tokens)
{
    try
    {
        auto key = tokens[CommandTokenizer::COMMAND_NAME];
        handlers_.at(key)(tokens);
    }
    catch (out_of_range &e)
    {
        cerr << "MdProcessor::process(): bad key for handler map. Exception: ["
        << string(e.what()) << "]" << '\n';
        return;
    }
}
