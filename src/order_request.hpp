//
//  order_request.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 17.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef order_request_hpp
#define order_request_hpp

//System includes
#include <ostream>

//Local includes

using namespace std;

//Forward declarations
struct OrderRequest;
struct OrderVwap;

/**
 * Is used to define the less compare strategy for the OrderRequest structure
 * @param lhs first object for compare
 * @param rhs second object for compare
 * @return true if a < b
 */
bool operator<(const OrderRequest &lhs, const OrderRequest &rhs);

/**
 * Is used to define the greater compare strategy for the OrderRequest structure
 * @param lhs first object for compare
 * @param rhs second object for compare
 * @return true if a > b
 */
bool operator>(const OrderRequest &lhs, const OrderRequest &rhs);

/**
 * Is used to define the equal compare strategy for the OrderRequest structure
 * @param lhs first object for compare
 * @param rhs second object for compare
 * @return true if a == b
 */
bool operator==(const OrderRequest &lhs, const OrderRequest &rhs);

/**
 * Is used to define the out stream print for OrderVwap structure
 * @param out where to print
 * @param obj what to print
 * @return modified out stream
 */
ostream & operator<<(ostream &out, const OrderVwap &obj);

/**
 * Order Request class. Represents the single order
 */
struct OrderRequest
{
    uint64_t order_id;
    uint64_t quantity;
    double price;
};

/**
 * Order Vwap class. Represents the Vwap information
 */
struct OrderVwap
{
    double buy_price;
    double sell_price;
};

#endif /* order_request_hpp */
