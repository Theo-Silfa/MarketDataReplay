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
#include "order_modify_data.hpp"
#include "order_cancel_data.hpp"
#include "bbo_subscription_data.hpp"
#include "vwap_subscription_data.hpp"
#include "print_data.hpp"

using namespace md::tokenizers;
using namespace md::processors;

/*************************** Helper Functions *************************/

namespace
{
/**
 * This function implements Order Add command
 * @param tokens for OA command
 */
bool ProcessOrderAdd(const vector<string> &tokens, const string &symbol_to_filter)
{
    try
    {
        static OrderAddData obj;
        obj.processTokens(tokens);

        if (!obj.isProcessed())
        {
            cout << "ProcessOrderAdd(): Tokens were not processed successfully. Reason: ["
                << obj.errorMessage() << "]" << '\n';
            return false;
        }

        uint64_t order_id = obj.getOrderId();
        const string &symbol = obj.getSymbol();
        OrderSide side = obj.getSide();
        uint64_t quantity = obj.getQuantity();
        double price = obj.getPrice();

        auto &orders_active = OrderRegistry::get().getOrdersActive();
        if(orders_active.find(order_id) != orders_active.end())
        {
            cout << "ProcessOrderAdd(): Order with id [" << order_id <<
                "] already exists" << '\n';
            return false;
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
                return false;
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

        return true;
    }
    catch (OrderProcessException &e)
    {
        cerr << "ProcessOrderAdd(): OrderProcessException: [" << string(e.what())
            << "]" << '\n';
        return false;
    }
}

/**
 * This function implements Order Modify command
 * @param tokens for OM command
 */
bool ProcessOrderModify(const vector<string> &tokens, const string &symbol_to_filter)
{
    try
    {
        static OrderModifyData obj;
        obj.processTokens(tokens);

        if (!obj.isProcessed())
        {
            cout << "ProcessOrderModify(): Tokens were not processed successfully. Reason: ["
                << obj.errorMessage() << "]" << '\n';
            return false;
        }

        uint64_t order_id = obj.getOrderId();
        uint64_t quantity = obj.getQuantity();
        double price = obj.getPrice();

        const auto &orders_active = OrderRegistry::get().getOrdersActive();
        auto search_for_active = orders_active.find(order_id);

        if( search_for_active == orders_active.end())
        {
            cout << "ProcessOrderModify(): Order with such id is not registered in the system ["
                << order_id << "]" << '\n';
            return false;
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
            return false;
        }

        return true;
    }
    catch (OrderProcessException &e)
    {
        cerr << "ProcessOrderModify(): OrderProcessException: [" << string(e.what())
            << "]" << '\n';
        return false;
    }
}

/**
 * This function implements Order Cancel command
 * @param tokens for OC command
 */
bool ProcessOrderCancel(const vector<string> &tokens, const string &symbol_to_filter)
{
    try
    {
        static OrderCancelData obj;
        obj.processTokens(tokens);

        if (!obj.isProcessed())
        {
            cout << "ProcessOrderCancel(): Tokens were not processed successfully. Reason: ["
                << obj.errorMessage() << "]" << '\n';
            return false;
        }

        uint64_t order_id = obj.getOrderId();

        auto &orders_active = OrderRegistry::get().getOrdersActive();
        auto search_for_active = orders_active.find(order_id);

        if( search_for_active == orders_active.end())
        {
            cout << "ProcessOrderCancel(): Order with such id is not registered in the system ["
                << order_id << "]" << '\n';
            return false;
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

        return true;
    }
    catch (OrderProcessException &e)
    {
        cerr << "ProcessOrderCancel(): OrderProcessException: [" << string(e.what())
            << "]" << '\n';
        return false;
    }
}

/**
 * This function implements Subscribe Bbo command
 * @param tokens for BBO command
 * @param symbol to be shown in output
 */
bool ProcessSubscribeBbo(const vector<string> &tokens, const string &)
{
    static BboSubscriptionData obj("SUBSCRIBE BBO");
    obj.processTokens(tokens);

    if (!obj.isProcessed())
    {
        cout << "ProcessSubscribeBbo(): Tokens were not processed successfully. Reason: ["
            << obj.errorMessage() << "]" << '\n';
        return false;
    }

    const string &symbol = obj.getSymbol();

    auto &bbo_subscribers = OrderRegistry::get().getBboSubscribers();

    ++bbo_subscribers[symbol];

    return true;
}

/**
 * This function implements Unsubscribe Bbo command
 * @param tokens for BBO command
 * @param symbol to be shown in output
 */
bool ProcessUnsubscribeBbo(const vector<string> &tokens, const string &)
{
    static BboSubscriptionData obj("UNSUBSCRIBE BBO");
    obj.processTokens(tokens);

    if (!obj.isProcessed())
    {
        cout << "ProcessUnsubscribeBbo(): Tokens were not processed successfully. Reason: ["
            << obj.errorMessage() << "]" << '\n';
        return false;
    }

    const string &symbol = obj.getSymbol();

    try
    {
        auto &bbo_subscribers = OrderRegistry::get().getBboSubscribers();

        auto &subscriber_count = bbo_subscribers.at(symbol);

        if (subscriber_count > 0)
        {
            --subscriber_count;
        }
        else
        {
            //Do nothing. We are not subscribed to anyone here
        }

        return true;
    }
    catch (out_of_range &)
    {
        cerr << "ProcessUnsubscribeBbo(): Can't unsubscribe. Where was no subscriptions to this symbol: ["
            << symbol << "]" << '\n';
        return false;
    }
}

/**
 * This function implements Subscribe Vwap command
 * @param tokens for VWAP command
 * @param symbol to be shown in output
 */
bool ProcessSubscribeVwap(const vector<string> &tokens, const string &)
{
    static VwapSubscriptionData obj("SUBSCRIBE VWAP");
    obj.processTokens(tokens);

    if (!obj.isProcessed())
    {
        cout << "ProcessSubscribeVwap(): Tokens were not processed successfully. Reason: ["
            << obj.errorMessage() << "]" << '\n';
        return false;
    }

    const string &symbol = obj.getSymbol();
    uint64_t quantity = obj.getQuantity();

    if (quantity == 0)
    {
        cout << "ProcessSubscribeVwap(): Quantity can't be zero" << '\n';
        return false;
    }

    auto &vwap_subscribers = OrderRegistry::get().getVwapSubscribers();

    ++vwap_subscribers[symbol][quantity];

    return true;
}

/**
 * This function implements Unsubscribe Vwap command
 * @param tokens for VWAP command
 * @param symbol to be shown in output
 */
bool ProcessUnsubscribeVwap(const vector<string> &tokens, const string &)
{
    static VwapSubscriptionData obj("UNSUBSCRIBE VWAP");
    obj.processTokens(tokens);

    if (!obj.isProcessed())
    {
        cout << "ProcessUnsubscribeVwap(): Tokens were not processed successfully. Reason: ["
            << obj.errorMessage() << "]" << '\n';
        return false;
    }

    const string &symbol = obj.getSymbol();
    uint64_t quantity = obj.getQuantity();

    try
    {
        if (quantity == 0)
        {
            cout << "ProcessUnsubscribeVwap(): Quantity can't be zero" << '\n';
            return false;
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
        }

        return true;
    }
    catch (out_of_range &)
    {
        cerr << "ProcessUnsubscribeVwap(): Can't unsubscribe. Where was no subscriptions with such configuration: ["
            << symbol << ", " << quantity << "]\n";
        return false;
    }
}

/**
 * This function implements Print command
 * @param tokens for PRINT command
 * @param symbol to be shown in output
 */
bool ProcessPrint(const vector<string> &tokens, const string &symbol_to_filter)
{
    static PrintData obj("PRINT");
    obj.processTokens(tokens);

    if (!obj.isProcessed())
    {
        cout << "ProcessPrint(): Tokens were not processed successfully. Reason: ["
            << obj.errorMessage() << "]" << '\n';
        return false;
    }

    const string &symbol_to_print = obj.getSymbol();

    if(symbol_to_print != symbol_to_filter && !symbol_to_filter.empty())
    {
        //Filtered out by the user
        return true;
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
        cout << "ProcessPrint(): Symbol is not registered in the system ["
            << symbol_to_print << "]" << '\n';
        return false;
    }

    return true;
}

/**
 * This function implements Print all command
 * @param tokens for PRINT_ALL command
 * @param symbol to be shown in output
 */
bool ProcessPrintFull(const vector<string> &tokens, const string &symbol_to_filter)
{
    static PrintData obj("PRINT_FULL");
    obj.processTokens(tokens);

    if (!obj.isProcessed())
    {
        cout << "ProcessPrintFull(): Tokens were not processed successfully. Reason: ["
            << obj.errorMessage() << "]" << '\n';
        return false;
    }

    const string &symbol_to_print = obj.getSymbol();

    if(symbol_to_print != symbol_to_filter && !symbol_to_filter.empty())
    {
        //Filtered out by the user
        return true;
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
        cout << "ProcessPrintFull(): Symbol is not registered in the system ["
             << symbol_to_print << "]" << '\n';
        return false;
    }

    return true;
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

bool MdProcessor::process(const vector<string> &tokens)
{
    try
    {
        if (!tokens.empty())
        {
            return handlers_.at(tokens[MdCommandData::COMMAND_NAME])(tokens, getFilter());
        }
        else
        {
            cout << "MdProcessor::process(): Provided array of tokens is empty. Ignoring" << '\n';
            return false;
        }
    }
    catch (out_of_range &e)
    {
        cerr << "MdProcessor::process(): bad key for handler map. Exception: ["
            << string(e.what()) << "]" << '\n';
        return false;
    }
}
