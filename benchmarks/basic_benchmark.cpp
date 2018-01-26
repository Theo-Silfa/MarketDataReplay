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

using namespace std;

/******************************* Helpers ******************************/

/***************************** Benchmarks *****************************/

static void BM_memcpy(benchmark::State& state) {
  char* src = new char[state.range(0)];
  char* dst = new char[state.range(0)];
  memset(src, 'x', state.range(0));
  for (auto _ : state)
    memcpy(dst, src, state.range(0));
  state.SetBytesProcessed(int64_t(state.iterations()) *
                          int64_t(state.range(0)));
  delete[] src;
  delete[] dst;
}

BENCHMARK(BM_memcpy)->Range(8, 8<<10);

BENCHMARK_MAIN();
