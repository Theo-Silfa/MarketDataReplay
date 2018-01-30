//
//  split.hpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 29.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

#ifndef split_hpp
#define split_hpp

//System includes
#include <vector>
#include <iterator>
#include <string>
#include <sstream>

//Local includes

using namespace std;

/**
 * Implements the split by delimiters alghorithm
 * @param s target string
 * @param delim delimiter to be used
 * @param result where to store the tokens
 */
template<typename Out>
void split(const string &s, char delim, Out result)
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim))
    {
        if (!item.empty())
        {
            *(result++) = item;
        }
    }
}

/**
 * Wraper around the split function. Vector specialized
 * @param s target string
 * @param delim delimiter to be used
 * @return vector of tokens
 */
vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    split(s, delim, back_inserter(elems));
    return elems;
}

#endif /* split_hpp */