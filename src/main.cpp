//
//  main.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 14.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

//System includes
#include <stdlib.h>
#include <iostream>
#include <fstream>

//Local includes
#include "split.hpp"
#include "md_processor.hpp"

using namespace std;

/** Program entry point */
int main(int argc, const char * argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: md_replay <file> [<symbol>]" << '\n';
        exit(EXIT_FAILURE);
    }

    string filename, symbol;

    if (argc > 2)
    {
        symbol = argv[2];
    }

    filename = argv[1];

    ifstream infs(filename);

    if (!infs.is_open())
    {
        cerr << "failed to open " << filename << '\n';
        exit(EXIT_FAILURE);
    }

    md::processors::MdProcessor processor;

    processor.setFilter(symbol);

    //Iterate through the lines of file and feed each to the processor
    for (string line; getline( infs, line ); )
    {
        if(!line.empty())
        {
            const auto tokens = split(line, ',');
            processor.process(tokens);
        }
    }

    exit(EXIT_SUCCESS);
}
