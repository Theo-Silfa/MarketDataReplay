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
#include <map>
#include <iostream>
#include <iomanip>

//Local includes
#include "order_registry.hpp"
#include "order_iterator.hpp"
#include "order_request.hpp"

using namespace md::processors;

/*************************** Helper Functions *************************/

namespace
{
/** Default output width */
const int default_width = 10;

/** NIL output string */
const string NIL = "NIL";

/** Default output precision */
const int default_precision = 2;

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
            auto added_symbol = symbol_to_orders.insert({symbol, make_unique<SymbolOrderList>(symbol)});

            if(!added_symbol.second)
            {
                cout << "ProcessOrderAdd(): Could not register order with id [" << order_id <<
                "] and symbol [" << symbol << "]" << '\n';
                return;
            }

            added_symbol.first->second->add(order_id, side, quantity, price);
        }

        orders_active.insert({order_id, symbol});
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
void ProcessOrderModify(const CommandTokenizer &tokens)
{
    try
    {
        uint64_t order_id = stoull(tokens[CommandTokenizer::OM_ORDER_ID]);
        uint64_t quantity = stoull(tokens[CommandTokenizer::OM_QUANTITY]);
        double price = stod(tokens[CommandTokenizer::OM_PRICE]);

        auto & orders_active = OrderRegistry::get().getOrdersActive();
        auto search_for_active = orders_active.find(order_id);

        if( search_for_active == orders_active.end())
        {
            cout << "ProcessOrderModify(): Can't find order with id [" << order_id <<
            "]" << '\n';
            return;
        }

        const string & symbol = search_for_active->second;

        auto & symbol_to_orders = OrderRegistry::get().getSymbolToOrdersBind();

        auto search = symbol_to_orders.find(symbol);

        if (search != symbol_to_orders.end())
        {
            //This symbol is registered
            search->second->modify(order_id, quantity, price);
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
void ProcessOrderCancel(const CommandTokenizer &tokens)
{
    try
    {
        uint64_t order_id = stoull(tokens[CommandTokenizer::OC_ORDER_ID]);

        auto & orders_active = OrderRegistry::get().getOrdersActive();
        auto search_for_active = orders_active.find(order_id);

        if( search_for_active == orders_active.end())
        {
            cout << "ProcessOrderCancel(): Can't find order with id [" << order_id <<
            "]" << '\n';
            return;
        }

        const string & symbol = search_for_active->second;

        auto & symbol_to_orders = OrderRegistry::get().getSymbolToOrdersBind();

        auto search = symbol_to_orders.find(symbol);

        if (search != symbol_to_orders.end())
        {
            //This symbol is registered
            search->second->cancel(order_id);
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
void ProcessSubscribeBbo(const CommandTokenizer &tokens, const string &symbol_to_filter) {}

/**
 * This function implements Unsubscribe Bbo command
 * @param tokens for BBO command
 * @param symbol to be shown in output
 */
void ProcessUnsubscribeBbo(const CommandTokenizer &tokens, const string &symbol_to_filter) {}

/**
 * This function implements Subscribe Vwap command
 * @param tokens for VWAP command
 * @param symbol to be shown in output
 */
void ProcessSubscribeVwap(const CommandTokenizer &tokens, const string &symbol_to_filter) {}

/**
 * This function implements Unsubscribe Vwap command
 * @param tokens for VWAP command
 * @param symbol to be shown in output
 */
void ProcessUnsubscribeVwap(const CommandTokenizer &tokens, const string &symbol_to_filter) {}

/**
 * This function implements Print command
 * @param tokens for PRINT command
 * @param symbol to be shown in output
 */
void ProcessPrint(const CommandTokenizer &tokens, const string &symbol_to_filter)
{
    try
    {
        string symbol_to_print = tokens[CommandTokenizer::PRINT_SYMBOL];

        if(symbol_to_print != symbol_to_filter && !symbol_to_filter.empty())
        {
            //Filtered out by the user
            return;
        }

        auto & symbol_to_orders = OrderRegistry::get().getSymbolToOrdersBind();

        auto search = symbol_to_orders.find(symbol_to_print);

        if (search != symbol_to_orders.end())
        {
            //This symbol is registered

            //format for map: price is a key, value is a volume
            map<double, uint64_t, greater<double>> bid_price_levels;
            map<double, uint64_t, less<double>> ask_price_levels;

            for (auto itr = search->second->getIterator();
                 itr.done() != OrderIterator::ALL_DONE;
                 itr.next())
            {
                auto done_status = itr.done();

                if(done_status != OrderIterator::BID_DONE)
                {
                    const auto &bid_order = itr.getBid();
                    bid_price_levels[bid_order.price] += bid_order.quantity;
                }

                if(done_status != OrderIterator::ASK_DONE)
                {
                    const auto &ask_order = itr.getAsk();
                    ask_price_levels[ask_order.price] += ask_order.quantity;
                }
            }

            auto bid_itr = bid_price_levels.begin();
            auto ask_itr = ask_price_levels.begin();

            //Output format is:
            //Bid                             Ask
            //<volume>@<price> | <volume>@<price>

            cout << "|Bid      |       Ask| <--" << symbol_to_print << '\n';

            while (bid_itr != bid_price_levels.end() || ask_itr != ask_price_levels.end())
            {
                if(bid_itr != bid_price_levels.end())
                {
                    cout << '<' << bid_itr->second
                         << '@' << fixed << setprecision(default_precision) << bid_itr->first
                         << '>';
                    bid_itr++;
                }
                else
                {
                    cout << '<' << NIL
                         << '@' << NIL
                         << '>';
                }

                cout << '|';

                if(ask_itr != ask_price_levels.end())
                {
                    cout << '<' << ask_itr->second
                         << '@' << fixed << setprecision(default_precision) << ask_itr->first
                         << '>' << '\n';
                    ask_itr++;
                }
                else
                {
                    cout << '<' << NIL
                         << '@' << NIL
                         << '>' << '\n';
                }
            }

            cout << '\n';
        }
        else
        {
            cout << "ProcessPrint(): Can't find symbol [" << symbol_to_print << "]" << '\n';
            return;
        }
    }
    catch (logic_error &e)
    {
        cerr << "ProcessPrint(): Bad attempt to convert string. Exception: ["
        << string(e.what()) << "]" << '\n';
        return;
    }
}

/**
 * This function implements Print all command
 * @param tokens for PRINT_ALL command
 * @param symbol to be shown in output
 */
void ProcessPrintFull(const CommandTokenizer &tokens, const string &symbol_to_filter)
{
    try
    {
        string symbol_to_print = tokens[CommandTokenizer::PRINT_SYMBOL];

        if(symbol_to_print != symbol_to_filter && !symbol_to_filter.empty())
        {
            //Filtered out by the user
            return;
        }

        auto & symbol_to_orders = OrderRegistry::get().getSymbolToOrdersBind();

        auto search = symbol_to_orders.find(symbol_to_print);

        if (search != symbol_to_orders.end())
        {
            //This symbol is registered

            cout << '|' << setw(default_width) << "order id"
                 << '|' << setw(default_width) << "quantity"
                 << '|' << setw(default_width) << "bid price"
                 << '|' << setw(default_width) << "ask price"
                 << '|' << setw(default_width) << "quantity"
                 << '|' << setw(default_width) << "order id"
                 << '|' << " <-- " << symbol_to_print <<'\n';

            for (auto itr = search->second->getIterator();
                 itr.done() != OrderIterator::ALL_DONE;
                 itr.next())
            {
                auto DoneStatus = itr.done();

                if(DoneStatus != OrderIterator::BID_DONE)
                {
                    const auto &bid_order = itr.getBid();

                    cout << '|' << setw(default_width) << bid_order.order_id
                         << '|' << setw(default_width) << bid_order.quantity
                         << '|' << setw(default_width) << fixed << setprecision(default_precision) << bid_order.price;
                }
                else
                {
                    cout << '|' << setw(default_width) << NIL
                         << '|' << setw(default_width) << NIL
                         << '|' << setw(default_width) << NIL;
                }

                if(DoneStatus != OrderIterator::ASK_DONE)
                {
                    const auto &ask_order = itr.getAsk();

                    cout << '|' << setw(default_width) << fixed << setprecision(default_precision) << ask_order.price
                         << '|' << setw(default_width) << ask_order.quantity
                         << '|' << setw(default_width) << ask_order.order_id
                         << '|' << '\n';
                }
                else
                {
                    cout << '|' << setw(default_width) << NIL
                         << '|' << setw(default_width) << NIL
                         << '|' << setw(default_width) << NIL
                         << '|' << '\n';
                }
            }

            cout << '\n';
        }
        else
        {
            cout << "ProcessPrintFull(): Can't find symbol [" << symbol_to_print << "]" << '\n';
            return;
        }
    }
    catch (logic_error &e)
    {
        cerr << "ProcessPrintFull(): Bad attempt to convert string. Exception: ["
        << string(e.what()) << "]" << '\n';
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
