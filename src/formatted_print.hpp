//
//  formatted_print.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 28.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef formatted_print_hpp
#define formatted_print_hpp

//System includes
#include <string>

//Local includes
#include "order_iterator.hpp"

using namespace std;

/**
* This function performs the actual bbo print upon the add, modify or cancel commands
* @param symbol to search for subscriptions
*/
void PrintBboInfo(const string &symbol);

/**
* This function performs the actual vwap print upon the add, modify or cancel commands
* @param symbol to search for subscriptions
*/
void PrintVwapInfo(const string &symbol);

/**
* This function prints down the price levels of the order book
* @param itr iterator to the order list
* @param symbol_to_print symbol to print in the header
*/
void PrintPriceLevels(OrderIterator itr, const string &symbol_to_print);

/**
* This function prints down the full order list
* @param itr iterator to the order list
* @param symbol_to_print symbol to print in the header
*/
void PrintFullOrderList(OrderIterator itr, const string &symbol_to_print);

#endif /* formatted_print_hpp */