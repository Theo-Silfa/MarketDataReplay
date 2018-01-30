//
//  formatted_print.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 28.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#include "formatted_print.hpp"

//System includes
#include <map>
#include <iostream>
#include <iomanip>

//Local includes
#include "order_registry.hpp"
#include "order_request.hpp"
#include "order_bbo.hpp"

/******************************* Helpers ******************************/

namespace
{

/** Default output width */
const int default_width = 10;

/** NIL output string */
const string NIL = "NIL";

/** Default output precision */
const int default_precision = 2;

} // namespace

/**************************** Implementation **************************/

void PrintBboInfo(const string &symbol)
{
    auto &bbo_subscribers = OrderRegistry::get().getBboSubscribers();

    if (bbo_subscribers[symbol] > 0)
    {
        //We have subscribers for this symbol

        const auto &symbol_to_orders = OrderRegistry::get().getSymbolToOrdersBind();

        auto search = symbol_to_orders.find(symbol);

        if (search != symbol_to_orders.end())
        {
            //This symbol is registered
            auto bbo = search->second->bbo();

            cout << '|' << setw(default_width) << "#orders"
                << '|' << setw(default_width) << "quantity"
                << '|' << setw(default_width) << "bid price"
                << '|' << setw(default_width) << "ask price"
                << '|' << setw(default_width) << "quantity"
                << '|' << setw(default_width) << "#orders"
                << '|' << " <-- " << symbol << " BBO" << '\n';

            cout << bbo << '\n';
        }
        else
        {
            cout << "PrintBboInfo(): Can't find order list associated with this symbol: ["
                << symbol <<"]. Skipping printing bbo info" << '\n';
            return;
        }
    }
    else
    {
        //There are no subscribers for this symbol. Do nothing
        return;
    }
}

void PrintVwapInfo(const string &symbol)
{
    try
    {
        auto &vwap_subscribers = OrderRegistry::get().getVwapSubscribers();

        const auto &symbol_to_orders = OrderRegistry::get().getSymbolToOrdersBind();

        auto search = symbol_to_orders.find(symbol);

        if (search != symbol_to_orders.end())
        {
            for (const auto &vwap_info : vwap_subscribers[symbol])
            {
                if (vwap_info.second > 0)
                {
                    //We have subscribers for this quantity

                    auto vwap = search->second->vwap(vwap_info.first);

                    cout << "<buy price, sell price> <-- " << symbol <<
                        " VWAP(" << vwap_info.first << ")" << '\n';

                    cout << vwap << '\n';
                }
                else
                {
                    //There are no subscribers for this quantity. Move to the next one
                    continue;
                }
            }
        }
        else
        {
            cout << "PrintVwapInfo(): Can't find order list associated with this symbol: ["
                << symbol << "]. Skipping printing vwap info" << '\n';
            return;
        }
    }
    catch (OrderProcessException &e)
    {
        cerr << "PrintVwapInfo(): OrderProcessException: [" << string(e.what())
            << "]" << '\n';
        return;
    }
}

void PrintPriceLevels(OrderIterator itr, const string &symbol_to_print)
{
    //format for map: price is a key, value is a volume
    map<double, uint64_t, greater<double>> bid_price_levels;
    map<double, uint64_t, less<double>> ask_price_levels;

    for (;
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

    auto bid_itr = bid_price_levels.cbegin();
    auto ask_itr = ask_price_levels.cbegin();

    //Output format is:
    //Bid                             Ask
    //<volume>@<price> | <volume>@<price>

    cout << "|Bid      |       Ask| <-- " << symbol_to_print << " PRINT" << '\n';

    do
    {
        if(bid_itr != bid_price_levels.cend())
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

        if(ask_itr != ask_price_levels.cend())
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
    while (bid_itr != bid_price_levels.cend() || ask_itr != ask_price_levels.cend());
}

void PrintFullOrderList(OrderIterator itr, const string &symbol_to_print)
{
    cout << '|' << setw(default_width) << "order id"
            << '|' << setw(default_width) << "quantity"
            << '|' << setw(default_width) << "bid price"
            << '|' << setw(default_width) << "ask price"
            << '|' << setw(default_width) << "quantity"
            << '|' << setw(default_width) << "order id"
            << '|' << " <-- " << symbol_to_print << " PRINT_FULL" <<'\n';

    if (itr.done() == OrderIterator::ALL_DONE)
    {
        cout << '|' << setw(default_width) << NIL
             << '|' << setw(default_width) << NIL
             << '|' << setw(default_width) << NIL
             << '|' << setw(default_width) << NIL
             << '|' << setw(default_width) << NIL
             << '|' << setw(default_width) << NIL
             << '|' << '\n';
        return;
    }

    for (;
        itr.done() != OrderIterator::ALL_DONE;
        itr.next())
    {
        auto status = itr.done();

        if(status != OrderIterator::BID_DONE)
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

        if(status != OrderIterator::ASK_DONE)
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
}