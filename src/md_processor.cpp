//
//  md_processor.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 16.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "md_processor.hpp"

//System includes
#include <iostream>

//Local includes
#include "order_registry.hpp"
#include "order_iterator.hpp"
#include "formatted_print.hpp"
#include "order_add_data.hpp"
//TODO: remove this
#include "command_tokenizer.hpp"
using namespace md::tokenizers;

using namespace md::processors;

/*************************** Helper Functions *************************/

namespace
{
/**
 * This function implements Order Add command
 * @param tokens for OA command
 */
void ProcessOrderAdd(const vector<string> &tokens, const string &symbol_to_filter)
{
    try
    {
        OrderAddData obj;
        uint64_t order_id = stoull(tokens[CommandTokenizer::OA_ORDER_ID]);
        const string &symbol = tokens[CommandTokenizer::OA_SYMBOL];
        const string &side = tokens[CommandTokenizer::OA_SIDE];
        uint64_t quantity = stoull(tokens[CommandTokenizer::OA_QUANTITY]);
        double price = stod(tokens[CommandTokenizer::OA_PRICE]);

        if (symbol.empty())
        {
            //Symbol can't be empty
            cout << "ProcessOrderAdd(): Symbol is empty for order with id ["
                << order_id << "]" << '\n';
            return;
        }

        auto &orders_active = OrderRegistry::get().getOrdersActive();
        if(orders_active.find(order_id) != orders_active.end())
        {
            cout << "ProcessOrderAdd(): Order with id [" << order_id <<
                "] already exists" << '\n';
            return;
        }

        auto & symbol_to_orders = OrderRegistry::get().getSymbolToOrdersBind();

        auto search = symbol_to_orders.find(symbol);

        if (search != symbol_to_orders.end())
        {
            //This symbol is registered
            search->second->add(order_id, side, quantity, price);
        }
        else
        {
            //This symbol is not registered. Need to add it
            auto added_symbol = symbol_to_orders.insert(make_pair(symbol, make_unique<SymbolOrderList>(symbol)));

            if(!added_symbol.second)
            {
                cout << "ProcessOrderAdd(): Could not register order with id [" << order_id <<
                "] and symbol [" << symbol << "]" << '\n';
                return;
            }

            added_symbol.first->second->add(order_id, side, quantity, price);
        }

        orders_active.insert({order_id, symbol});

        //Now once we have added a new order, let's print it's updated bbo and vwap
        if (symbol_to_filter == symbol || symbol_to_filter.empty())
        {
            PrintBboInfo(symbol);
            PrintVwapInfo(symbol);
        }
    }
    catch (logic_error &e)
    {
        cerr << "ProcessOrderAdd(): Bad attempt to convert string. Exception: ["
        << string(e.what()) << "]" << '\n';
        return;
    }
    catch (OrderProcessException &e)
    {
        cerr << "ProcessOrderAdd(): OrderProcessException: [" << string(e.what())
            << "]" << '\n';
        return;
    }
}

/**
 * This function implements Order Modify command
 * @param tokens for OM command
 */
void ProcessOrderModify(const vector<string> &tokens, const string &symbol_to_filter)
{
    try
    {
        uint64_t order_id = stoull(tokens[CommandTokenizer::OM_ORDER_ID]);
        uint64_t quantity = stoull(tokens[CommandTokenizer::OM_QUANTITY]);
        double price = stod(tokens[CommandTokenizer::OM_PRICE]);

        const auto &orders_active = OrderRegistry::get().getOrdersActive();
        auto search_for_active = orders_active.find(order_id);

        if( search_for_active == orders_active.end())
        {
            cout << "ProcessOrderModify(): Can't find order with id [" << order_id <<
            "]" << '\n';
            return;
        }

        const string &symbol = search_for_active->second;

        const auto &symbol_to_orders = OrderRegistry::get().getSymbolToOrdersBind();

        auto search = symbol_to_orders.find(symbol);

        if (search != symbol_to_orders.end())
        {
            //This symbol is registered
            search->second->modify(order_id, quantity, price);

            //Now once we have modified an order, let's print it's updated bbo and vwap
            if (symbol_to_filter == symbol || symbol_to_filter.empty())
            {
                PrintBboInfo(symbol);
                PrintVwapInfo(symbol);
            }
        }
        else
        {
            cout << "ProcessOrderModify(): The order is present but it's symbol is not registered. Order id:["
            << order_id << "]. Symbol [" << symbol << "]" << '\n';
            return;
        }
    }
    catch (logic_error &e)
    {
        cerr << "ProcessOrderModify(): Bad attempt to convert string. Exception: ["
        << string(e.what()) << "]" << '\n';
        return;
    }
    catch (OrderProcessException &e)
    {
        cerr << "ProcessOrderModify(): OrderProcessException: [" << string(e.what())
            << "]" << '\n';
        return;
    }
}

/**
 * This function implements Order Cancel command
 * @param tokens for OC command
 */
void ProcessOrderCancel(const vector<string> &tokens, const string &symbol_to_filter)
{
    try
    {
        uint64_t order_id = stoull(tokens[CommandTokenizer::OC_ORDER_ID]);

        auto &orders_active = OrderRegistry::get().getOrdersActive();
        auto search_for_active = orders_active.find(order_id);

        if( search_for_active == orders_active.end())
        {
            cout << "ProcessOrderCancel(): Can't find order with id [" << order_id <<
            "]" << '\n';
            return;
        }

        const string &symbol = search_for_active->second;

        const auto &symbol_to_orders = OrderRegistry::get().getSymbolToOrdersBind();

        auto search = symbol_to_orders.find(symbol);

        if (search != symbol_to_orders.end())
        {
            //This symbol is registered
            search->second->cancel(order_id);

            //Now once we have canceled an order, let's print it's updated bbo and vwap
            if (symbol_to_filter == symbol || symbol_to_filter.empty())
            {
                PrintBboInfo(symbol);
                PrintVwapInfo(symbol);
            }
        }
        else
        {
            cout << "ProcessOrderCancel(): The order is present but it's symbol is not registered. " <<
            "Erasing it anyway. Order id:[" << order_id << "]. Symbol [" << symbol << "]" << '\n';
            //Do not return from here. Since the order is not in the symbol_to_orders anyway
            //we will just erase it.
        }

        orders_active.erase(search_for_active);
    }
    catch (logic_error &e)
    {
        cerr << "ProcessOrderCancel(): Bad attempt to convert string. Exception: ["
        << string(e.what()) << "]" << '\n';
        return;
    }
    catch (OrderProcessException &e)
    {
        cerr << "ProcessOrderCancel(): OrderProcessException: [" << string(e.what())
            << "]" << '\n';
        return;
    }
}

/**
 * This function implements Subscribe Bbo command
 * @param tokens for BBO command
 * @param symbol to be shown in output
 */
void ProcessSubscribeBbo(const vector<string> &tokens, const string &)
{
    const string &symbol = tokens[CommandTokenizer::BBO_SYMBOL];

    if (symbol.empty())
    {
        //Symbol can't be empty
        cout << "ProcessSubscribeBbo(): Symbol is empty" << '\n';
        return;
    }

    auto &bbo_subscribers = OrderRegistry::get().getBboSubscribers();

    ++bbo_subscribers[symbol];
}

/**
 * This function implements Unsubscribe Bbo command
 * @param tokens for BBO command
 * @param symbol to be shown in output
 */
void ProcessUnsubscribeBbo(const vector<string> &tokens, const string &)
{
    try
    {
        const string &symbol = tokens[CommandTokenizer::BBO_SYMBOL];

        if (symbol.empty())
        {
            //Symbol can't be empty
            cout << "ProcessUnsubscribeBbo(): Symbol is empty" << '\n';
            return;
        }

        auto &bbo_subscribers = OrderRegistry::get().getBboSubscribers();

        auto &subscriber_count = bbo_subscribers.at(symbol);

        if (subscriber_count > 0)
        {
            --subscriber_count;
        }
        else
        {
            //Do nothing. We are not subscribed to anyone here
            return;
        }
    }
    catch (out_of_range &)
    {
        cerr << "ProcessUnsubscribeBbo(): Can't unsubscribe. Where was no subscriptions to this symbol: ["
            << tokens[CommandTokenizer::BBO_SYMBOL] << "]" << '\n';
        return;
    }
}

/**
 * This function implements Subscribe Vwap command
 * @param tokens for VWAP command
 * @param symbol to be shown in output
 */
void ProcessSubscribeVwap(const vector<string> &tokens, const string &)
{
    try
    {
        const string &symbol = tokens[CommandTokenizer::VWAP_SYMBOL];
        uint64_t quantity = stoull(tokens[CommandTokenizer::VWAP_QUANTITY]);

        if (symbol.empty())
        {
            //Symbol can't be empty
            cout << "ProcessSubscribeVwap(): Symbol is empty" << '\n';
            return;
        }

        if (quantity == 0)
        {
            cout << "ProcessSubscribeVwap(): Quantity can't be zero" << '\n';
            return;
        }

        auto &vwap_subscribers = OrderRegistry::get().getVwapSubscribers();

        ++vwap_subscribers[symbol][quantity];
    }
    catch (logic_error &e)
    {
        cerr << "ProcessSubscribeVwap(): Bad attempt to convert string. Exception: ["
            << string(e.what()) << "]" << '\n';
        return;
    }
}

/**
 * This function implements Unsubscribe Vwap command
 * @param tokens for VWAP command
 * @param symbol to be shown in output
 */
void ProcessUnsubscribeVwap(const vector<string> &tokens, const string &)
{
    try
    {
        const string &symbol = tokens[CommandTokenizer::VWAP_SYMBOL];
        uint64_t quantity = stoull(tokens[CommandTokenizer::VWAP_QUANTITY]);

        if (symbol.empty())
        {
            //Symbol can't be empty
            cout << "ProcessUnsubscribeVwap(): Symbol is empty" << '\n';
            return;
        }

        if (quantity == 0)
        {
            cout << "ProcessUnsubscribeVwap(): Quantity can't be zero" << '\n';
            return;
        }

        auto &vwap_subscribers = OrderRegistry::get().getVwapSubscribers();

        auto &subscriber_count = vwap_subscribers.at(symbol).at(quantity);

        if (subscriber_count > 0)
        {
            --subscriber_count;
        }
        else
        {
            //Do nothing. We are not subscribed to anyone here
            return;
        }
    }
    catch (out_of_range &)
    {
        cerr << "ProcessUnsubscribeVwap(): Can't unsubscribe. Where was no subscriptions with such configuration: ["
            << tokens[CommandTokenizer::VWAP_SYMBOL] << ", " << tokens[CommandTokenizer::VWAP_QUANTITY] << "]\n";
        return;
    }
    catch (logic_error &e)
    {
        cerr << "ProcessSubscribeVwap(): Bad attempt to convert string. Exception: ["
            << string(e.what()) << "]" << '\n';
        return;
    }
}

/**
 * This function implements Print command
 * @param tokens for PRINT command
 * @param symbol to be shown in output
 */
void ProcessPrint(const vector<string> &tokens, const string &symbol_to_filter)
{
    const string &symbol_to_print = tokens[CommandTokenizer::PRINT_SYMBOL];

    if (symbol_to_print.empty())
    {
        //Symbol can't be empty
        cout << "ProcessPrint(): Symbol is empty" << '\n';
        return;
    }

    if(symbol_to_print != symbol_to_filter && !symbol_to_filter.empty())
    {
        //Filtered out by the user
        return;
    }

    const auto &symbol_to_orders = OrderRegistry::get().getSymbolToOrdersBind();

    auto search = symbol_to_orders.find(symbol_to_print);

    if (search != symbol_to_orders.end())
    {
        //This symbol is registered

        PrintPriceLevels(search->second->getIterator(), symbol_to_print);
    }
    else
    {
        cout << "ProcessPrint(): Can't find symbol [" << symbol_to_print << "]" << '\n';
        return;
    }
}

/**
 * This function implements Print all command
 * @param tokens for PRINT_ALL command
 * @param symbol to be shown in output
 */
void ProcessPrintFull(const vector<string> &tokens, const string &symbol_to_filter)
{
    const string &symbol_to_print = tokens[CommandTokenizer::PRINT_SYMBOL];

    if (symbol_to_print.empty())
    {
        //Symbol can't be empty
        cout << "ProcessPrintFull(): Symbol is empty" << '\n';
        return;
    }

    if(symbol_to_print != symbol_to_filter && !symbol_to_filter.empty())
    {
        //Filtered out by the user
        return;
    }

    const auto &symbol_to_orders = OrderRegistry::get().getSymbolToOrdersBind();

    auto search = symbol_to_orders.find(symbol_to_print);

    if (search != symbol_to_orders.end())
    {
        //This symbol is registered

        PrintFullOrderList(search->second->getIterator(), symbol_to_print);
    }
    else
    {
        cout << "ProcessPrintFull(): Can't find symbol [" << symbol_to_print << "]" << '\n';
        return;
    }
}

} // namespace

/*************************** MdProcessor *******************************/

MdProcessor::MdProcessor() :
    handlers_
    {
        { "ORDER ADD", ProcessOrderAdd },
        { "ORDER MODIFY", ProcessOrderModify },
        { "ORDER CANCEL", ProcessOrderCancel },
        { "SUBSCRIBE BBO", ProcessSubscribeBbo },
        { "UNSUBSCRIBE BBO", ProcessUnsubscribeBbo },
        { "SUBSCRIBE VWAP", ProcessSubscribeVwap },
        { "UNSUBSCRIBE VWAP", ProcessUnsubscribeVwap },
        { "PRINT", ProcessPrint },
        { "PRINT_FULL", ProcessPrintFull }
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

void MdProcessor::process(const vector<string> &tokens)
{
    try
    {
        //TODO: fix magic number
        handlers_.at(tokens[0])(tokens, getFilter());
    }
    catch (out_of_range &e)
    {
        cerr << "MdProcessor::process(): bad key for handler map. Exception: ["
        << string(e.what()) << "]" << '\n';
        return;
    }
}
