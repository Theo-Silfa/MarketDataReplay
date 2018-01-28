//
//  basic_benchmark.cpp
//  market_data_replay
//
//  Created by Fedor Lisochenko on 26.01.2018.
//  Copyright © 2018 Fedor Lisochenko. All rights reserved.
//

//System includes
#include <benchmark/benchmark.h>

//Local includes
#include "symbol_order_list.hpp"
#include "order_bbo.hpp"

using namespace std;

/******************************* Helpers ******************************/

/***************************** Benchmarks *****************************/

static void BM_OrderListAdd(benchmark::State& state)
{
    SymbolOrderList orders("AAPL");
    uint64_t order_id = 0;
    bool side_state = false;
    uint64_t quantity = 1;
    double price = 0.0;

    for (auto _ : state)
    {
        for(int i = 1; i <= state.range(0); ++i)
        {
            orders.add(order_id++, (side_state ? "Buy" : "Sell"), quantity++, price);
            price += 0.01;
            side_state = !side_state;
        }
    }
}

BENCHMARK(BM_OrderListAdd)->Unit(benchmark::kMicrosecond)->RangeMultiplier(2)->Range(1<<10, 8<<10);


static void BM_OrderListModify(benchmark::State& state)
{
    SymbolOrderList orders("AAPL");
    uint64_t order_id = 0;
    bool side_state = false;
    uint64_t quantity = 1;
    double price = 0.01;

    for(int i = 1; i <= state.range(0); ++i)
    {
        orders.add(order_id++, (side_state ? "Buy" : "Sell"), quantity++, price);
        price += 0.01;
        side_state = !side_state;
    }

    for (auto _ : state)
    {
        order_id = 0;
        quantity = 2;
        price = 0.02;

        for(int i = 1; i <= state.range(0); ++i)
        {
            orders.modify(order_id++, quantity, price);
            quantity += 2;
            price += 0.02;
        }
    }
}

BENCHMARK(BM_OrderListModify)->Unit(benchmark::kMicrosecond)->RangeMultiplier(2)->Range(1<<10, 8<<10);

static void BM_OrderListCancel(benchmark::State& state)
{
    SymbolOrderList orders("AAPL");

    for (auto _ : state)
    {
        uint64_t order_id = 0;
        bool side_state = false;
        uint64_t quantity = 1;
        double price = 0.01;

        for(int i = 1; i <= state.range(0); ++i)
        {
            orders.add(order_id++, (side_state ? "Buy" : "Sell"), quantity++, price);
            price += 0.01;
            side_state = !side_state;
        }

        order_id = 0;

        for(int i = 1; i <= state.range(0); ++i)
        {
            orders.cancel(order_id++);
        }
    }
}

BENCHMARK(BM_OrderListCancel)->Unit(benchmark::kMicrosecond)->RangeMultiplier(2)->Range(1<<10, 8<<10);

static void BM_OrderListVwap(benchmark::State& state)
{
    SymbolOrderList orders("AAPL");
    uint64_t order_id = 0;
    bool side_state = false;
    uint64_t quantity = 1;
    double price = 0.0;

    for(int i = 1; i <= state.range(0); ++i)
    {
        orders.add(order_id++, (side_state ? "Buy" : "Sell"), quantity++, price);
        price += 0.01;
        side_state = !side_state;
    }

    auto demanded_quantity = orders.totalQuantity();

    for (auto _ : state)
    {
        auto vwap = orders.vwap(demanded_quantity);
        benchmark::DoNotOptimize(vwap);
    }
}

BENCHMARK(BM_OrderListVwap)->Unit(benchmark::kMicrosecond)->RangeMultiplier(2)->Range(1<<10, 8<<10);

static void BM_OrderListBbo(benchmark::State& state)
{
    SymbolOrderList orders("AAPL");
    uint64_t order_id = 0;
    bool side_state = false;
    uint64_t quantity = 1;
    double price = 0.0;

    for(int i = 1; i <= state.range(0); ++i)
    {
        orders.add(order_id++, (side_state ? "Buy" : "Sell"), quantity++, price);
        price += 0.01;
        side_state = !side_state;
    }

    for (auto _ : state)
    {
        auto bbo = orders.bbo();
        benchmark::DoNotOptimize(bbo);
    }
}

BENCHMARK(BM_OrderListBbo)->Unit(benchmark::kNanosecond)->RangeMultiplier(2)->Range(1<<10, 8<<10);

BENCHMARK_MAIN();
